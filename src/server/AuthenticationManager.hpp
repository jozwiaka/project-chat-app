#pragma once
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <array>
#include <tuple>

class AuthenticationManager
{
public:
    // returns {password, salt}
    static std::tuple<std::string, std::string> HashPassword(const std::string &password);

    static bool VerifyPassword(const std::string &password, const std::string &storedPassword, const std::string &salt);

private:
    static std::string GenerateSalt();

    static std::string HashPasswordWithSalt(const std::string &password, const std::string &salt);
};