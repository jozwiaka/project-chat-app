#pragma once

#include <iostream>
#include "JsonHandler.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <nlohmann/json.hpp>
#include "Config.hpp"
#include "User.hpp"

class Commands
{
public:
    using Json = JsonHandler::Json;

public:
    Commands(int clientSocket, User &user);

    void Execute(const std::string_view cmd);

    std::vector<std::string> GetCommands();

    void ClearDatabase();

    bool CheckConnectionWithServer();

    virtual ~Commands() = default;

protected:
    User &m_User;
    std::map<std::string, std::function<void()>> m_CommandsFunctions;
    int m_ClientSocket;
};

class CommandsNotLoggedIn : public Commands
{
public:
    CommandsNotLoggedIn(int clientSocket, User &user);

    void SignIn(std::string username = "", std::string password = "");

    void LogIn(std::string username = "", std::string password = "");
};
class CommandsLoggedIn : public Commands
{
public:
    CommandsLoggedIn(int clientSocket, User &user);

    void Message(std::string receiver = "", std::string content = "");

    void ShowChat(std::string receiver = "");

    void ListUsers();

    void LogOut();
};

class CommandHandler // Strategy
{
public:
    void SetStrategy(Commands *commands);

    void ExecuteCommand(const std::string_view cmd);

    std::vector<std::string> ListCommands();

private:
    Commands *m_Commands;
};