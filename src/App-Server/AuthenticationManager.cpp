#include <openssl/sha.h>
#include <openssl/rand.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <array>
#include <tuple>
#include "AuthenticationManager.hpp"

std::tuple<std::string, std::string> AuthenticationManager::HashPassword(const std::string &password)
{
    std::string salt = GenerateSalt();
    return {HashPasswordWithSalt(password, salt), salt};
}

bool AuthenticationManager::VerifyPassword(const std::string &password, const std::string &storedPassword, const std::string &salt)
{
    std::string hashedInputPassword = HashPasswordWithSalt(password, salt);
    return (hashedInputPassword == storedPassword);
}

std::string AuthenticationManager::GenerateSalt()
{
    std::array<unsigned char, 16> salt;
    RAND_bytes(salt.data(), salt.size());

    std::stringstream ss;
    for (unsigned int i = 0; i < salt.size(); ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(salt[i]);
    }

    return ss.str();
}

std::string AuthenticationManager::HashPasswordWithSalt(const std::string &password, const std::string &salt)
{
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(mdctx, reinterpret_cast<const unsigned char *>(salt.c_str()), salt.length());
    EVP_DigestUpdate(mdctx, password.c_str(), password.length());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    // Convert the binary hash to a hex string
    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];

    return ss.str();
}