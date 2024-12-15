#include <Logic.hpp>

#include <csetjmp>

#include <spdlog/spdlog.h>

void meetx::MeetXStart(const std::string& token) {
    if (token.empty()) {
        std::string tokenBuffer;
        spdlog::get("TgBotBackend")->log(spdlog::level::info, "Write token:");
        std::getline(std::cin, tokenBuffer);
        setjmp(signalsUtils::programBuf);
        if (bot)
            spdlog::get("TgBotBackend")->log(spdlog::level::warn, "The std::shared_ptr<TgBot::Bot> object already exists, it will be overwritten");
        else
            spdlog::get("TgBotBackend")->log(spdlog::level::info, "Created std::shared_ptr<TgBot> object with argument {}", tokenBuffer);
        bot = std::make_shared<TgBot::Bot>(tokenBuffer);
    }
}
