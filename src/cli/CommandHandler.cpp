#include "Commands.hpp"
#include "CommandHandler.hpp"

void CommandHandler::SetStrategy(Commands *commands)
{
    m_Commands = commands;
}

void CommandHandler::ExecuteCommand(const std::string_view cmd)
{
    if (m_Commands)
    {
        m_Commands->Execute(cmd);
    }
}

std::vector<std::string> CommandHandler::ListCommands()
{
    std::vector<std::string> commands;
    if (m_Commands)
    {
        commands = m_Commands->GetCommands();
    }
    return commands;
}