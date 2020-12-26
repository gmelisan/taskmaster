#include <iostream>
#include <cstring>
#include <algorithm>

#include "yaml-cpp/yaml.h"
#include "Jobs.h"
#include "Logger.h"

Job::Job()
{}

Job::Job(const Job& j)
{
    //logger.info("copying Job (old exit_codes.size() = %d)", j.exit_codes.size());
    name = j.name;
    cmd = j.cmd;
    numprocs = j.numprocs;
    working_dir = j.working_dir;
    autostart = j.autostart;
    restart = j.restart;
    exit_codes = j.exit_codes;
    start_retries = j.start_retries;
    start_time = j.start_time;
    stop_time = j.stop_time;
    stop_signal = j.stop_signal;
    stdout_path = j.stdout_path;
    stderr_path = j.stderr_path;
    umask = j.umask;
    env = j.env;
    pid = j.pid;
    status = j.status;
    status_code = j.status_code;
    started_at = j.started_at;
    stopped_at = j.stopped_at;
    plan2restart = j.plan2restart;
}

bool Job::isAlive()
{
    switch (status) {
    case Status::Starting:
    case Status::Running:
    case Status::Stopping:
        return true;
    default:
        return false;
    }
}

std::string Job::strRestart() const
{
    switch (restart) {
    case Restart::Always:
        return "always";
    case Restart::Never:
        return "never";
    case Restart::UnexpectedExits:
        return "unexpected";
    }
    return "unknown";
}

std::string Job::strStatus() const
{
    switch (status) {
    case Status::Starting:
        return "starting";
    case Status::Running:
        return "running";
    case Status::Stopping:
        return "stopping";
    case Status::Stopped:
        return "stopped";
    case Status::Exited:
        return "exited";
    case Status::Crashed:
        return "crashed";
    case Status::Failed:
        return "failed";
    }
    return "unknown";
}

static std::string vecint2str(const std::vector<int> &vec)
{
    std::string res;
    if (vec.empty())
        return "(empty)";
    for (size_t i = 0; i < vec.size(); ++i) {
        res += std::to_string(vec[i]);
        if (i != vec.size() - 1)
            res += ", ";
    }
    return res;
}


static std::string vecstr2str(const std::vector<std::string> &vec)
{
    std::string res;
    if (vec.empty())
        return "(empty)";
    for (size_t i = 0; i < vec.size(); ++i) {
        res += vec[i];
        if (i != vec.size() - 1)
            res += ", ";
    }
    return res;
}

bool Jobs::fill(const std::string &path)
{
    YAML::Node cfg;
    YAML::Node programs;
    jobs = {};
    try {
        cfg = YAML::LoadFile(path);
        programs = cfg["programs"];
    } catch (YAML::Exception &ex) {
        logger.error("Can't load configuration: %s", ex.msg.c_str());
        return false;
    }
    if (programs.IsMap()) {
        for (YAML::const_iterator it = programs.begin(); it != programs.end(); ++it) {
            try {
                std::string name = it->first.as<std::string>();
                logger.info("Loading '%s':", name.c_str());
                //jobs.push_back({});
                //Job &job = jobs.back();
                Job job;
                job.name = name;
                YAML::Node program = it->second;
                if (program.IsMap()) {
                    if (!program["cmd"].IsDefined()) {
                        logger.error("No cmd. Skipping");
                        continue ;
                    }
                    job.cmd = program["cmd"].as<std::string>();
                    logger.info("\tcmd: '%s'", job.cmd.c_str());
                    if (program["numprocs"].IsDefined()) {
                        job.numprocs = program["numprocs"].as<int>();
                        logger.info("\tnumproc: %d", job.numprocs);
                    }
                    if (program["workingdir"].IsDefined()) {
                        job.working_dir = program["workingdir"].as<std::string>();
                        logger.info("\tworking_dir: '%s'", job.working_dir.c_str());
                    }
                    if (program["autostart"].IsDefined()) {
                        job.autostart = program["autostart"].as<bool>();
                        logger.info("\tautostart: %s", job.autostart ? "true" : "false");
                    }
                    if (program["autorestart"].IsDefined()) {
                        std::string restart = program["autorestart"].as<std::string>();
                        if (restart == "never") {
                            job.restart = Job::Restart::Never;
                            logger.info("\trestart: Never");
                        } else if (restart == "always") {
                            job.restart = Job::Restart::Always;
                            logger.info("\trestart: Always");
                        } else {
                            job.restart = Job::Restart::UnexpectedExits;
                            logger.info("\trestart: UnexpectedExits");
                        }
                    }
                    if (program["exitcodes"].IsDefined()) {
                        if (program["exitcodes"].IsSequence())
                            job.exit_codes = program["exitcodes"].as<std::vector<int>>();
                        else
                            job.exit_codes.emplace_back(program["exitcodes"].as<int>());
                        logger.info("\texitcodes: %s", vecint2str(job.exit_codes).c_str());
                    } else {
                        job.exit_codes.push_back(0);
                    }
                    if (program["startretries"].IsDefined()) {
                        job.start_retries = program["startretries"].as<int>();
                        logger.info("\tstart_retries: %d", job.start_retries);
                    }
                    if (program["starttime"].IsDefined()) {
                        job.start_time = program["starttime"].as<int>();
                        logger.info("\tstart_time: %d", job.start_time);
                    }
                    if (program["stoptime"].IsDefined()) {
                        job.stop_time = program["stoptime"].as<int>();
                        logger.info("\tstop_time: %d", job.stop_time);
                    }
                    if (program["stopsignal"].IsDefined()) {
                        job.stop_signal = program["stopsignal"].as<int>();
                        logger.info("\tstop_signal: %d (%s)", job.stop_signal, strsignal(job.stop_signal));
                    }
                    if (program["stdout"].IsDefined()) {
                        job.stdout_path = program["stdout"].as<std::string>();
                        logger.info("\tstdout: '%s'", job.stdout_path.c_str());
                    }
                    if (program["stderr"].IsDefined()) {
                        job.stderr_path = program["stderr"].as<std::string>();
                        logger.info("\tstderr: '%s'", job.stderr_path.c_str());
                    }
                    if (program["umask"].IsDefined()) {
                        std::string umask_str = program["umask"].as<std::string>();
                        job.umask = std::stoi(umask_str, nullptr, 0);
                        logger.info("\tumask: %s (%d)", umask_str.c_str(), job.umask);
                    }
                    if (program["env"].IsDefined()) {
                        if (program["env"].IsMap()) {
                            for (YAML::const_iterator it_env = program["env"].begin(); 
                                    it_env != program["env"].end(); ++it_env) {
                                job.env.push_back(it_env->first.as<std::string>() + "=" + 
                                                it_env->second.as<std::string>());
                            }
                        }
                        logger.info("\tenv: %s", vecstr2str(job.env).c_str());
                    }
                    if (job.numprocs >= 1) {
                        for (int i = 0; i < job.numprocs; ++i) {
                            jobs.push_back(job);
                        }
                    }
                } else {
                    logger.error("'%s' is not a map", name.c_str());
                    return false;
                }
            } catch (YAML::Exception &ex) {
                if (ex.mark.line >= 0 && ex.mark.column >=0) 
                    logger.warning("Error while parsing configuration:%d:%d: %s", 
                        ex.mark.line + 1, ex.mark.column, ex.msg.c_str());
                else
                    logger.warning("Error while parsing configuration: %s", ex.msg.c_str());
                continue ;
            }
        }
    }
    
    return true;
}

static std::vector<int> find_pids(std::string name, std::vector<Job> jobs)
{
    std::vector<int> res;
    for (auto i : jobs) {
        if (i.name == name && i.isAlive())
            res.push_back(i.pid);
    }
    return res;
}

bool Jobs::reload(const std::string &path, std::vector<std::string> &to_restart, 
                                           std::vector<std::string> &to_stop_and_delete,
                                           std::vector<int> &to_kill_pids)
{
    logger.info("Reloading configuration from '%s'", path.c_str());
    std::vector<Job> old_jobs = jobs;
    if (!fill(path)) {
        jobs = old_jobs;
        return false;
    }
    for (size_t i = 0; i < jobs.size(); ++i) {
        Job old_job;
        if (findByName(jobs[i].name, old_jobs, old_job)) {
            if (jobs[i].numprocs == 1) {
                jobs[i].status = old_job.status;
                jobs[i].pid = old_job.pid;
            } else if (jobs[i].numprocs != old_job.numprocs) {
                to_kill_pids = find_pids(jobs[i].name, old_jobs);
            }
            jobs[i].status_code = old_job.status_code;
            jobs[i].started_at = old_job.started_at;
            jobs[i].stopped_at = old_job.stopped_at;
            if (needToRestart(old_job, jobs[i]))
                to_restart.push_back(jobs[i].name);
        } else {
            if (jobs[i].autostart)
                to_restart.push_back(jobs[i].name);
        }
    }
    for (size_t i = 0; i < old_jobs.size(); ++i) {
        Job new_job;
        if (!findByName(old_jobs[i].name, jobs, new_job)) {
            jobs.push_back(old_jobs[i]);
            to_stop_and_delete.push_back(old_jobs[i].name);
        }
    }
    // remove duplicates for case when some programs has numprocs > 1
    std::sort(to_restart.begin(), to_restart.end());
    to_restart.erase(std::unique(to_restart.begin(), to_restart.end()), to_restart.end());
    if (to_restart.size())
        logger.info("Need to (re)start %ld jobs", to_restart.size());
    else
        logger.info("No jobs need restart");
    
    return true;
}

bool Jobs::findByName(const std::string &name, const std::vector<Job> &jobs, Job &job)
{
    for (size_t i = 0; i < jobs.size(); ++i) {
        if (jobs[i].name == name) {
            job = jobs[i];
            return true;
        }
    }
    return false;
}

bool Jobs::needToRestart(const Job &old_job, const Job &new_job)
{
    if (old_job.autostart == false && new_job.autostart == true)
        return true;
    if (new_job.autostart == false)
        return false;
    if (old_job.cmd != new_job.cmd
            || old_job.numprocs != new_job.numprocs
            || old_job.working_dir != new_job.working_dir
            || old_job.stdout_path != new_job.stdout_path
            || old_job.stderr_path != new_job.stderr_path
            || old_job.umask != new_job.umask
            || old_job.env != new_job.env)
        return true;
    return false;
}