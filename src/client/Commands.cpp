#include <iostream>
#include "JsonHandler.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "Config.hpp"
#include "Commands.hpp"
#include "User.hpp"

Commands::Commands(int clientSocket, User &user) : m_ClientSocket{clientSocket}, m_User{user} {}

void Commands::Execute(const std::string_view cmd)
{
    auto it = m_CommandsFunctions.find(cmd.data());

    if (it != m_CommandsFunctions.end())
    {
        it->second();
    }
    else
    {
        std::cout << "Unknown command." << std::endl;
    }
}

std::vector<std::string> Commands::GetCommands()
{
    std::vector<std::string> commands;

    for (const auto &pair : m_CommandsFunctions)
    {
        commands.emplace_back(pair.first);
    }

    return commands;
}

void Commands::ClearDatabase()
{
    Json request;
    request[Config::JsonHeaders::ACTION] = Config::Actions::TRUNCATE_TABLES;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);
}

bool Commands::CheckConnectionWithServer()
{
    try
    {
        Json request;
        request[Config::JsonHeaders::ACTION] = Config::Actions::CHECK_CONNECTION_WITH_SERVER;
        if (!JsonHandler::SendJson(m_ClientSocket, request))
        {
            return false;
        }

        Json response;
        if (!JsonHandler::ReceiveJson(m_ClientSocket, response))
        {
            return false;
        }
    }
    catch (const std::exception &e)
    {
        return false;
    }
    return true;
}
