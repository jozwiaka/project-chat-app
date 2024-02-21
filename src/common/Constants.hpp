#pragma once

#include <string_view>

namespace JsonHeader
{
    constexpr std::string_view ACTION = "action";
    constexpr std::string_view STATUS = "status";

    namespace User
    {
        constexpr std::string_view USER = "user";
        constexpr std::string_view USERS = "users";

        constexpr std::string_view USER_ID = "user_id";
        constexpr std::string_view USERNAME = "username";
        constexpr std::string_view PASSWORD = "password";
        constexpr std::string_view PASSWORD_HASH = "password_hash";
        constexpr std::string_view SALT = "salt";
        constexpr std::string_view CREATED_AT = "created_at";

    }

    namespace Message
    {
        constexpr std::string_view MESSAGE = "message";
        constexpr std::string_view MESSAGES = "messages";

        constexpr std::string_view MESSAGE_ID = "message_id";
        constexpr std::string_view SENDER_ID = "sender_id";
        constexpr std::string_view RECEIVER_ID = "receiver_id";
        constexpr std::string_view CONTENT = "content";
        constexpr std::string_view SENT_AT = "sent_at";

        constexpr std::string_view SENDER_USERNAME = "sender_username";
        constexpr std::string_view RECEIVER_USERNAME = "receiver_username";
    }
}

namespace Action
{
    constexpr std::string_view CREATE_USER = "CREATE_USER";
    constexpr std::string_view VERIFY_USER = "VERIFY_USER";
    constexpr std::string_view LIST_USERS = "LIST_USERS";
    constexpr std::string_view SEND_MESSAGE_TO_USER = "SEND_MESSAGE_TO_USER";
    constexpr std::string_view SHOW_CHAT_HISTORY_WITH_USER = "SHOW_CHAT_HISTORY_WITH_USER";
    constexpr std::string_view TRUNCATE_TABLES = "TRUNCATE_TABLES";
    constexpr std::string_view CHECK_CONNECTION_WITH_SERVER = "CHECK_CONNECTION_WITH_SERVER";
}

namespace Status
{
    constexpr std::string_view SUCCESS = "Success";
    constexpr std::string_view ERROR = "Error";
}