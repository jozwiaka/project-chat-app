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
#include "Constants.hpp"

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
    request[JsonHeader::ACTION] = Action::CREATE_USER;
    request[JsonHeader::User::USER][JsonHeader::User::USERNAME] = username;
    request[JsonHeader::User::USER][JsonHeader::User::PASSWORD] = password;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);

    if (response[JsonHeader::STATUS] == Status::ERROR)
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
    request[JsonHeader::ACTION] = Action::VERIFY_USER;
    request[JsonHeader::User::USER][JsonHeader::User::USERNAME] = username;
    request[JsonHeader::User::USER][JsonHeader::User::PASSWORD] = password;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);

    if (response[JsonHeader::STATUS] == Status::ERROR)
    {
        std::cerr << "Error: wrong username or user \"" << username << "\" does not exist." << std::endl;
        return;
    }

    m_User.LogIn(username);
}
