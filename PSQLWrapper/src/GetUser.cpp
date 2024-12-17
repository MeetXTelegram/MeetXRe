#include <PSQLWrapper.hpp>

#include <variant>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

namespace db { extern std::shared_ptr<pqxx::connection> database; }

User dbInteraction::GetUser(std::variant<std::int64_t, User> identificator) {
    auto logger = spdlog::get("PSQL");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"PSQL\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    User result;
    std::int64_t id = (std::holds_alternative<std::int64_t>(identificator) ? std::get<std::int64_t>(identificator) : std::get<User>(identificator).id);
    pqxx::work w(*db::database);
    try {
        pqxx::result dbresult = w.exec("SELECT * FROM users WHERE id = " + std::to_string(id));
        for (auto i : dbresult) {
            result.tgFirstName = i["tgFirstName"].as<std::string>();
            result.tgLastName = i["tgLastName"].as<std::string>();
            if (!i["age"].is_null())
                result.age = i["age"].as<std::uint16_t>();
            result.name = i["name"].as<std::string>();
            if (!i["bio"].is_null())
                result.bio = i["bio"].as<std::string>();
            result.gender = static_cast<UserGender>(i["gender"].as<int>());
            result.language = static_cast<UserLanguage>(i["language"].as<int>());
            if (!i["city"].is_null())
                result.city = i["city"].as<std::string>();
            if (!i["media"].is_null())
                result.media = parsers::parse<std::string, false, std::string>(i["media"].as<std::string>());
            if (!i["preferredLanguages"].is_null())
                result.preferredLanguages = parsers::parse<UserLanguage, true, UserLanguage>(i["preferredLanguage"].as<std::string>());
            if (!i["preferredAges"].is_null())
                result.preferredAges = parsers::parse<std::uint16_t, false, std::uint16_t>(i["preferredAges"].as<std::string>());
            if (!i["preferredCities"].is_null())
                result.preferredCities = parsers::parse<std::string, false, std::string>(i["preferredCities"].as<std::string>());
            if (!i["preferredGenders"].is_null())
                result.preferredGenders = parsers::parse<UserGender, false, UserGender>(i["preferredGenders"].as<std::string>());
            result.hasProfile = i["hasProfile"].as<bool>();
        }
    } catch (std::exception& dberr) {
        logger->log(spdlog::level::err, "Failed to get user: pqxx created an exception: {}", dberr.what());
        return result;
    }

    return result;
}
