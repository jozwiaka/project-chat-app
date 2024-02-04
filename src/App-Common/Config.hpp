#pragma once
#include <string_view>
#include <fstream>
#include <filesystem>

namespace Config
{
    namespace ChatDatabase
    {
        const std::string_view HOST = (std::getenv("DOCKER_CONTAINER") != nullptr) ? "postgres-service" : "localhost";
        constexpr int PORT = 5432;
        constexpr std::string_view DBNAME = "db_chat";
        constexpr std::string_view USER = "jozwiaka";
        constexpr std::string_view PASSWORD = "password";
    }
    namespace Networking
    {
        // If a server is listening on 0.0.0.0 for a certain port, it will accept connections from any external IP address or from the localhost. When a server binds to 127.0.0.1, it will only accept connections that originate from the same machine (the local loopback interface). It's used for communication between processes on the same machine
        // [Local/Docker-Server] Server listens on 0.0.0.0:8080
        // [Local-Client] Client connects to 127.0.0.1:8080
        // [Docker-Client] Client connects to 172.20.0.1:8080 (or directly to 172.20.0.3:8080)
        // 172.20.0.1 - Address from a Docker bridge network. Used as the gateway for containers within that Docker bridge network
        const std::string_view SERVER_IP_ADDRESS = (std::getenv("DOCKER_CONTAINER") != nullptr) ? "172.20.0.3" : "127.0.0.1";
        constexpr int PORT = 8080;
        constexpr int BUFFER_SIZE = 1024;
    }

    namespace JsonHeaders
    {
        constexpr std::string_view ACTION = "action";
        constexpr std::string_view STATUS = "status";

        namespace User
        {
            constexpr std::string_view USER = "user";
            constexpr std::string_view USERS = "users";

            constexpr std::string_view USER_ID = "user_id";
            constexpr std::string_view USERNAME = "username";
            constexpr std::string_view PASSWORD = "password";
            constexpr std::string_view PASSWORD_HASH = "password_hash";
            constexpr std::string_view SALT = "salt";
            constexpr std::string_view CREATED_AT = "created_at";

        }

        namespace Message
        {
            constexpr std::string_view MESSAGE = "message";
            constexpr std::string_view MESSAGES = "messages";

            constexpr std::string_view MESSAGE_ID = "message_id";
            constexpr std::string_view SENDER_ID = "sender_id";
            constexpr std::string_view RECEIVER_ID = "receiver_id";
            constexpr std::string_view CONTENT = "content";
            constexpr std::string_view SENT_AT = "sent_at";

            constexpr std::string_view SENDER_USERNAME = "sender_username";
            constexpr std::string_view RECEIVER_USERNAME = "receiver_username";
        }
    }

    namespace Actions
    {
        constexpr std::string_view CREATE_USER = "CREATE_USER";
        constexpr std::string_view VERIFY_USER = "VERIFY_USER";
        constexpr std::string_view LIST_USERS = "LIST_USERS";
        constexpr std::string_view SEND_MESSAGE_TO_USER = "SEND_MESSAGE_TO_USER";
        constexpr std::string_view SHOW_CHAT_HISTORY_WITH_USER = "SHOW_CHAT_HISTORY_WITH_USER";
        constexpr std::string_view TRUNCATE_TABLES = "TRUNCATE_TABLES";
        constexpr std::string_view CHECK_CONNECTION_WITH_SERVER = "CHECK_CONNECTION_WITH_SERVER";
    }

    namespace Statuses
    {
        constexpr std::string_view SUCCESS = "Success";
        constexpr std::string_view ERROR = "Error";
    }
}