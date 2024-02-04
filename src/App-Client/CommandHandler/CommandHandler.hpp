#pragma once

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