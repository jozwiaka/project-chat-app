#include <iostream>
#include "User.hpp"

User::User() : m_Username{""}, m_LoggedIn{false}
{
}

User::User(const std::string &username) : m_Username{username}, m_LoggedIn{true}
{
}

std::string User::GetUsername() const noexcept
{
    return m_Username;
}

void User::LogIn(const std::string &username)
{
    m_Username = username;
    m_LoggedIn = true;
}

void User::LogOut()
{
    m_Username = "";
    m_LoggedIn = false;
}

bool User::IsLoggedIn()
{
    return m_LoggedIn;
}
