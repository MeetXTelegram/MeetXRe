#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <tgbot/tgbot.h>

namespace meetx {
    extern std::unordered_map<std::string,
           std::unordered_map<std::string, void(*)(TgBot::CallbackQuery::Ptr,
           std::vector<std::string>)>> actionsList;

    extern std::shared_ptr<TgBot::Bot> bot;
    namespace actions {
        void PreActionHandler(TgBot::CallbackQuery::Ptr query);
    }
}