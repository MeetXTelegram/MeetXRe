#include <Show.hpp>

#include <string>
#include <vector>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include <Definitions.hpp>

std::string meetx::runners::Show(std::vector<std::string> args) {
    auto logger = spdlog::get("TgBotBackend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotBackend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    if (args.empty()) {
        logger->log(spdlog::level::err, "Runners::Show: too few arguments: expected 1, but provided 0");
        return "Show usage:\n/run [password] show [chat_id]";
    }

    pqxx::work w(*db::database);
    try {
        pqxx::result usersResult = w.exec("SELECT * FROM users WHERE id = $1", std::stoll(args[0]));
        pqxx::result bannedUsersResult = w.exec("SELECT * FROM banned_users WHERE id = $1", std::stoll(args[0]));

        if (usersResult.empty()) {
            logger->log(spdlog::level::err, "Runners::Show: failed to show user with id {}: it does not exists in database", args[0]);
            return "User does not exists in database";
        }

        User userInfo;
        for (auto element : usersResult) {
            userInfo.tgFirstName = element["tgFirstName"].as<std::string>();
            if (!element["tgLastName"].is_null())
                userInfo.tgLastName = element["tgLastName"].as<std::string>();
            userInfo.age = element["age"].as<std::uint16_t>();
            if (!element["username"].is_null())
                userInfo.username = element["username"].as<std::string>();
            /* Soon */
        }

        auto isBanned = !bannedUsersResult.empty();
        if (std::find(args.begin(), args.end(), "-g") != args.end() || std::find(args.begin(), args.end(), "--global") != args.end()) {
            logger->log(spdlog::level::warn, "Runners::Show: attempting to show user with id {} with parameter \"--global\"", args[0]);
            auto chatFullInfo = bot->getApi().getChat(args[0]);
            return fmt::vformat("User {}:\nfirst_name: {}\nlast_name: {}\nusername: {}\nbio: {}\nbanned: {}", fmt::make_format_args(args[0], chatFullInfo->firstName, chatFullInfo->lastName, chatFullInfo->username, chatFullInfo->bio, isBanned));
        } else {
            logger->log(spdlog::level::warn, "Runners::Show: attempting to show user with id {} with parameter \"--global\"", args[0]);
        }

    } catch (std::exception& dberr) /* pqxx error */ {
        logger->log(spdlog::level::err, "Runners::Show: failed to show user: pqxx made an exception: {} ", dberr.what());
        return "Database error(see logs)";
    } catch (std::invalid_argument& casterr) /* std::stoll error */ {
        logger->log(spdlog::level::err, "Runners::Show: cast error(maybe args[0] is not a number?)");
        return "Please provide a NUMBER, NOT A STRING";

    }
}