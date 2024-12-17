#pragma once

#include <string>
#include <vector>

#include <pqxx/pqxx>
#include <spdlog/spdlog.h>
#include <tgbot/tgbot.h>

namespace meetx {
    extern std::shared_ptr<TgBot::Bot> bot;
    namespace runners {
        std::string Show(std::vector<std::string> args);
    }
}