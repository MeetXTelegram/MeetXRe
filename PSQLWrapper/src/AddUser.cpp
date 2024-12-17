#include <PSQLWrapper.hpp>

#include <csignal>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

namespace db { extern std::shared_ptr<pqxx::connection> database; }

AddUserResult dbInteraction::AddUser(User& user) {
    auto logger = spdlog::get("PSQL");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"PSQL\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    if (!db::database) {
        logger->log(spdlog::level::err, "Failed to add user: db::database is not valid(maybe init::MeetXInit didn't not called?)");
        std::raise(SIGUSR1);
    }

    pqxx::work w(*db::database);
    pqxx::result res = w.exec("SELECT * FROM users WHERE id = $1", user.id);
    if (!res.empty()) {
        logger->log(spdlog::level::warn, "Failed to add user: it looks like the user with id {} already exists (use the updateUserInfo to update the information)", user.id);
        return AddUserResult::ALREADY_EXISTS;
    } else {
        try {
            w.exec("INSERT INTO users "
                   "("
                   "id,"
                   "tgFirstName,"
                   "tgLastName,"
                   "username,"
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
                   "','" +
                   w.esc((user.username.empty() ? "" : user.username)) +
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
                   w.quote(user.hasProfile) +
                   ");");
            w.commit();
        } catch (std::exception& dberr) {
            logger->log(spdlog::level::err, "Failed to add user: pqxx created an exception: {}", dberr.what());
            return AddUserResult::DB_ERROR;
        }
        logger->log(spdlog::level::info, "Added a new user: id = {}", user.id);
        return AddUserResult::OK;
    }
}
