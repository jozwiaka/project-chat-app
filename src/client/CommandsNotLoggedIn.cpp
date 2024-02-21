#include <iostream>
#include "JsonHandler.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "Config.hpp"
#include "Commands.hpp"
#include "CommandsNotLoggedIn.hpp"
#include "User.hpp"

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
