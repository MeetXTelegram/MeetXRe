#include <Logic.hpp>

#include <tgbot/tgbot.h>

#include <PSQLWrapper.hpp>
#include <MLSystem.hpp>

void meetx::handlers::HelpCommandHandler(TgBot::Message::Ptr message) {
    auto replyParameters = std::make_shared<TgBot::ReplyParameters>();
    replyParameters->messageId = message->messageId;
    replyParameters->chatId = message->chat->id;

    auto user = dbInteraction::GetUser(message->from->id);

    bot->getApi().sendMessage(message->chat->id, mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "commands", "command_help"), nullptr, replyParameters);
}
