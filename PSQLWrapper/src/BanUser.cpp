#include <PSQLWrapper.hpp>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include <Definitions.hpp>

void dbInteraction::BanUser(std::variant<std::int64_t, User> identificator) {
    std::int64_t id;
    if (std::holds_alternative<std::int64_t>(identificator))
        id = std::get<std::int64_t>(identificator);
    else
        id = std::get<User>(identificator).id;

    if (!IsExists(id)) {
        spdlog::get("PSQL")->log(spdlog::level::err, "Failed to ban user: the user with id {} does not exists", id);
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
        spdlog::get("PSQL")->log(spdlog::level::err, "Failed to ban user: pqxx made an exception: {}", dberr.what());
        return;
    }

    spdlog::get("PSQL")->log(spdlog::level::info, "Banned user: id = {}", id);
}
