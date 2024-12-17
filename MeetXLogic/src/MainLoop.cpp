#include <Logic.hpp>

#include <csetjmp>
#include <csignal>

#include <spdlog/spdlog.h>

#include <PreActionHandler.hpp>

void meetx::MainLoop() {
    auto logger = spdlog::get("TgBotBackend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotBackend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    if (!bot) {
        logger->log(spdlog::level::err, "Something went wrong: MainLoop cannot be called without first creating a std::shared_ptr<TgBot> object");
        std::raise(SIGUSR1);
    }

    sigsetjmp(signalsUtils::programBuf, 1);
    auto botInfo = bot->getApi().getMe();
    logger->log(spdlog::level::info, "Bot info:\n\tusername: {}\n\tfirst_name: {}", botInfo->username, botInfo->firstName);

    bot->getEvents().onCallbackQuery(actions::PreActionHandler);
    bot->getEvents().onCommand("help", handlers::HelpCommandHandler);
    bot->getEvents().onCommand("start", handlers::StartCommandHandler);
    bot->getEvents().onCommand("run", handlers::RunCommandHandler);
    bot->getEvents().onAnyMessage([logger](TgBot::Message::Ptr message) {
        logger->log(spdlog::level::info, "Message received:\n\nchat_id = {}\ntext = {}", message->from->id, message->text);
    });

    try {
        TgBot::TgLongPoll lpoll(*bot);
        while (true) {
            lpoll.start();
        }
    } catch (TgBot::TgException& apierr) {
        logger->log(spdlog::level::err, "The bot spawned an exception: {}", apierr.what());
        std::raise(SIGUSR1);
    }
}
