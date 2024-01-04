#pragma once
#include <iostream>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ChatDatabase.hpp"
#include "Config.hpp"
#include "ClientHandler.hpp"
#include <mutex>
#include <condition_variable>

class Server
{
public:
    static Server &GetInstance();

    void Start();

    void Stop();

    void Restart();

    void Wait();

    ~Server();

private:
    Server();

    Server(const Server &) = delete;

    Server &operator=(const Server &) = delete;

private:
    int m_ServerSocket;
    std::thread m_ServerThread;
    std::vector<std::thread> m_ClientThreads;
    std::vector<std::shared_ptr<ClientHandler>> m_ClientHandlers;
    ChatDatabase &m_Database;
    std::mutex m_Mtx;
    std::condition_variable m_Cv;
    bool m_Stop;
};