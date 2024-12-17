#include <Logic.hpp>

#include <csetjmp>

#include <spdlog/spdlog.h>

void meetx::MeetXStart(const std::string& token) {
    auto logger = spdlog::get("TgBotBackend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotBackend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    if (token.empty()) {
        std::string tokenBuffer;
        logger->log(spdlog::level::info, "Write token:");
        std::getline(std::cin, tokenBuffer);
        sigsetjmp(signalsUtils::programBuf, 1);
        if (bot)
            logger->log(spdlog::level::warn, "The std::shared_ptr<TgBot::Bot> object already exists, it will be overwritten");
        else
            logger->log(spdlog::level::info, "Created std::shared_ptr<TgBot> object with argument {}", tokenBuffer);
        bot = std::make_shared<TgBot::Bot>(tokenBuffer);
    }
}
