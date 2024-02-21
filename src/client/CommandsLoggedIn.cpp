#include <iostream>
#include "JsonHandler.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "Config.hpp"
#include "Commands.hpp"
#include "CommandsLoggedIn.hpp"
#include "User.hpp"
#include "Constants.hpp"

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
    request[JsonHeader::ACTION] = Action::SEND_MESSAGE_TO_USER;
    request[JsonHeader::Message::MESSAGE][JsonHeader::Message::SENDER_USERNAME] = m_User.GetUsername();
    request[JsonHeader::Message::MESSAGE][JsonHeader::Message::RECEIVER_USERNAME] = receiver;
    request[JsonHeader::Message::MESSAGE][JsonHeader::Message::CONTENT] = content;
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
    request[JsonHeader::ACTION] = Action::SHOW_CHAT_HISTORY_WITH_USER;
    request[JsonHeader::Message::MESSAGES][JsonHeader::Message::SENDER_USERNAME] = m_User.GetUsername();
    request[JsonHeader::Message::MESSAGES][JsonHeader::Message::RECEIVER_USERNAME] = receiver;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);
    JsonHandler::Print2D(response[JsonHeader::Message::MESSAGES]);
}

void CommandsLoggedIn::ListUsers()
{
    Json request;
    request[JsonHeader::ACTION] = Action::LIST_USERS;
    JsonHandler::SendJson(m_ClientSocket, request);

    Json response;
    JsonHandler::ReceiveJson(m_ClientSocket, response);
    Json usersJson = response[JsonHeader::User::USERS];
    std::cout << "Users: " << std::endl;
    JsonHandler::Print2D(usersJson);
}

void CommandsLoggedIn::LogOut()
{
    m_User.LogOut();
}