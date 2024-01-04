#include "Server.hpp"
#include <thread>
#include <chrono>

int main()
{
    Server &server = Server::GetInstance();
    server.Start();
    server.Wait();

    return 0;
}
