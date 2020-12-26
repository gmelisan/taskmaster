#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wordexp.h>
#include <cstring>
#include <algorithm>
#include <set>
#include "JobsManager.h"
#include "Logger.h"
#include "taskmaster.h"

#define ARGVSIZE                    100
#define COMMAND_EXECUTION_TIMEOUT   5    

JobsManager manager;

JobsManager::JobsManager()
{
    pending_cmd = Command::None;
}

bool JobsManager::init(const std::string &config_path)
{
    this->config_path = config_path;
    return jobs.fill(config_path);
}

bool JobsManager::start()
{
    startAllJobs();
    while (true) {
        if (pending_cmd != Command::None) {
            //logger.info(" (->) pending command != none");
            switch (pending_cmd.load()) {
            case Command::Reload: {
                std::vector<std::string> to_restart;
                std::vector<std::string> to_stop_and_delete;
                std::vector<int> to_kill_pids;
                if (!jobs.reload(config_path, to_restart, 
                                to_stop_and_delete, to_kill_pids)) {
                    cmd_result = "fail Error while reloading configuration";
                    break ;
                }
                for (auto i : to_kill_pids) {
                    logger.info("[-:%d] Killing", i);
                    if (kill(i, SIGKILL) == -1)
                            logger.error("Can't kill pid %d: %s",
                                    i, strerror(errno));
                }
                for (auto i : to_stop_and_delete) {
                    stopJob(i, false, true);
                }
                for (auto i : to_restart) {
                    stopJob(i, true);
                }
                cmd_result = "ok";
                break ;
            }
            case Command::Start:
                startJob(cmd_arg);
                break ;
            case Command::Stop:
                stopJob(cmd_arg, false, false, true);
                break ;
            case Command::Restart:
                stopJob(cmd_arg, true);
                break ;
            }
            pending_cmd = Command::None;
            cmd_done = true;
            //logger.info(" (<-) pending command == none");
        }
        action();
        
        usleep(10000);
    }

    return true;
}

bool JobsManager::needRestart(const Job &job)
{
    if (job.restart == Job::Restart::Never)
        return false;
    if (job.restart == Job::Restart::Always)
        return true;
    if (job.status == Job::Status::Failed 
        || job.status == Job::Status::Crashed)
        return true;
    return false;
}

void JobsManager::action()
{
    if (!mtx.try_lock())
        return ;
    //std::lock_guard<std::mutex> lck(mtx);

    //logger.info(" (->) action");
    int active_jobs_count = 0;
    
    bool has_planned2remove = false;
    for (size_t i = 0; i < jobs.jobs.size(); ++i) {
        Job &job = jobs.jobs[i];

        switch (job.status) {
        case Job::Status::Starting:     // Make starting programms to be running
            job.force_stop = false;
            if (time(nullptr) - job.started_at >= job.start_time) {
                logger.info("[%s:%d] Running",
                            job.name.c_str(), job.pid);
                job.status = Job::Status::Running;
            }
            break ;
        case Job::Status::Stopping:     // Make stopping programms to be stopped
            if (time(nullptr) - job.stopped_at >= job.stop_time) {
                logger.info("[%s:%d] Killing",
                            job.name.c_str(), job.pid);
                if (kill(job.pid, SIGKILL) == -1)
                    logger.error("Can't kill pid %d: %s",
                                    job.pid, strerror(errno));
            }
            break ;
        case Job::Status::Failed:       // Restart programms
        case Job::Status::Crashed:
        case Job::Status::Stopped:
        case Job::Status::Exited:
            if (job.plan2remove)
                has_planned2remove = true;
            if (job.plan2restart) {
                startJob(i);
                job.plan2restart = false;
                break ;
            }
            if (job.force_stop) {
                break ;
            }
            if (needRestart(job)) {
                if (job.start_retries && job.retry_count < job.start_retries) {
                    startJob(i);
                    job.retry_count++;
                }
            }
            break ;
        case Job::Status::Running:
            job.retry_count = 0;
        default:
            break ;
        };

        switch (job.status) {
        case Job::Status::Starting:
        case Job::Status::Running:
        case Job::Status::Stopping:
            active_jobs_count++;
        default:
            break ;
        }
    }
    if (has_planned2remove) {
        auto it = jobs.jobs.begin();
        while (it != jobs.jobs.end()) {
            if (it->plan2remove)
                it = jobs.jobs.erase(it);
            else
                ++it;
        }
    }
    all_jobs_stopped = !active_jobs_count;
    mtx.unlock();
    //logger.info(" (<-) action");
}

bool JobsManager::executeCommand(Command cmd, const std::string &arg, std::string &result)
{
    std::lock_guard<std::mutex> lck(mtx);
    //logger.info(" (->) executeCommand");
    cmd_arg = arg;
    cmd_done = false;
    pending_cmd = cmd; // trigger for actual executing in start()
    time_t t = time(nullptr);
    while (cmd_done != true) {
        if (time(nullptr) - t >= COMMAND_EXECUTION_TIMEOUT) {
            result = "fail Command execution timeout";
            //logger.info(" (<-) executeCommand");
            return false;
        }
        usleep(1000);
    }
    result = cmd_result;
    //logger.info(" (<-) executeCommand");
    if (cmd_result == "ok")
        return true;
    return false;
}

const Jobs &JobsManager::getJobs()
{
    std::lock_guard<std::mutex> lck(mtx);
    return jobs;
}

bool JobsManager::startAllJobs()
{
   for (size_t i = 0; i < jobs.jobs.size(); ++i) {
       if (jobs.jobs[i].autostart) {
           startJob(i);
       }
   }
   return true;
}

bool JobsManager::stopAllJobs() // call only before quitting!
{
    for (size_t i = 0; i < jobs.jobs.size(); ++i) {
        jobs.jobs[i].restart = Job::Restart::Never;
        stopJob(i);
    }
    return true;
}

bool JobsManager::vec2envp(const std::vector<std::string> &vec, char ***penvp)
{
    char **envp = *penvp;
    size_t i;

    envp = (char **)malloc(sizeof(char *) * (vec.size() + 1));
    for (i = 0; i < vec.size(); ++i) {
        envp[i] = strdup(vec[i].c_str());
    }
    envp[i] = nullptr;
    *penvp = envp;
    return true;
}

static void clear_envp(char **envp)
{
    int i = 0;
    while (envp[i])
        free(envp[i++]);
    free(envp);
}

bool JobsManager::startJob(size_t i)
{
    Job *job = nullptr;

    if (i >= jobs.jobs.size()) {
        logger.error("startJob: wrong index");
        return false;
    }
    job = &jobs.jobs[i];
    if (job->isAlive()) {
        cmd_result = "fail Already started";
        return false;
    }

    wordexp_t we;
    if (!str2wordexp(job->cmd, &we)) {
        cmd_result = "fail Syntax error in '" + job->cmd + "'";
        return false;
    }
    if (!check_cmd(we.we_wordv[0])) {
        cmd_result = "fail Can't execute '" + job->name + "'";
        return false;
    }
    char **envp;
    vec2envp(job->env, &envp);
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        cmd_result = "fail fork() failed: " + std::string(strerror(errno));
        logger.error("%s", cmd_result.c_str());
        return false;
    }
    if (pid == 0) { // child
        if (!job->working_dir.empty() && chdir(job->working_dir.c_str()) == -1) {
            //logger.warning("'%s': can't set working directory to '%s': %s", 
            //                name.c_str(), job->working_dir.c_str(), strerror(errno));
        }
        umask(job->umask);
        //mode_t old_mask = umask(job->umask);
        //logger.info("'%s': set umask to %#o, old was %#o", 
        //            name.c_str(), job->umask, old_mask);
        
        if (!job->stdout_path.empty()) {
            int fd = ::open(job->stdout_path.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
            if (fd != -1) {
                dup2(fd, STDOUT_FILENO); // redirect stdout
                ::close(fd);
            }
        } else
            ::close(STDOUT_FILENO);
        
        if (!job->stderr_path.empty()) {
            int fd = ::open(job->stderr_path.c_str(), O_RDWR | O_CREAT | O_APPEND, 0644);
            if (fd != -1) {
                dup2(fd, STDERR_FILENO); // redirect stderr
                ::close(fd);
            }
        } else
            ::close(STDERR_FILENO);
        if (execve(we.we_wordv[0], we.we_wordv, envp) == -1) {
            logger.error("Can't execute '%s': %s", job->cmd.c_str(), strerror(errno));
            exit(1);
        }
    }
    // parent
    std::string real_command;
    for (size_t i = 0; i < we.we_wordc; ++i)
        real_command += we.we_wordv[i] + std::string(i == we.we_wordc - 1 ?  "" : " ");
    logger.info("[%s:%d] Starting '%s'", job->name.c_str(), pid, real_command.c_str());
    wordfree(&we);
    clear_envp(envp);
    job->status = Job::Status::Starting;
    job->started_at = time(nullptr);
    job->pid = pid;
    //logger.info("[%s:%d] Starting", job->name.c_str(), pid);
    return true;
}

bool JobsManager::startJob(const std::string &name)
{
    bool exist = false;
    int started = 0;
    int need2start = 0;
    for (size_t i = 0; i < jobs.jobs.size(); ++i) {
        if (jobs.jobs[i].name == name) {
            exist = true;
            need2start = jobs.jobs[i].numprocs;
            if (startJob(i))
                ++started;
        }
    }
    if (!exist) {
        cmd_result = "fail Unknown job '" + name + "'";
        return false;
    } else if (started == 0 && need2start == 1) {
        // cmd_result already contains reason
    } else if (started != need2start) {
        cmd_result = "fail Partial success: ";
        cmd_result += std::to_string(started) + "/" + std::to_string(need2start);
    } else {
        cmd_result = "ok";
    }
    return true;
}

bool JobsManager::str2wordexp(const std::string &str, wordexp_t *we)
{
    if (!we)
        return false;

    int ret = wordexp(str.c_str(), we, 0);
    if (ret != 0) {
        switch (ret) {
        case WRDE_BADCHAR:
            logger.error(
                R"(Error in command '%s': it contains one of the following unquoted characters: \
                `|', `&', `;', `<', `>', `(', `)', `{', `}'.)", str.c_str());
            break ;
        case WRDE_BADVAL:
            // can reach if WRDE_UNDEF flag is set
            logger.error("Error in command '%s': An attempt was made to expand an undefined shell variable.",
                        str.c_str());
            break ;
        case WRDE_CMDSUB:
            // can reach if WRDE_NOCMD flag is set
            logger.error("Error in command '%s': An attempt was made to use command substitution.", str.c_str());
            break ;
        case WRDE_NOSPACE:
            logger.error("Error in command '%s': Out of memory", str.c_str());
            break ;
        case WRDE_SYNTAX:
            logger.error("Error in command '%s': Syntax error", str.c_str());
            break ;
        default:
            logger.error("Unknown error in command '%s'", str.c_str());
            break ;
        }
        return false;
    }
    return true;
}

bool JobsManager::check_cmd(const char *cmd) 
{
    if (!cmd || strlen(cmd) == 0) {
        logger.error("Wrong command '%s'", cmd);
        return false;
    }
    struct stat st;
    if (stat(cmd, &st) == -1) {
        logger.error("Can't access '%s': %s", cmd, strerror(errno));
        return false;
    }
    return true;
}

bool JobsManager::stopJob(size_t i)
{
    Job *job = nullptr;

    if (i >= jobs.jobs.size()) {
        logger.error("startJob: wrong index");
        return false;
    }
    job = &jobs.jobs[i];
    if (job->plan2restart && !job->isAlive())
        return true;
    if (!job->isAlive()) {
        cmd_result = "fail Already stopped";
        return false;
    }
    job->status = Job::Status::Stopping;
    job->stopped_at = time(nullptr);
    logger.info("[%s:%d] Stopping", job->name.c_str(), job->pid);
    if (kill(job->pid, job->stop_signal) == -1)
                logger.error("Can't kill pid %d: %s",
                job->pid, strerror(errno));
    return true;
}

bool JobsManager::stopJob(const std::string &name, bool plan2restart, 
                          bool plan2remove, bool force_stop)
{
    bool exist = false;
    int stopped = 0;
    int need2stop = 0;
    for (size_t i = 0; i < jobs.jobs.size(); ++i) {
        if (jobs.jobs[i].name == name) {
            exist = true;
            need2stop = jobs.jobs[i].numprocs;
            if (plan2restart)
                jobs.jobs[i].plan2restart = true;
            if (plan2remove)
                jobs.jobs[i].plan2remove = true;
            if (force_stop)
                jobs.jobs[i].force_stop = true;
            if (stopJob(i))
                ++stopped;
        }
    }
    if (!exist) {
        cmd_result = "fail Unknown job '" + name + "'";
        return false;
    } else if (stopped == 0 && need2stop == 1) {
        // cmd_result already contains reason
    } else if (stopped != need2stop) {
        cmd_result = "fail Partial success: ";
        cmd_result += std::to_string(stopped) + "/" + std::to_string(need2stop);
    } else {
        cmd_result = "ok";
    }
    return true;
}

bool JobsManager::jobEvent(pid_t pid, int wstatus)
{
    std::lock_guard<std::mutex> lck(mtx);

    for (size_t i = 0; i < jobs.jobs.size(); ++i) {
        if (jobs.jobs[i].pid == pid) {
            Job &job = jobs.jobs[i];
            if (WIFEXITED(wstatus)) {
                job.status_code = WEXITSTATUS(wstatus);
                if (std::find(job.exit_codes.begin(), 
                              job.exit_codes.end(), 
                              job.status_code) != job.exit_codes.end()) {
                    job.status = Job::Status::Exited;
                    logger.info("[%s:%d] Exited (%d)", 
                                job.name.c_str(), pid, job.status_code);
                } else {
                    job.status = Job::Status::Failed;
                    logger.info("[%s:%d] Failed (%d)", 
                                job.name.c_str(), pid, job.status_code);
                }
            } else if (WIFSIGNALED(wstatus)) {
                job.status_code = WTERMSIG(wstatus);
                if (job.status == Job::Status::Stopping) {
                    job.status = Job::Status::Stopped;
                    logger.info("[%s:%d] %s", job.name.c_str(), pid,
                                job.status_code == SIGKILL ? "Killed" : "Stopped");
                } else {
                    job.status = Job::Status::Crashed;
                    logger.info("[%s:%d] Crashed (%d - %s)", 
                                job.name.c_str(), pid, 
                                job.status_code, strsignal(job.status_code));
                }
                
            }
            return true;
        }
    }
    if (WIFEXITED(wstatus)) {
        logger.info("[-:%d] Exited (%d)", pid, WEXITSTATUS(wstatus));
    } else if (WIFSIGNALED(wstatus)) {
        logger.info("[-:%d] Terminated (%d)", pid, WTERMSIG(wstatus));
    }
    return false;
}

bool JobsManager::allJobsStopped()
{
    std::lock_guard<std::mutex> lck(mtx);
    return all_jobs_stopped;
}

#undef ARGVSIZE