#include <Logic.hpp>

#include <tgbot/tgbot.h>
#include <fmt/base.h>

#include <PSQLWrapper.hpp>
#include <MLSystem.hpp>

void meetx::handlers::StartCommandHandler(TgBot::Message::Ptr message) {
    auto logger = spdlog::get("TgBotBackend");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"TgBotBackend\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    auto replyParameters = std::make_shared<TgBot::ReplyParameters>();
    replyParameters->messageId = message->messageId;
    replyParameters->chatId = message->chat->id;

    if (dbInteraction::IsBanned(message->from->id)) {
        logger->log(spdlog::level::trace, "Banned user was attempt to use bot");
        bot->getApi().sendMessage(message->chat->id, "You are banned");
        return;
    }

    if (dbInteraction::IsExists(message->from->id)) {
        auto user = dbInteraction::GetUser(message->from->id);

        auto buttonMyProfile = std::make_shared<TgBot::InlineKeyboardButton>();
        buttonMyProfile->text = mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "buttons", "button_my_profile");
        buttonMyProfile->callbackData = fmt::vformat("!show profile {}", fmt::make_format_args(user.id));

        auto buttonCreateProfile = std::make_shared<TgBot::InlineKeyboardButton>();
        buttonCreateProfile->text = mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "buttons", "button_create_profile");
        buttonCreateProfile->callbackData = "!create profile";

        auto buttonChangeLanguage = std::make_shared<TgBot::InlineKeyboardButton>();
        buttonChangeLanguage->text = mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "buttons", "button_change_language");
        buttonChangeLanguage->callbackData = "!show menu changelanguage";

        auto keyboard = std::make_shared<TgBot::InlineKeyboardMarkup>();
        keyboard->inlineKeyboard.push_back({{buttonMyProfile, buttonCreateProfile, buttonChangeLanguage}});

        bot->getApi().sendMessage(message->chat->id, fmt::vformat(mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "commands", "command_start"), fmt::make_format_args(user.name)), nullptr, replyParameters, keyboard);
        return;
    } else {
        User user;
        user.tgFirstName = message->from->firstName;
        user.tgLastName = message->from->lastName;
        user.username = message->from->username;
        user.name = message->from->firstName;
        user.age = 0;
        user.id = message->from->id;
        switch (dbInteraction::AddUser(user)) {
            case AddUserResult::DB_ERROR: {
                bot->getApi().sendMessage(message->chat->id, mlsys::getReplic(mlsys::castLanguageToFilename(), "errors", "add_user_db_error"), nullptr, replyParameters);
                return;
            }

            case AddUserResult::OK: {
                StartCommandHandler(message);
                return;
            }

            case AddUserResult::BANNED: {
                logger->log(spdlog::level::warn, "Something strange: the user is banned, but AddUser was called\n\nid = {}\nfirst_name = {}\nusername = {}", message->from->id, message->from->firstName, message->from->username);
                return;
            }

            case AddUserResult::ALREADY_EXISTS: {
                logger->log(spdlog::level::warn, "Something strange: the user already exists, but AddUser was called\n\nid = {}\nfirst_name = {}\nusername = {}", message->from->id, message->from->firstName, message->from->username);
                return;
            }
        };
    }

}
