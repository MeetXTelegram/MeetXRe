#include <Ban.hpp>

#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include <PSQLWrapper.hpp>

std::string meetx::runners::Ban(std::vector<std::string> args) {
    auto logger = spdlog::get("TgBotBackend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotBackend\" logger is not registered, correct communication is not possible");
    }

    if (args.empty()) {
        logger->log(spdlog::level::err, "Runners::Ban: too few arguments: expected 1, but provided 0");
        return "Ban usage: \n/run [password] ban [chat_id]";
    }

    try {
        if (!dbInteraction::IsExists(std::stoll(args[0]))) {
            logger->log(spdlog::level::err, "Runners::Ban: attempting to ban user with id {}, but it does not exists in database", args[0]);
            return "User does not exist in database";
        } else {
            dbInteraction::BanUser(std::stoll(args[0]));
            return "OK";
        }
    } catch (std::invalid_argument& casterr) {
        logger->log(spdlog::level::err, "Runners::Ban: cast error(maybe args[0] is not a number?)");
        return "Please provide a NUMBER, NOT A STRING";
    }
}