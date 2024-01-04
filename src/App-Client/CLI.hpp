#pragma once

#include <iostream>
#include <memory>
#include "User.hpp"
#include "Commands.hpp"
#include "Client.hpp"
#include <thread>
#include <mutex>
#include <future>

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
