#include <PSQLWrapper.hpp>

#include <csignal>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

namespace db { extern std::shared_ptr<pqxx::connection> database; }

AddUserResult dbInteraction::AddUser(User& user) {
    if (!db::database) {
        spdlog::get("PSQL")->log(spdlog::level::err, "Failed to add user: db::database is not valid(maybe init::MeetXInit didn't not called?)");
        std::raise(SIGUSR1);
    }

    pqxx::work w(*db::database);
    pqxx::result res = w.exec("SELECT * FROM users WHERE id = $1", user.id);
    if (!res.empty()) {
        spdlog::get("PSQL")->log(spdlog::level::warn, "Failed to add user: it looks like the user with id {} already exists (use the updateUserInfo to update the information)", user.id);
        return AddUserResult::ALREADY_EXISTS;
    } else {
        try {
            w.exec("INSERT INTO users "
                   "("
                   "id,"
                   "tgFirstName,"
                   "tgLastName,"
                   "age,"
                   "name,"
                   "bio,"
                   "gender,"
                   "language,"
                   "city,"
                   "media,"
                   "preferredLanguages,"
                   "preferredAges,"
                   "preferredCities,"
                   "preferredGenders,"
                   "hasProfile"
                   ")"
                   "VALUES "
                   "(" +
                   std::to_string(user.id) +
                   ",'" +
                   w.esc(user.tgFirstName) +
                   "','" +
                   w.esc(user.tgLastName) +
                   "'," +
                   std::to_string(user.age) +
                   ",'" +
                   w.esc(user.name) +
                   "','" +
                   w.esc(user.bio) +
                   "'," +
                   std::to_string(static_cast<int>(user.gender)) +
                   "," +
                   std::to_string(static_cast<int>(user.language)) +
                   ",'" +
                   w.esc(user.city) +
                   "'," +
                   w.esc(parsers::cast<std::string, false, std::string>(user.media)) +
                   "," +
                   parsers::cast<UserLanguage, true, int>(user.preferredLanguages) +
                   "," +
                   w.esc(parsers::cast<std::uint16_t, false, std::uint16_t>(user.preferredAges)) +
                   "," +
                   w.esc(parsers::cast<std::string, false, std::string>(user.preferredCities)) +
                   "," +
                   w.esc(parsers::cast<UserGender, true, int>(user.preferredGenders)) +
                   "," +
                   w.quote(true) +
                   ");");
            w.commit();
        } catch (std::exception& dberr) {
            spdlog::get("PSQL")->log(spdlog::level::err, "Failed to add user: pqxx created an exception: {}", dberr.what());
            return AddUserResult::DB_ERROR;
        }
        spdlog::get("PSQL")->log(spdlog::level::info, "Added a new user: id = {}", user.id);
        return AddUserResult::OK;
    }
}
