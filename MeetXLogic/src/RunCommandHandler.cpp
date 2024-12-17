#include <Logic.hpp>

#include <sstream>
#include <functional>

#include <spdlog/spdlog.h>

#include <Definitions.hpp>

void meetx::handlers::RunCommandHandler(TgBot::Message::Ptr message) {
    auto logger = spdlog::get("TgBotBackend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotBackend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    auto replyParameters = std::make_shared<TgBot::ReplyParameters>();
    replyParameters->messageId = message->messageId;
    replyParameters->chatId = message->chat->id;

    std::string dataBuffer;
    std::istringstream msgStream(message->text);
    msgStream >> dataBuffer;
    if (msgStream.eof()) {
        logger->log(spdlog::level::warn, "Calling /run without command and password");
        bot->getApi().sendMessage(message->chat->id, "You need to provide a password and a command: \n/run [password] [command]", nullptr, replyParameters);
        return;
    } else {
        msgStream >> dataBuffer;
        std::hash<std::string> passwordHasher;
        if (db::adminModePasswordHash == passwordHasher(dataBuffer)) {
            if (msgStream.eof()) {
                logger->log(spdlog::level::warn, "Attempting to execute the admin command, but the command is empty");
                return;
            } else {
                std::string command;
                msgStream >> command;
                logger->log(spdlog::level::warn, "Attempting to execute the admin command: \"{}\"", command);

                if (!runnersList.count(command)) {
                    logger->log(spdlog::level::warn, "Execute failed: command \"{}\" does not exists", command);
                    return;
                } else {
                    if (msgStream.eof())
                        bot->getApi().sendMessage(message->chat->id, runnersList.at(command)({}), nullptr, replyParameters);
                    else {
                        std::vector<std::string> args;
                        std::string argBuffer;
                        while (msgStream >> argBuffer)
                            args.emplace_back(argBuffer);
                        bot->getApi().sendMessage(message->chat->id, runnersList.at(command)(args), nullptr, replyParameters);
                    }
                }
            }
        } else {
            logger->log(spdlog::level::warn, "Attempting to execute the admin command, but the hash does not match");
            bot->getApi().sendMessage(message->chat->id, "Authentification error(incorrect password)", nullptr, replyParameters);
        }
    }
}
