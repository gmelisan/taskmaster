#ifndef JOBSMANAGER_H
#define JOBSMANAGER_H

#include <sys/types.h>
#include <wordexp.h>
#include <mutex>
#include <atomic>
#include <string>
#include "Jobs.h"

class JobsManager
{
public:
    enum Command {
        None,
        Reload,
        Start,
        Stop,
        Restart
    };

private:
    std::string config_path;
    Jobs jobs;
    bool all_jobs_stopped = false;
    std::atomic_int pending_cmd;
    std::string cmd_arg;
    std::string cmd_result;
    mutable std::mutex mtx;
    std::atomic_bool cmd_done;

    bool startJob(size_t i);
    bool startJob(const std::string &name);
    bool stopJob(size_t i);
    bool stopJob(std::vector<Job>::iterator it);
    bool stopJob(const std::string &name, 
                        bool plan2restart = false, 
                        bool plan2remove = false,
                        bool force_stop = false);
    bool str2wordexp(const std::string &str, wordexp_t *we);
    bool vec2envp(const std::vector<std::string> &vec, char ***envp);
    bool check_cmd(const char *cmd);
    bool needRestart(const Job &job);

public:
    JobsManager();
    bool init(const std::string &config_path);
    bool start();
    void action();
    bool startAllJobs();
    bool stopAllJobs();
    bool executeCommand(Command cmd, const std::string &arg, std::string &result);
    bool jobEvent(pid_t pid, int wstatus);
    const Jobs &getJobs();
    bool allJobsStopped();

};

extern JobsManager manager;

#endif