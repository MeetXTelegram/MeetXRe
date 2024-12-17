#pragma once

#include <tgbot/tgbot.h>

namespace meetx {
    namespace actions {
        void PreActionHandler(TgBot::CallbackQuery::Ptr query);
    }
}