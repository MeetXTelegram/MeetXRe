#include <ShowProfile.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>
#include <tgbot/tgbot.h>

#include <PSQLWrapper.hpp>
#include <MLSystem.hpp>

void meetx::actions::ShowProfile(TgBot::CallbackQuery::Ptr query, std::vector<std::string> args)
{
    auto logger = spdlog::get("TgBotFrontend");
    if (!logger) {
        std::exit(-1);
    }

    bot->getApi().deleteMessage(query->message->chat->id, query->message->messageId);

    if (args.empty()) {
        logger->log(spdlog::level::warn, "Actions::ShowProfile: too few arguments: expected 1, but provided 0");
        bot->getApi().sendMessage(query->message->chat->id,
            "In Actions::ShowProfile: too few arguments\n\nIF YOU ARE NOT A DEVELOPER, JUST TELL HIM ABOUT IT.\nThanks");
        return;
    }

    User user;
    try {
        user = dbInteraction::GetUser(std::stoll(args[0]));
    } catch (std::invalid_argument& casterr) {
        logger->log(spdlog::level::err, "Actions::ShowProfile: cast error(maybe args[0] is not number?)");
        bot->getApi().sendMessage(query->message->chat->id,
                     "In Actions::ShowProfile: cast error\n\nIF YOU ARE NOT A DEVELOPER, JUST TELL HIM ABOUT IT.\nThanks");
        return;
    }

    if (user.hasProfile) {
        if (user.media.empty())
            bot->getApi().sendMessage(query->message->chat->id,
                                    fmt::vformat(
                                        mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "actions", "show_profile"),
                                             fmt::make_format_args(user.name, user.age, user.city, user.bio)
                                        )
            );
        else {
            if (user.bio.length() < 1024) /* Limitation of the TelegramAPI */ {
                std::vector<TgBot::InputMedia::Ptr> media;
                bool addedCaption = false;
                for (auto& element : user.media) {
                    if (!std::filesystem::exists(element))
                        continue;
                    auto inputMedia = std::make_shared<TgBot::InputMedia>();
                    inputMedia->media = element;
                    if (!addedCaption) {
                        inputMedia->caption = fmt::vformat(
                            mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "actions", "show_profile"),
                                 fmt::make_format_args(user.name, user.age, user.city, user.bio)
                            );
                        addedCaption = true;
                    }
                    media.emplace_back(inputMedia);
                }
                bot->getApi().sendMediaGroup(query->message->chat->id, media);
            } else {
                std::vector<TgBot::InputMedia::Ptr> media;
                for (auto& element : user.media) {
                    if (!std::filesystem::exists(element))
                        continue;
                    auto inputMedia = std::make_shared<TgBot::InputMedia>();
                    inputMedia->media = element;
                    media.emplace_back(inputMedia);
                }
                bot->getApi().sendMediaGroup(query->message->chat->id, media);
                bot->getApi().sendMessage(query->message->chat->id,
                                    fmt::vformat(
                                        mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "actions", "show_profile"),
                                             fmt::make_format_args(user.name, user.age, user.city, user.bio)
                                    )
                );
            }
        }
    } else
        bot->getApi().sendMessage(query->message->chat->id,
                    mlsys::getReplic(mlsys::castLanguageToFilename(user.language), "errors", "show_profile_not_found"));
}
