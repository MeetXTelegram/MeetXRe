#include <PSQLWrapper.hpp>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include <Definitions.hpp>

bool dbInteraction::IsBanned(std::variant<std::int64_t, User> identificator) {
    std::int64_t id;
    if (std::holds_alternative<std::int64_t>(identificator))
        id = std::get<std::int64_t>(identificator);
    else
        id = std::get<User>(identificator).id;

    pqxx::work w(*db::database);
    try {
        pqxx::result result = w.exec("SELECT * FROM banned_users WHERE id = $1;", id);
        if (result.empty())
            return false;
        else
            return true;
    } catch (std::exception& dberr) {
        spdlog::get("PSQL")->log(spdlog::level::err, "Failed to check user: pqxx made an exception: {}", dberr.what());
        return true;
    }
}
