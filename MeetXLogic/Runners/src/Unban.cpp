#include <Unban.hpp>

#include <string>
#include <vector>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

#include "PSQLWrapper.hpp"

std::string meetx::runners::Unban(std::vector<std::string> args) {
    auto logger = spdlog::get("TgBotBackend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotBackend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }
    if (args.empty()) {
        logger->log(spdlog::level::err, "Runners::Unban: too few arguments: expected 1, but provided 0");
        return "Unban usage:\n/run [password] unban [chat_id]";
    }

    try {
        dbInteraction::UnbanUser(std::stoll(args[0]));
    } catch (std::invalid_argument& casterr) {
        logger->log(spdlog::level::err, "Runners::Unban: cast error(maybe args[0] is not a number?)");
        return "Please provide a NUMBER, NOT A STRING";
    }
    return "OK";
}
