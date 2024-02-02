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

CommandsNotLoggedIn::CommandsNotLoggedIn(int clientSocket, User &user) : Commands(clientSocket, user)
{
    m_CommandsFunctions = {
        {"signin", [this]()
         { SignIn(); }},
        {"login", [this]()
         { LogIn(); }},
    };
}

void CommandsNotLoggedIn::SignIn(std::string username, std::string password)
{
    if (username.empty())
    {
        std::cout << "Enter username:" << std::endl;
        std::getline(std::cin, username);
    }

    if (password.empty())
    {
        std::cout << "Enter password:" << std::endl;
        std::getline(std::cin, password);
    }

    Json request;
    request[Config::JsonHeaders::ACTION] = Config::Actions::CREATE_USER;
    request[Config::JsonHeaders::User::USER][Config::JsonHeaders::User::USERNAME] = username;
    request[Config::JsonHeaders::User::USER][Config::JsonHeaders::User::PASSWORD] = password;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);

    if (response[Config::JsonHeaders::STATUS] == Config::Statuses::ERROR)
    {
        std::cerr << "User with username \"" + username + "\" already exists" << std::endl;
    }
}

void CommandsNotLoggedIn::LogIn(std::string username, std::string password)
{
    if (username.empty())
    {
        std::cout << "Enter username:" << std::endl;
        std::getline(std::cin, username);
    }

    if (password.empty())
    {
        std::cout << "Enter password:" << std::endl;
        std::getline(std::cin, password);
    }

    Json request;
    request[Config::JsonHeaders::ACTION] = Config::Actions::VERIFY_USER;
    request[Config::JsonHeaders::User::USER][Config::JsonHeaders::User::USERNAME] = username;
    request[Config::JsonHeaders::User::USER][Config::JsonHeaders::User::PASSWORD] = password;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);

    if (response[Config::JsonHeaders::STATUS] == Config::Statuses::ERROR)
    {
        std::cerr << "Error: wrong username or user \"" << username << "\" does not exist." << std::endl;
        return;
    }

    m_User.LogIn(username);
}

CommandsLoggedIn::CommandsLoggedIn(int clientSocket, User &user) : Commands(clientSocket, user)
{
    m_CommandsFunctions = {
        {"message", [this]()
         { Message(); }},
        {"showchat", [this]()
         { ShowChat(); }},
        {"users", [this]()
         { ListUsers(); }},
        {"logout", [this]()
         { LogOut(); }},
    };
}

void CommandsLoggedIn::Message(std::string receiver, std::string content)
{
    if (receiver.empty())
    {
        std::cout << "Enter receiver username:" << std::endl;
        std::getline(std::cin, receiver);
    }

    if (content.empty())
    {
        std::cout << "Enter message content:" << std::endl;
        std::getline(std::cin, content);
    }

    Json request;
    request[Config::JsonHeaders::ACTION] = Config::Actions::SEND_MESSAGE_TO_USER;
    request[Config::JsonHeaders::Message::MESSAGE][Config::JsonHeaders::Message::SENDER_USERNAME] = m_User.GetUsername();
    request[Config::JsonHeaders::Message::MESSAGE][Config::JsonHeaders::Message::RECEIVER_USERNAME] = receiver;
    request[Config::JsonHeaders::Message::MESSAGE][Config::JsonHeaders::Message::CONTENT] = content;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);
}

void CommandsLoggedIn::ShowChat(std::string receiver)
{
    if (receiver.empty())
    {
        std::cout << "Enter receiver username:" << std::endl;
        std::getline(std::cin, receiver);
    }

    Json request;
    request[Config::JsonHeaders::ACTION] = Config::Actions::SHOW_CHAT_HISTORY_WITH_USER;
    request[Config::JsonHeaders::Message::MESSAGES][Config::JsonHeaders::Message::SENDER_USERNAME] = m_User.GetUsername();
    request[Config::JsonHeaders::Message::MESSAGES][Config::JsonHeaders::Message::RECEIVER_USERNAME] = receiver;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);
    JsonHandler::Print2D(response[Config::JsonHeaders::Message::MESSAGES]);
}

void CommandsLoggedIn::ListUsers()
{
    Json request;
    request[Config::JsonHeaders::ACTION] = Config::Actions::LIST_USERS;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);
    Json usersJson = response[Config::JsonHeaders::User::USERS];
    std::cout << "Users: " << std::endl;
    JsonHandler::Print2D(usersJson);
}

void CommandsLoggedIn::LogOut()
{
    m_User.LogOut();
}

void CommandHandler::SetStrategy(Commands *commands)
{
    m_Commands = commands;
}

void CommandHandler::ExecuteCommand(const std::string_view cmd)
{
    if (m_Commands)
    {
        m_Commands->Execute(cmd);
    }
}

std::vector<std::string> CommandHandler::ListCommands()
{
    std::vector<std::string> commands;
    if (m_Commands)
    {
        commands = m_Commands->GetCommands();
    }
    return commands;
}