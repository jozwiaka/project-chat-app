#pragma once
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pqxx/pqxx>
#include "ChatDatabase.hpp"
#include "Config.hpp"
#include "JsonHandler.hpp"
#include <mutex>
#include <condition_variable>

class ClientHandler : public std::enable_shared_from_this<ClientHandler>
{
public:
    using Json = typename JsonHandler::Json;
    using ActionHandler = std::function<void(ClientHandler &, Json &request, Json &response)>;

public:
    explicit ClientHandler(int clientSocket, ChatDatabase &database);

    ~ClientHandler();

    void Start();

    void Stop();

private:
    void RegisterActionHandler(const std::string &action, ActionHandler handler);

    void HandleAction();

    void HandleCreateUser(const Json &request, Json &response);

    void HandleVerifyUser(const Json &request, Json &response);

    void HandleListUsers(const Json &request, Json &response);

    void HandleSendMessageToUser(const Json &request, Json &response);

    void HandleShowChatHistoryWithUser(const Json &request, Json &response);

    void HandleTruncateTables(const Json &request, Json &response);

    void HandleCheckConnectionWithServer(const Json &request, Json &response);

private:
    int m_ClientSocket;
    std::unordered_map<std::string, ActionHandler> m_ActionHandlers;
    ChatDatabase &m_Database;
    bool m_Stop;
};
