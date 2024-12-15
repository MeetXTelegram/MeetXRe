#include <PSQLWrapper.hpp>

#include <cstdint>

#include <pqxx/pqxx>

#include <Definitions.hpp>

bool dbInteraction::IsExists(std::variant<std::int64_t, User> identificator) {
    std::int64_t id;
    if (std::holds_alternative<std::int64_t>(identificator))
        id = std::get<std::int64_t>(identificator);
    else
        id = std::get<User>(identificator).id;

    pqxx::work w(*db::database);
    try {
        pqxx::result result = w.exec("SELECT * FROM users WHERE id = $1;", id);
        if (result.empty())
            return false;
        else
            return true;
    } catch (std::exception& dberr) {
        spdlog::get("PSQLW")->log(spdlog::level::err, "Failed to check user: pqxx made an exception: {}", dberr.what());
        return false;
    }
}
