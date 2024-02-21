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
}