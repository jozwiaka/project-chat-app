#pragma once

#include <iostream>
#include "JsonHandler.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "Config.hpp"
#include "User.hpp"
#include "Commands.hpp"

class CommandsLoggedIn : public Commands
{
public:
    CommandsLoggedIn(int clientSocket, User &user);

    void Message(std::string receiver = "", std::string content = "");

    void ShowChat(std::string receiver = "");

    void ListUsers();

    void LogOut();
};
