#include <iostream>
#include <memory>
#include "User.hpp"
#include "Commands.hpp"
#include "Client.hpp"
#include "CLI.hpp"
#include <cstdlib>
#include <future>

void CLI::DisplayMenu()
{
    std::cout << "=========  CLI =========" << std::endl;
    if (m_User.IsLoggedIn())
    {
        std::cout << "Logged as " << m_User.GetUsername() << std::endl;
        std::cout << "========================" << std::endl;
    }
    for (const auto &option : m_CommandHandler.ListCommands())
    {
        std::cout << option << std::endl;
    }

    std::cout << "========================" << std::endl;
}

void CLI::Start()
{
    CommandsNotLoggedIn commandsNotLoggedIn{m_Client.GetClientSocket(), m_User};
    CommandsLoggedIn commandsLoggedIn{m_Client.GetClientSocket(), m_User};

    while (true)
    {
        if (m_User.IsLoggedIn())
        {
            m_CommandHandler.SetStrategy(&commandsLoggedIn);
        }
        else
        {
            m_CommandHandler.SetStrategy(&commandsNotLoggedIn);
        }

        DisplayMenu();
        std::string input;
        std::cout << ">> ";
        std::getline(std::cin, input);

        if (input == "q")
        {
            break;
        }
        else
        {
            m_CommandHandler.ExecuteCommand(input);
        }
    }
}
