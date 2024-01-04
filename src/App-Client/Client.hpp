#pragma once

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "User.hpp"
#include "Commands.hpp"
#include "Config.hpp"

class Client
{
public:
    Client();

    int GetClientSocket();

    void Stop();

    ~Client();

private:
    int m_ClientSocket;
};
