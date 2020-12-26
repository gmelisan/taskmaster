#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "taskmaster.h"
#include "Server.h"
#include "Logger.h"
#include "JobsManager.h"
#include "ft_socket.h"
#include "taskmaster.h"

#define BUFSIZE     100
#define BACKLOG     1

static void clear_array(char **arr)
{
    if (!arr)
        return ;
    int i = 0;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}

bool Server::start()
{
    struct sockaddr_un addr = {};
    int sfd, cfd;
    ssize_t num_read;
    char buf[BUFSIZE] = {};
    bool shutdown = false;

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1) {
        logger.perror("Can't create socket");
        return false;
    }
    logger.info("Socket path: %s", SOCKET_PATH);
    if (remove(SOCKET_PATH) == -1 && errno != ENOENT) {
        logger.perror("Can't delete old socket");
        return false;
    }
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    if (bind(sfd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(struct sockaddr_un)) == -1) {
        logger.perror("Can't bind socket");
        return false;
    }
    if (listen(sfd, BACKLOG) == -1) {
        logger.perror("Can't start listening on socket");
        return false;
    }
    logger.info("Server started");
    while (true) {
        cfd = accept(sfd, nullptr, nullptr);
        if (cfd == -1) {
            logger.perror("Error in accept()");
            return false;
        }
        num_read = read(cfd, buf, BUFSIZE);
        if (num_read == -1) {
            logger.perror("Error in read()");
            return false;
        }
        char **msgv = nullptr;
        int msgc;
        //logger.info("numRead = %d", num_read);
        decode_message(buf, &msgv, &msgc);
        std::string income;
        std::string reply;
        for (int i = 0; i < msgc; ++i) {
            income += std::string(msgv[i]) + (i == msgc - 1 ? "" : " ");
        }
        if (!msgv || !msgv[0] || msgc < 1) {
            logger.error("Invalid command");
            continue ;
        } else {
            logger.info("Get command: '%s'", income.c_str());
        }
        
        if (strcmp(msgv[0], "start") == 0) {
            if (msgc < 2) {
                reply = "fail No argument";
            } else {
                for (int i = 1; i < msgc; ++i)
                    manager.executeCommand(JobsManager::Command::Start, msgv[i], reply);
            }
        } else if (strcmp(msgv[0], "stop") == 0) {
            if (msgc < 2) {
                reply = "fail No argument";
            } else {
                for (int i = 1; i < msgc; ++i)
                    manager.executeCommand(JobsManager::Command::Stop, msgv[i], reply);
            }
        } else if (strcmp(msgv[0], "restart") == 0) {
            if (msgc < 2) {
                reply = "fail No argument";
            } else {
                for (int i = 1; i < msgc; ++i)
                    manager.executeCommand(JobsManager::Command::Restart, msgv[i], reply);
            }
        } else if (strcmp(msgv[0], "status") == 0) {
            Jobs jobs = manager.getJobs();
            reply = buildStatusReply(jobs, msgc > 1 ? msgv[1] : "");
        } else if (strcmp(msgv[0], "reload") == 0) {
            manager.executeCommand(JobsManager::Command::Reload, "", reply);
        } else if (strcmp(msgv[0], "shutdown") == 0) {
            shutdown = true;
            reply = "ok";
        } else {
            reply = "fail Unknown command";
        }
        clear_array(msgv);
        logger.info("Reply: '%s'", reply.c_str());
        char *bufwrite;
        int sizewrite;
        encode_message(reply.c_str(), &bufwrite, &sizewrite);
        if (write(cfd, bufwrite, sizewrite) == -1)
            logger.perror("socket write");
        free(bufwrite);
        if (close(cfd) < 0) {
            logger.perror("Error in close()");
            return false;
        }
        if (shutdown) {
            raise(SIGTERM);
            /*
            time_t t = time(nullptr);
            manager.stopAllJobs();
            while (!manager.allJobsStopped() && time(nullptr) - t <= QUIT_WAIT_TIMEOUT) {
                usleep(10000); // 10ms
            }
            logger.info("Quit");
            exit(0);
            */
        }
    }
    return true;
}

std::string Server::buildStatusReply(const Jobs &jobs, const std::string &name)
{
    std::string res;
    std::string after_n;
    int n = 0;

    for (size_t i = 0; i < jobs.jobs.size(); ++i) {
        const Job &job = jobs.jobs[i];
        if (name.empty() || job.name == name) {
            after_n += "\"" + job.name + "\" ";
            after_n += "\"" + job.cmd + "\" ";
            after_n += std::to_string(job.pid) + " ";
            after_n += std::to_string(job.start_retries) + " ";
            after_n += "\"" + job.strStatus() + " ";
            after_n += std::to_string(job.status_code) + "\" ";
            n++;
        }
    }
    if (n) {
        after_n.pop_back();
        res = "ok " + std::to_string(n) + " " + after_n;
    } else if (name.empty()) {
        res = "fail No jobs in config file";
    } else {
        res = "fail Unknown job '" + name + "'";
    }
    return res;
}

#undef BACKLOG
#undef BUFSIZE
