#pragma once

#include <iostream>

class User
{
public:
    User();

    User(const std::string &username);

    std::string GetUsername() const noexcept;

    void LogIn(const std::string &username);

    void LogOut();

    bool IsLoggedIn();

private:
    std::string m_Username;
    bool m_LoggedIn;
};
