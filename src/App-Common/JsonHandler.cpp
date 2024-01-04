#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "Config.hpp"
#include "JsonHandler.hpp"

bool JsonHandler::ReceiveJson(int socket, Json &response)
{
    constexpr size_t bufferSize = Config::Networking::BUFFER_SIZE;
    std::vector<char> buffer(bufferSize, 0);

    ssize_t bytesRead = recv(socket, buffer.data(), buffer.size(), 0);
    if (bytesRead <= 0)
    {
        return false;
    }

    try
    {
        response = Json::parse(buffer.data());
        return true;
    }
    catch (const Json::exception &e)
    {
        std::cerr << "Error parsing JSON: " << e.what() << '\n';
        return false;
    }
}

bool JsonHandler::SendJson(int socket, const Json &request)
{
    std::string jsonStr = request.dump();

    ssize_t bytesSent = send(socket, jsonStr.c_str(), jsonStr.size(), 0);
    if (bytesSent == -1)
    {
        std::cerr << "Error sending JSON message\n";
        return false;
    }

    return true;
}

template <typename T>
std::vector<std::vector<T>> JsonHandler::Convert2dJsonArrayTo2dArray(const Json &j2d)
{
    std::vector<std::vector<T>> v2d;

    try
    {
        for (const auto &row : j2d)
        {
            v2d.emplace_back(row.get<std::vector<T>>());
        }
    }
    catch (const Json::exception &e)
    {
        std::cerr << "Error converting JSON to 2D array: " << e.what() << '\n';
    }

    return v2d;
}

template <typename T>
void JsonHandler::Print2D(const std::vector<std::vector<T>> &v2d)
{
    for (const auto &row : v2d)
    {
        for (const auto &elem : row)
        {
            std::cout << elem << ' ';
        }
        std::cout << '\n';
    }
}

void JsonHandler::Print2D(const Json &j2d)
{
    for (const auto &row : j2d)
    {
        for (const auto &elem : row)
        {
            std::cout << elem.get<std::string>() << ' ';
        }
        std::cout << '\n';
    }
}