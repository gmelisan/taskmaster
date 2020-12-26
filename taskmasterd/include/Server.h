#ifndef SERVER_H
#define SERVER_H

#include "Jobs.h"

class Server
{
    std::string buildStatusReply(const Jobs &jobs, const std::string &name);
public:
    bool start();
};

#endif