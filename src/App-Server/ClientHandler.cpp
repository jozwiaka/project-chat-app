#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pqxx/pqxx>
#include "ChatDatabase.hpp"
#include "Config.hpp"
#include "JsonHandler.hpp"
#include "ClientHandler.hpp"
#include <optional>
#include "AuthenticationManager.hpp"

ClientHandler::ClientHandler(int clientSocket, ChatDatabase &database) : m_ClientSocket{clientSocket}, m_Database{database}, m_Stop{false}
{
#ifdef DEBUG
    std::cout << "ClientHandler::m_ClientSocket = " << m_ClientSocket << std::endl;
#endif
    RegisterActionHandler(Config::Actions::CREATE_USER.data(), &ClientHandler::HandleCreateUser);
    RegisterActionHandler(Config::Actions::VERIFY_USER.data(), &ClientHandler::HandleVerifyUser);
    RegisterActionHandler(Config::Actions::LIST_USERS.data(), &ClientHandler::HandleListUsers);
    RegisterActionHandler(Config::Actions::SEND_MESSAGE_TO_USER.data(), &ClientHandler::HandleSendMessageToUser);
    RegisterActionHandler(Config::Actions::SHOW_CHAT_HISTORY_WITH_USER.data(), &ClientHandler::HandleShowChatHistoryWithUser);
    RegisterActionHandler(Config::Actions::TRUNCATE_TABLES.data(), &ClientHandler::HandleTruncateTables);
    RegisterActionHandler(Config::Actions::CHECK_CONNECTION_WITH_SERVER.data(), &ClientHandler::HandleCheckConnectionWithServer);
}

ClientHandler::~ClientHandler() { Stop(); }

void ClientHandler::Start()
{
    while (!m_Stop)
    {
        std::cout << "Waiting for action..." << std::endl;

        try
        {
            HandleAction();
        }
        catch (const std::exception &e)
        {
            if (m_Stop)
            {
                break;
            }
            std::cerr << e.what() << std::endl;
            break;
        }

        if (m_Stop)
        {
            break;
        }

        std::cout << "Action handled" << std::endl;
    }
}

void ClientHandler::Stop()
{
    m_Stop = true;
    if (m_ClientSocket != -1)
    {
        std::cout << "Closing client socket..." << std::endl;
        // shutdown(m_ClientSocket, SHUT_RDWR);
        close(m_ClientSocket);
        m_ClientSocket = -1;
        std::cout << "Socket closed" << std::endl;
    }
}

void ClientHandler::RegisterActionHandler(const std::string &action, ActionHandler handler)
{
    m_ActionHandlers[action] = handler;
}

void ClientHandler::HandleAction()
{
    Json request;
    if (!JsonHandler::ReceiveJson(m_ClientSocket, request))
    {
        throw std::runtime_error("Error receiving JSON message");
    }
    std::cout << "Request: " << std::endl;
    std::cout << request << std::endl;

    std::string action = request[Config::JsonHeaders::ACTION];
    Json response;
    if (m_ActionHandlers.find(action) != m_ActionHandlers.end())
    {
        m_ActionHandlers[action](*this, request, response);
    }
    else
    {
        response[Config::JsonHeaders::STATUS] = "Unknown action";
    }

    if (!JsonHandler::SendJson(m_ClientSocket, response))
    {
        throw std::runtime_error("Error sending JSON message");
    };
    std::cout << "Response: " << std::endl;
    std::cout << response << std::endl;
}

void ClientHandler::HandleCreateUser(const Json &request, Json &response)
{
    std::string username = request[Config::JsonHeaders::User::USER][Config::JsonHeaders::User::USERNAME];
    std::string password = request[Config::JsonHeaders::User::USER][Config::JsonHeaders::User::PASSWORD];
    auto [passwordHash, salt] = AuthenticationManager::HashPassword(password);

    if (m_Database.CreateUser(username, passwordHash, salt).has_value())
    {
        response[Config::JsonHeaders::STATUS] = Config::Statuses::SUCCESS.data();
    }
    else
    {
        response[Config::JsonHeaders::STATUS] = Config::Statuses::ERROR.data();
    }
}

void ClientHandler::HandleVerifyUser(const Json &request, Json &response)
{
    const std::string &username = request[Config::JsonHeaders::User::USER][Config::JsonHeaders::User::USERNAME];
    const std::string &password = request[Config::JsonHeaders::User::USER][Config::JsonHeaders::User::PASSWORD];

    auto resultSalt = m_Database.GetSaltByUsername(username);
    auto resultPasswordHash = m_Database.GetPasswordHashByUsername(username);

    if (resultPasswordHash.has_value() && resultSalt.has_value() && !resultPasswordHash.value().empty() && !resultSalt.value().empty())
    {
        if (AuthenticationManager::VerifyPassword(password, resultPasswordHash.value()[0][0], resultSalt.value()[0][0]))
        {
            response[Config::JsonHeaders::STATUS] = Config::Statuses::SUCCESS.data();
            return;
        }
    }
    response[Config::JsonHeaders::STATUS] = Config::Statuses::ERROR.data();
}

void ClientHandler::HandleListUsers(const Json &request, Json &response)
{
    auto resultUsers = m_Database.ListUsers();
    if (resultUsers.has_value())
    {
        response[Config::JsonHeaders::User::USERS] = JsonHandler::Convert2dArrayTo2dJsonArray(resultUsers.value());
        response[Config::JsonHeaders::STATUS] = Config::Statuses::SUCCESS.data();
    }
    else
    {
        response[Config::JsonHeaders::STATUS] = Config::Statuses::ERROR.data();
    }
}

void ClientHandler::HandleSendMessageToUser(const Json &request, Json &response)
{
    std::string sender = request[Config::JsonHeaders::Message::MESSAGE][Config::JsonHeaders::Message::SENDER_USERNAME];
    std::string receiver = request[Config::JsonHeaders::Message::MESSAGE][Config::JsonHeaders::Message::RECEIVER_USERNAME];
    std::string content = request[Config::JsonHeaders::Message::MESSAGE][Config::JsonHeaders::Message::CONTENT];

    auto resultSenderID = m_Database.GetIDByUsername(sender);
    auto resultReceiverID = m_Database.GetIDByUsername(receiver);
    if (resultSenderID.has_value() && resultReceiverID.has_value())
    {
        if (m_Database.CreateMessage(resultSenderID.value()[0][0], resultReceiverID.value()[0][0], content).has_value())
        {
            response[Config::JsonHeaders::STATUS] = Config::Statuses::SUCCESS.data();
            return;
        }
    }
    response[Config::JsonHeaders::STATUS] = Config::Statuses::ERROR.data();
}

void ClientHandler::HandleShowChatHistoryWithUser(const Json &request, Json &response)
{
    std::string sender = request[Config::JsonHeaders::Message::MESSAGES][Config::JsonHeaders::Message::SENDER_USERNAME];
    std::string receiver = request[Config::JsonHeaders::Message::MESSAGES][Config::JsonHeaders::Message::RECEIVER_USERNAME];

    auto resultMessages = m_Database.ListMessages(sender, receiver);

    if (!resultMessages.has_value())
    {
        response[Config::JsonHeaders::STATUS] = Config::Statuses::ERROR.data();
        return;
    }

    std::vector<std::vector<std::string>> formattedMessages;
    for (const auto &row : resultMessages.value())
    {
        std::vector<std::string> formattedStringRow;
        std::string username = row[0];
        std::string timestamp = row[1];
        std::string header = "[" + username + ", " + timestamp + "]:";
        std::string content = row[2];
        formattedStringRow.push_back(header);
        formattedStringRow.push_back(content);
        formattedMessages.push_back(formattedStringRow);
    }
    response[Config::JsonHeaders::Message::MESSAGES] = JsonHandler::Convert2dArrayTo2dJsonArray(formattedMessages);
    response[Config::JsonHeaders::STATUS] = Config::Statuses::SUCCESS.data();
}

void ClientHandler::HandleTruncateTables(const Json &request, Json &response)
{
    if (!m_Database.ClearDatabase().has_value())
    {
        response[Config::JsonHeaders::STATUS] = Config::Statuses::ERROR.data();
        return;
    }
    response[Config::JsonHeaders::STATUS] = Config::Statuses::SUCCESS.data();
}

void ClientHandler::HandleCheckConnectionWithServer(const Json &request, Json &response)
{
    response[Config::JsonHeaders::STATUS] = Config::Statuses::SUCCESS.data();
}