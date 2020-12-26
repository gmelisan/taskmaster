#ifndef JOBS_H
#define JOBS_H

#include <vector>
#include <string>
#include <signal.h>

struct Job
{
    std::string name;
    std::string cmd;
    int numprocs = 1;
    std::string working_dir;
    bool autostart = true; // whether to start ad launch or not
    enum class Restart {
        Never,
        Always,
        UnexpectedExits
    } restart = Restart::UnexpectedExits;
    std::string strRestart() const;
    std::vector<int> exit_codes;
    int start_retries = 3; // how many times to restart before aborting
    int retry_count = 0;
    int start_time = 1; // how long shold be running to be considered successfully started
    int stop_time = 1; // how long to wait after graceful stop before killing
    int stop_signal = SIGTERM; // signal for graceful stopping
    std::string stdout_path;
    std::string stderr_path;
    int umask = 022;
    std::vector<std::string> env;
    int pid = 0;
    enum class Status {
        Starting,
        Running,
        Stopping,
        Stopped,
        Exited,
        Failed,
        Crashed
    } status = Status::Stopped;
    bool isAlive();
    std::string strStatus() const;
    int status_code = 0;
    time_t started_at;
    time_t stopped_at;
    bool plan2restart = false;
    bool plan2remove = false;
    bool force_stop = false;

    Job();
    Job(const Job& j);
    Job& operator=(const Job& j) = default;
};

class Jobs
{
    bool needToRestart(const Job &old_job, const Job &new_job);
    bool findByName(const std::string &name, const std::vector<Job> &jobs, Job &job);
public:
    std::vector<Job> jobs;
    bool fill(const std::string &path);
    bool reload(const std::string &path, std::vector<std::string> &to_restart,
                                         std::vector<std::string> &to_stop_and_delete,
                                         std::vector<int> &to_kill_pids);
};

#endif
