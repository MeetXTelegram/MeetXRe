#pragma once

#include <cstdint>
#include <csignal>
#include <variant>

#include <nlohmann/json.hpp>
#include <pqxx/pqxx>

#include <Definitions.hpp>

enum AddUserResult {
    OK, /* The user succefully added */
    ALREADY_EXISTS, /* The user already exists */
    BANNED, /* The user has been banned */
    DB_ERROR, /* An error occurred on the database side */
};

namespace dbInteraction {
    AddUserResult AddUser(User& user);
    void RemoveUser(std::variant<std::int64_t, User> identificator);
    User GetUser(std::variant<std::int64_t, User> identificator);
    void UpdateUser(User& user);
    bool IsExists(std::variant<std::int64_t, User> identificator);
    bool IsBanned(std::variant<std::int64_t, User> identificator);
    void BanUser(std::variant<std::int64_t, User> identificator);
}

namespace parsers {
    template <typename ValueType, bool useCast, typename CastType> std::vector<ValueType> parse(const std::string& json) {
        std::vector<ValueType> result;
        if (json.empty())
            return result;

        auto j = nlohmann::json::parse(json);
        for (auto& element : j)
            if (useCast)
                result.emplace_back(static_cast<CastType>(element.get<ValueType>()));
            else
                result.emplace_back(element.get<ValueType>());

        return result;
    }

    template <typename ValueType, bool useCast, typename CastType> std::string cast(std::vector<ValueType>& v) {
        nlohmann::json j;
        for (auto& element : v) {
            if (useCast)
                j.emplace_back(static_cast<CastType>(element));
            else
                j.emplace_back(element);
        }
        return j.dump();
    }
}
