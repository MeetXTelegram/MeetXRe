#include <Repassword.hpp>

#include <string>
#include <functional>
#include <vector>

#include <spdlog/spdlog.h>

#include <Definitions.hpp>

std::string meetx::runners::Repassword(std::vector<std::string> args) {
    if (args.empty()) {
        spdlog::get("TgBotBackend")->log(spdlog::level::err, "Runner::Repassword: too few arguments: expected 2, but provided 0");
        return "Repassword usage:\n/run [password] repassword [new password]";
    }

    std::hash<std::string> passwordHasher;
    db::adminModePasswordHash = passwordHasher(args[0]);
    spdlog::get("TgBotBackend")->log(spdlog::level::warn, "Runner::Repassword: password has been changed");
    return "The password has been changed succefully";
}
