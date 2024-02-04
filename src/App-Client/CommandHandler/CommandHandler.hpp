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

class CommandHandler // Strategy
{
public:
    void SetStrategy(Commands *commands);

    void ExecuteCommand(const std::string_view cmd);

    std::vector<std::string> ListCommands();

private:
    Commands *m_Commands;
};