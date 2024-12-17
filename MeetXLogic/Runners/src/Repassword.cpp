#include <Repassword.hpp>

#include <string>
#include <functional>
#include <vector>

#include <spdlog/spdlog.h>

#include <Definitions.hpp>

std::string meetx::runners::Repassword(std::vector<std::string> args) {
    auto logger = spdlog::get("TgBotBackend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotBackend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }
    if (args.empty()) {
        logger->log(spdlog::level::err, "Runners::Repassword: too few arguments: expected 1, but provided 0");
        return "Repassword usage:\n/run [password] repassword [new password]";
    }

    std::hash<std::string> passwordHasher;
    db::adminModePasswordHash = passwordHasher(args[0]);
    logger->log(spdlog::level::warn, "Runners::Repassword: password has been changed");
    return "The password has been changed succefully";
}
