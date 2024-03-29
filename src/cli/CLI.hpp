#pragma once

#include <iostream>
#include <memory>
#include "User.hpp"
#include "Commands.hpp"
#include "CommandsLoggedIn.hpp"
#include "CommandsNotLoggedIn.hpp"
#include "Client.hpp"
#include <mutex>
#include <future>
#include "CommandHandler.hpp"

class CLI
{
public:
    CLI() = default;

    void ClearScreen();

    void DisplayMenu();

    void Start();

    CLI(const CLI &) = delete;

    CLI &operator=(const CLI &) = delete;

private:
    CommandHandler m_CommandHandler;
    Client m_Client;
    User m_User;
};
