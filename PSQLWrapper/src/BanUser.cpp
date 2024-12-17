#include <PSQLWrapper.hpp>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include <Definitions.hpp>

void dbInteraction::BanUser(std::variant<std::int64_t, User> identificator) {
    auto logger = spdlog::get("PSQL");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"PSQL\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    std::int64_t id = (std::holds_alternative<std::int64_t>(identificator) ? std::get<std::int64_t>(identificator) : std::get<User>(identificator).id);

    if (!IsExists(id)) {
        logger->log(spdlog::level::err, "Failed to ban user: the user with id {} does not exists", id);
        return;
    }

    pqxx::work w(*db::database);
    try {
        w.exec("INSERT INTO banned_users (id, reason) VALUES (" +
                std::to_string(id) +
                "," +
                "'NONE'"
                ");");
        w.commit();
    } catch (std::exception& dberr) {
        logger->log(spdlog::level::err, "Failed to ban user: pqxx made an exception: {}", dberr.what());
        return;
    }

    logger->log(spdlog::level::info, "Banned user: id = {}", id);
}
