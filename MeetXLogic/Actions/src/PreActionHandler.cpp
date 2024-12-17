#include <PreActionHandler.hpp>

#include <spdlog/spdlog.h>
#include <tgbot/tgbot.h>

namespace meetx {
     extern std::shared_ptr<TgBot::Bot> bot;
}

void meetx::actions::PreActionHandler(TgBot::CallbackQuery::Ptr query) {
    auto logger = spdlog::get("TgBotFrontend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotFrontend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    logger->log(spdlog::level::warn, "Actions is not implemented yet");
    logger->log(spdlog::level::info, "New callback query received:\n\nchat_id = {}\ncallbackData = {}", query->from->id, query->data);
}