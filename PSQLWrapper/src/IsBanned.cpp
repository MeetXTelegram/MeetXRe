#include <PSQLWrapper.hpp>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include <Definitions.hpp>

bool dbInteraction::IsBanned(std::variant<std::int64_t, User> identificator) {
    auto logger = spdlog::get("PSQL");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"PSQL\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    std::int64_t id = (std::holds_alternative<std::int64_t>(identificator) ? std::get<std::int64_t>(identificator) : std::get<User>(identificator).id);

    pqxx::work w(*db::database);
    try {
        pqxx::result result = w.exec("SELECT * FROM banned_users WHERE id = $1;", id);
        if (result.empty())
            return false;
        else
            return true;
    } catch (std::exception& dberr) {
        logger->log(spdlog::level::err, "Failed to check user: pqxx made an exception: {}", dberr.what());
        return true;
    }
}
