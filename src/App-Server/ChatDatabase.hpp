#pragma once

#include <iostream>
#include <pqxx/pqxx>
#include "Config.hpp"
#include "JsonHandler.hpp"
#include <optional>

class ChatDatabase
{
public:
    using QeuryResultType = std::optional<std::vector<std::vector<std::string>>>;

public:
    static ChatDatabase &GetInstance();

    QeuryResultType CreateUser(const std::string &username, const std::string &passwordHash, const std::string &salt);

    QeuryResultType ListUsers();

    QeuryResultType ListMessages(const std::string &senderUsername, const std::string &receiverUsername);

    QeuryResultType ClearDatabase();

    QeuryResultType GetPasswordHashByUsername(const std::string &username);

    QeuryResultType GetSaltByUsername(const std::string &username);

    QeuryResultType GetIDByUsername(const std::string &username);

    QeuryResultType CreateMessage(const std::string &senderID, const std::string &receiverID, const std::string &content);

private:
    ChatDatabase(const ChatDatabase &) = delete;

    ChatDatabase &operator=(const ChatDatabase &) = delete;

    ChatDatabase();

    QeuryResultType ExecuteQuery(const std::string &query);

    std::string CreateConnectionString();

private:
    pqxx::connection m_Connection;
};
