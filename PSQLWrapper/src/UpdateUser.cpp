#include <PSQLWrapper.hpp>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

namespace db { extern std::shared_ptr<pqxx::connection> database; }

void dbInteraction::UpdateUser(User& user) {
    pqxx::work w(*db::database);
    try {
        pqxx::result checkUser = w.exec("SELECT * FROM users WHERE id = $1;", user.id);
        if (checkUser.empty()) {
            spdlog::get("PSQL")->log(spdlog::level::warn, "Failed to update user: user with id {} was not found (use the addUser to add user)", user.id);
            return;
        }

        w.exec("UPDATE users "
               "SET tgFirstName = " + w.esc(user.tgFirstName) +
               ", tgLastName = " + w.esc(user.tgLastName) +
               ", age = " + std::to_string(user.age) +
               ", name = " + w.esc(user.name) +
               ", bio = " + w.esc(user.bio) +
               " " +
               "WHERE id = " + std::to_string(user.id) + ";");

        w.commit();
    } catch (std::exception& dberr) {
        spdlog::get("PSQL")->log(spdlog::level::err, "Failed to update user: pqxx created an exception: {}", dberr.what());
    }
}
