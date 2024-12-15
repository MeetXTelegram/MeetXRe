#include <PSQLWrapper.hpp>

#include <variant>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

namespace db { extern std::shared_ptr<pqxx::connection> database; }

void dbInteraction::RemoveUser(std::variant<std::int64_t, User> identificator) {
    std::int64_t id = (std::holds_alternative<std::int64_t>(identificator) ? std::get<std::int64_t>(identificator) : std::get<User>(identificator).id);
    pqxx::work w(*db::database);
    try {
        w.exec("DELETE FROM users WHERE id = $1;", id);
        w.commit();
    } catch (std::exception& dberr) {
        spdlog::get("PSQL")->log(spdlog::level::err, "Failed to remove user: pqxx created an exception: {}", dberr.what());
    return;        }
    spdlog::get("PSQL")->log(spdlog::level::info, "Deleted user with id {}", id);
}
