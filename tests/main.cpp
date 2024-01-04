#include <iostream>
#include <thread>
#include "Server.hpp"
#include "CLI.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include "ChatDatabase.hpp"
#include <openssl/opensslv.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <string>
#include <string_view>
#include "Commands.hpp"
#include "Client.hpp"

TEST(ServerTest, Restart)
{
    ASSERT_NO_THROW(
        {
            Server &server = Server::GetInstance();
            server.Start();
            server.Restart();
            server.Stop();
        });
}

TEST(ServerTest, RestartWithClients)
{
    ASSERT_NO_THROW(
        {
            Server &server = Server::GetInstance();
            server.Start();
            Client client;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            client.Stop();
            server.Restart();
            server.Stop();
        });
}

TEST(OpenSSLTest, Version)
{
    EXPECT_TRUE([]
                {
        std::string_view opensslVersion = SSLeay_version(SSLEAY_VERSION);
        return opensslVersion.find("3.0.12") != std::string::npos; });
}

TEST(ChatDatabaseTest, Connect)
{
    ASSERT_NO_THROW(
        {
            ChatDatabase::GetInstance();
        });
}

TEST(MultipleClientsTest, SendMessages)
{
    ASSERT_NO_THROW(
        {
            Server &server = Server::GetInstance();
            server.Start();

            Client client1;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Client client2;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            User user1;
            User user2;

            CommandsNotLoggedIn commandsNotLoggedIn1 = CommandsNotLoggedIn(client1.GetClientSocket(), user1);
            CommandsLoggedIn commandsLoggedIn1 = CommandsLoggedIn(client1.GetClientSocket(), user1);

            CommandsNotLoggedIn commandsNotLoggedIn2 = CommandsNotLoggedIn(client2.GetClientSocket(), user2);
            CommandsLoggedIn commandsLoggedIn2 = CommandsLoggedIn(client2.GetClientSocket(), user2);

            std::string username1 = "User_1";
            std::string username2 = "User_2";
            std::string password = "password";

            auto formatMessage([](const std::string &sender, const std::string &receiver)
                               { return "Message from " + sender + " to " + receiver; });

            std::cout << "Clear database" << std::endl;
            commandsNotLoggedIn1.ClearDatabase();

            commandsNotLoggedIn1.SignIn(username1, password);
            commandsNotLoggedIn2.SignIn(username2, password);

            std::cout << "Log in as " + username1 << std::endl;
            commandsNotLoggedIn1.LogIn(username1, password);
            commandsNotLoggedIn2.LogIn(username2, password);

            commandsLoggedIn1.ListUsers();
            commandsLoggedIn1.Message(username2, formatMessage(username1, username2));
            commandsLoggedIn2.Message(username1, formatMessage(username2, username1));

            std::cout << "Chat with " + username2 << ":" << std::endl;
            commandsLoggedIn1.ShowChat(username2);

            std::cout << "Log out" << std::endl;
            commandsLoggedIn1.LogOut();
            commandsLoggedIn2.LogOut();

            std::cout << "Clear database" << std::endl;
            commandsLoggedIn1.ClearDatabase();
            client1.Stop();
            client2.Stop();

            server.Stop();
        });
}

TEST(SingleClientTest, SendMessages)
{
    ASSERT_NO_THROW(
        {
            Server &server = Server::GetInstance();
            server.Start();

            Client client;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            User user;

            CommandsNotLoggedIn commandsNotLoggedIn = CommandsNotLoggedIn(client.GetClientSocket(), user);
            CommandsLoggedIn commandsLoggedIn = CommandsLoggedIn(client.GetClientSocket(), user);

            std::string username1 = "User_1";
            std::string username2 = "User_2";
            std::string password = "password";

            auto formatMessage([](const std::string &sender, const std::string &receiver)
                               { return "Message from " + sender + " to " + receiver; });

            std::cout << "Clear database" << std::endl;
            commandsNotLoggedIn.ClearDatabase();

            commandsNotLoggedIn.SignIn(username1, password);
            commandsNotLoggedIn.SignIn(username2, password);

            std::cout << "Log in as " + username1 << std::endl;
            commandsNotLoggedIn.LogIn(username1, password);

            commandsLoggedIn.ListUsers();
            commandsLoggedIn.Message(username1, formatMessage(username1, username1));
            commandsLoggedIn.Message(username2, formatMessage(username1, username2));
            std::cout << "Log out" << std::endl;
            commandsLoggedIn.LogOut();

            std::cout << "Log in as " + username2 << std::endl;
            commandsNotLoggedIn.LogIn(username2, password);
            commandsLoggedIn.Message(username2, formatMessage(username2, username2));
            commandsLoggedIn.Message(username1, formatMessage(username2, username1));
            std::cout << "Chat with " + username2 << ":" << std::endl;
            commandsLoggedIn.ShowChat(username2);
            std::cout << "Chat with " + username1 << ":" << std::endl;
            commandsLoggedIn.ShowChat(username1);
            std::cout << "Log out" << std::endl;
            commandsLoggedIn.LogOut();

            std::cout << "Clear database" << std::endl;
            commandsLoggedIn.ClearDatabase();

            client.Stop();
            server.Stop();
        });
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
