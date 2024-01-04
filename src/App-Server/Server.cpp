#include <iostream>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ChatDatabase.hpp"
#include "Config.hpp"
#include "ClientHandler.hpp"
#include "Server.hpp"
#include <chrono>

Server &Server::GetInstance()
{
    static Server server;
    return server;
}

void Server::Start()
{
    std::unique_lock<std::mutex> ul(m_Mtx);

    if (m_ServerThread.joinable())
    {
        std::cout << "Server is already running!" << std::endl;
        return;
    }

    m_Stop = false;

    // if ((m_ServerSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) //UDP
    if ((m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) // TCP
    {
        throw std::runtime_error("Error creating socket");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(Config::Networking::PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_ServerSocket, reinterpret_cast<struct sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1)
    {
        close(m_ServerSocket);
        throw std::runtime_error("Error binding socket");
    }

    // TCP ONLY
    if (listen(m_ServerSocket, 5) == -1)
    {
        close(m_ServerSocket);
        throw std::runtime_error("Error listening for connections");
    }
    // TCP ONLY

    std::cout << "Server is listening on " << inet_ntoa(serverAddr.sin_addr) << ":" << ntohs(serverAddr.sin_port) << std::endl;

    m_ServerThread = std::thread([this]()
                                 {
    while (!m_Stop)
    {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);

        int clientSocket = accept(m_ServerSocket, reinterpret_cast<struct sockaddr *>(&clientAddr), &clientLen);
        {
            if (m_Stop)
            {
                break;
            }

            if (clientSocket == -1)
            {
                std::cerr << "Error accepting connection" << std::endl;
                continue;
            }

            std::cout << "Accepted connection from client with IP: " << inet_ntoa(clientAddr.sin_addr) << std::endl;

            m_ClientHandlers.emplace_back(std::make_shared<ClientHandler>(clientSocket, m_Database));
            m_ClientThreads.emplace_back(&ClientHandler::Start, m_ClientHandlers.back().get());
        }
    } });
}

void Server::Stop()
{
    std::unique_lock<std::mutex> ul(m_Mtx);

    for (auto &clientHandler : m_ClientHandlers)
    {
        clientHandler->Stop();
    }

    for (auto &clientThread : m_ClientThreads)
    {
        clientThread.join();
    }

    m_ClientHandlers.clear();
    m_ClientThreads.clear();

    m_Stop = true;

    if (m_ServerSocket != -1)
    {
        shutdown(m_ServerSocket, SHUT_RDWR);
        close(m_ServerSocket);
        std::cout << "Server socket closed" << std::endl;
        m_ServerSocket = -1;
    }

    if (m_ServerThread.joinable())
    {
        m_ServerThread.join();
        std::cout << "Server stopped" << std::endl;
    }
}

void Server::Restart()
{
    Stop();
    Start();
}

void Server::Wait()
{
    m_ServerThread.join();
}

Server::~Server()
{
    Stop();
}

Server::Server() : m_Database{ChatDatabase::GetInstance()}, m_Stop{false}
{
}
