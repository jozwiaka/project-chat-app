#pragma once
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "Config.hpp"

class JsonHandler
{
public:
    using Json = nlohmann::json;

public:
    static bool ReceiveJson(int socket, Json &response);

    static bool SendJson(int socket, const Json &request);

    template <typename T>
    static Json Convert2dArrayTo2dJsonArray(const std::vector<std::vector<T>> &v2d)
    {
        Json j2d;
        for (const auto &row : v2d)
        {
            Json jRow;
            for (const auto &col : row)
            {
                jRow.push_back(col);
            }
            j2d.push_back(jRow);
        }

        return j2d;
    }

    template <typename T>
    static std::vector<std::vector<T>> Convert2dJsonArrayTo2dArray(const Json &j2d);

    template <typename T>
    static void Print2D(const std::vector<std::vector<T>> &v2d);

    static void Print2D(const Json &j2d);
};
