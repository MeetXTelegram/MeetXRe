#include <Unban.hpp>

#include <string>
#include <vector>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

std::string meetx::runners::Unban(std::vector<std::string> args) {
    if (args.empty()) {
        spdlog::get("TgBotBackend")->log(spdlog::level::err, "Runners::Unban: too few arguments: expected 1, but provided 0");
        return "Unban usage:\n/run [password] unban [chat_id]";
    }
    spdlog::get("TgBotBackend")->log(spdlog::level::warn, "Runners::Unban: in development");
    return "In development";
}
