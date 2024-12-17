#include <PSQLWrapper.hpp>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include <Definitions.hpp>

void dbInteraction::UnbanUser(std::variant<std::int64_t, User> identificator) {
    auto logger = spdlog::get("PSQL");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"PSQL\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    std::int64_t id = (std::holds_alternative<std::int64_t>(identificator) ? std::get<std::int64_t>(identificator) : std::get<User>(identificator).id);

    pqxx::work w(*db::database);
    try {
        pqxx::result result = w.exec("SELECT * FROM banned_users WHERE id = $1", id);
        if (result.empty()) {
            logger->log(spdlog::level::warn, "Failed to unban user: not found in banned_users");
            return;
        } else {
            w.exec("DELETE FROM banned_users WHERE id = $1", id);
            w.commit();
            logger->log(spdlog::level::info, "The user with id {} was succefully unbanned", id);
        }
    } catch (std::exception& dberr) {
        logger->log(spdlog::level::err, "Failed to unban user: pqxx made an exception: {}", dberr.what());
    }
}
