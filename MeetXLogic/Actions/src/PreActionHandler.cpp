#include <PreActionHandler.hpp>

#include <sstream>

#include <spdlog/spdlog.h>
#include <tgbot/tgbot.h>

void meetx::actions::PreActionHandler(TgBot::CallbackQuery::Ptr query) {
    auto logger = spdlog::get("TgBotFrontend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotFrontend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    logger->log(spdlog::level::warn, "Actions is not implemented yet");
    logger->log(spdlog::level::info, "New callback query received:\n\nchat_id = {}\ncallbackData = {}", query->from->id, query->data);

    if (query->data.empty()) {
        logger->log(spdlog::level::warn, "The callbackData is empty");
        return;
    }

    std::string command, subcommand;
    std::istringstream readStream(query->data.substr(1));
    readStream >> command;
    if (readStream.eof()) {
        logger->log(spdlog::level::warn, "Too few data in callbackData: expected ![command] [subcommand], but provided ![command]");
        return;
    }
    readStream >> subcommand;

    if (actionsList[command][subcommand]) {
        if (readStream.eof())
            actionsList[command][subcommand](query, {});
        else {
            std::vector<std::string> tmpArgsVector;
            std::string tmpArgBuffer;
            while (readStream >> tmpArgBuffer)
                tmpArgsVector.push_back(tmpArgBuffer);
            actionsList[command][subcommand](query, tmpArgsVector);
        }
    } else {
        logger->log(spdlog::level::warn, "Not found actions for command {} and subcommand {}", command, subcommand);
    }
}