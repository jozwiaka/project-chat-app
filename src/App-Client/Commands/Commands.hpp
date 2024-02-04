#pragma once

#include <iostream>
#include "JsonHandler.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "Config.hpp"
#include "User.hpp"

class Commands
{
public:
    using Json = JsonHandler::Json;

public:
    Commands(int clientSocket, User &user);

    void Execute(const std::string_view cmd);

    std::vector<std::string> GetCommands();

    void ClearDatabase();

    bool CheckConnectionWithServer();

    virtual ~Commands() = default;

protected:
    User &m_User;
    std::map<std::string, std::function<void()>> m_CommandsFunctions;
    int m_ClientSocket;
};
