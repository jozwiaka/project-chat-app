#include <iostream>
#include <pqxx/pqxx>
#include "Config.hpp"
#include "JsonHandler.hpp"
#include "ChatDatabase.hpp"
#include <string>
#include <optional>

ChatDatabase &ChatDatabase::GetInstance()
{
    static ChatDatabase instance;
    return instance;
}

ChatDatabase::QeuryResultType ChatDatabase::CreateUser(const std::string &username, const std::string &passwordHash, const std::string &salt)
{
    std::string query = "INSERT INTO users (username, password_hash, salt) VALUES ('" + username + "', '" + passwordHash + "', '" + salt + "');";
    return ExecuteQuery(query);
}

ChatDatabase::QeuryResultType ChatDatabase::ListUsers()
{
#ifdef DEBUG
    std::string query = "SELECT * FROM users;";
#else
    std::string query = "SELECT username FROM users;";
#endif
    return ExecuteQuery(query);
}

ChatDatabase::QeuryResultType ChatDatabase::ListMessages(const std::string &senderUsername, const std::string &receiverUsername)
{
    std::string query = "SELECT u_sender.username, m.sent_at, m.content "
                        "FROM messages m "
                        "JOIN users u_sender ON m.sender_id = u_sender.user_id "
                        "JOIN users u_receiver ON m.receiver_id = u_receiver.user_id "
                        "WHERE (u_sender.username = '" +
                        senderUsername + "' AND u_receiver.username = '" + receiverUsername + "') OR (u_sender.username = '" + receiverUsername + "' AND u_receiver.username = '" + senderUsername + "') "
                                                                                                                                                                                                     "ORDER BY m.sent_at;";
    return ExecuteQuery(query);
}

ChatDatabase::QeuryResultType ChatDatabase::ClearDatabase()
{
    std::string query = "TRUNCATE TABLE messages, users;";
    return ExecuteQuery(query);
}

ChatDatabase::QeuryResultType ChatDatabase::GetPasswordHashByUsername(const std::string &username)
{
    std::string query = "SELECT password_hash FROM users WHERE username = '" + username + "';";
    return ExecuteQuery(query);
}

ChatDatabase::QeuryResultType ChatDatabase::GetSaltByUsername(const std::string &username)
{
    std::string query = "SELECT salt FROM users WHERE username = '" + username + "';";
    return ExecuteQuery(query);
}

ChatDatabase::QeuryResultType ChatDatabase::GetIDByUsername(const std::string &username)
{
    std::string query = "SELECT user_id FROM users WHERE username = '" + username + "';";
    return ExecuteQuery(query);
}

ChatDatabase::QeuryResultType ChatDatabase::CreateMessage(const std::string &senderID, const std::string &receiverID, const std::string &content)
{
    std::string query = "INSERT INTO messages(sender_id, receiver_id, content) VALUES ('" + senderID + "', '" + receiverID + "', '" + content + "')";
    return ExecuteQuery(query);
}

ChatDatabase::ChatDatabase() : m_Connection{CreateConnectionString()}
{
    if (m_Connection.is_open())
    {
        std::cout << "Connected to database" << std::endl;
    }
    else
    {
        throw std::runtime_error("Connection to database failed");
    }
}

ChatDatabase::QeuryResultType ChatDatabase::ExecuteQuery(const std::string &query)
{
    try
    {
        pqxx::work txn(m_Connection); // transaction
        pqxx::result table = txn.exec(query);
        txn.commit();

        std::vector<std::vector<std::string>> result;
        for (const auto &row : table)
        {
            std::vector<std::string> r;
            for (const auto &col : row)
            {
                r.push_back(pqxx::to_string(col));
            }
            result.push_back(r);
        }
        return result;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    }
}

std::string ChatDatabase::CreateConnectionString()
{
    const std::string host = Config::ChatDatabase::HOST.data();
    const int port = Config::ChatDatabase::PORT;
    const std::string dbname = Config::ChatDatabase::DBNAME.data();
    const std::string user = Config::ChatDatabase::USER.data();
    const std::string password = Config::ChatDatabase::PASSWORD.data();
    return "host=" + host + " port=" + std::to_string(port) +
           " dbname=" + dbname + " user=" + user + " password=" + password;
}
