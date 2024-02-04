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

class CommandsNotLoggedIn : public Commands
{
public:
    CommandsNotLoggedIn(int clientSocket, User &user);

    void SignIn(std::string username = "", std::string password = "");

    void LogIn(std::string username = "", std::string password = "");
};
