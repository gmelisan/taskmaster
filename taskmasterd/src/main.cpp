// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: gmelisan <gmelisan@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2020/09/28 04:59:47 by gmelisan          #+#    #+#             //
//   Updated: 2020/09/28 05:31:02 by gmelisan         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <getopt.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <sys/wait.h>
#include "taskmaster.h"
#include "yaml-cpp/yaml.h"
#include "Jobs.h"
#include "Logger.h"
#include "Server.h"
#include "JobsManager.h"

#define HELPTEXT \
"Usage: " PROGRAMNAME " -[cdhnv]\n" \
"Available options:\n" \
"    -c [path]   Set path for configurations file\n" \
"    -d          Enable debug mode when messages transates to stderr\n" \
"    -h          This help\n" \
"    -l [path]   Specify path for logfile. Default is '" LOGPATH "'.\n" \
"                'syslog' can be used instead\n" \
"    -n          No fork at startup\n" \
"    -v          Print program version and exit\n" \
""

std::string config_path = DEFAULT_CONFIG_PATH;
static bool nofork = false;
static std::string log_path = LOGPATH;

static void sighandler(int n)
{
	logger.info("Caught signal %d (%s)", n, strsignal(n));
	switch (n) {
	case SIGHUP: {
		std::string res;
		manager.executeCommand(JobsManager::Command::Reload, "", res);
		break ;
	}
	case SIGINT:
	case SIGTERM:
	case SIGABRT:
		time_t t = time(nullptr);
		manager.stopAllJobs();
		while (!manager.allJobsStopped() && time(nullptr) - t <= QUIT_WAIT_TIMEOUT) {
			usleep(10000); // 10ms
		}
		logger.info("Quit");
		exit(0);
	}
}

static void daemonize()
{
	pid_t pid;
    
    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    signal(SIGCHLD, SIG_IGN);
    pid = fork();
    if (pid < 0)
        exit(EXIT_FAILURE);
    if (pid > 0)
        exit(EXIT_SUCCESS);
    umask(0);
}

static void parse_options(int argc, char **argv)
{
	int opt;

	while ((opt = getopt(argc, argv, "c:dhl:nvs")) != -1)  {
		switch (opt) {
		case 'c':
			config_path = optarg;
			break ;
		case 'd':
			debug_mode = true;
			break ;
		case 'h':
			printf("%s", HELPTEXT);
			exit(EXIT_SUCCESS);
		case 'l':
			log_path = optarg;
			break ;
		case 'n':
			nofork  = true;
			break ;
		case 'v':
			printf("%s, version %s\n", PROGRAMNAME, PROGRAMVERSION);
			exit(EXIT_SUCCESS);
		} 
	}
}

int main(int argc, char **argv)
{
	signal(SIGHUP, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGABRT, sighandler);
	signal(SIGINT, sighandler);
	parse_options(argc, argv);
	if (log_path == "syslog")
		logger.init(true);
	else
		logger.init(log_path.c_str());
	logger.info("Started, version %s", PROGRAMVERSION);
	logger.info("PID = %d", getpid());

	if (!nofork)
		daemonize();
	logger.info("Loading configuration from file '%s'", config_path.c_str());
	std::thread server_thread([]{
		Server server;
		if (!server.start())
			exit(EXIT_FAILURE);
	});
	std::thread manager_thread([]{
		if (!manager.init(config_path))
			exit(1);
		manager.start();
	});
	std::thread waiter_thread([]{
		for (;; usleep(10000)) {
			int wstatus;
			pid_t pid = wait(&wstatus);
			if (pid == -1) {
				if (errno == ECHILD)
					continue ;
				logger.perror("wait()");
			} else {
				manager.jobEvent(pid, wstatus);
			}
		}
	});
	server_thread.join();
	manager_thread.join();
	waiter_thread.join();
	return 0;
}
