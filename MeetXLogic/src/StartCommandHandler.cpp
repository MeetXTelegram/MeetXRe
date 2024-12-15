#include <Logic.hpp>

#include <tgbot/tgbot.h>
#include <fmt/base.h>

#include <PSQLWrapper.hpp>
#include <MLSystem.hpp>

void meetx::handlers::StartCommandHandler(TgBot::Message::Ptr message) {
    auto replyParameters = std::make_shared<TgBot::ReplyParameters>();
    replyParameters->messageId = message->messageId;
    replyParameters->chatId = message->chat->id;

    if (dbInteraction::IsBanned(message->from->id)) {
        spdlog::get("TgBotBackend")->log(spdlog::level::trace, "Banned user was attempt to use bot");
        bot->getApi().sendMessage(message->chat->id, "You are banned");
        return;
    }

    if (dbInteraction::IsExists(message->from->id)) {
        auto user = dbInteraction::GetUser(message->from->id);
        bot->getApi().sendMessage(message->chat->id, fmt::vformat(mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "commands", "command_start"), fmt::make_format_args(user.name)), nullptr, replyParameters);
    } else {
        User user;
        user.tgFirstName = message->from->firstName;
        user.tgLastName = message->from->lastName;
        user.username = message->from->username;
        user.name = message->from->firstName;
        user.id = message->from->id;
        dbInteraction::AddUser(user);
        StartCommandHandler(message);
        return;
    }

}
