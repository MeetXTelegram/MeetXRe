#pragma once

#include <string>
#include <vector>

#include <tgbot/tgbot.h>

namespace meetx {
    extern std::shared_ptr<TgBot::Bot> bot;
    namespace actions {
        void ShowProfile(TgBot::CallbackQuery::Ptr query, std::vector<std::string> args);
    }
}