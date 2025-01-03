#pragma once

#include <csetjmp>
#include <functional>
#include <unordered_map>
#include <vector>

#include <tgbot/tgbot.h>

#include <Ban.hpp>
#include <Definitions.hpp>
#include <Repassword.hpp>
#include <Show.hpp>
#include <ShowProfile.hpp>
#include <Unban.hpp>

namespace db {
    extern std::shared_ptr<pqxx::connection> database;
}

namespace signalsUtils {
    extern sigjmp_buf programBuf;
}

namespace meetx {
    extern CreateProfileStep createProfileStep;
    extern std::shared_ptr<TgBot::Bot> bot;
    void MeetXStart(const std::string& token = "");

    extern std::unordered_map<std::string, std::function<std::string(std::vector<std::string>)>> runnersList;
    inline void InitRunners() {
        runnersList["repassword"] = runners::Repassword;
        runnersList["ban"] = runners::Ban;
        runnersList["unban"] = runners::Unban;
        runnersList["show"] = runners::Show;
    }

    extern std::unordered_map<std::string, std::unordered_map<std::string,
                              void(*)(TgBot::CallbackQuery::Ptr query,
                              std::vector<std::string>)>> actionsList;

    inline void InitActions() {
        actionsList["show"]["profile"] = actions::ShowProfile;
    }

    namespace handlers {
        void RunCommandHandler(TgBot::Message::Ptr message);
        void AnyMessageHandler(TgBot::Message::Ptr message);
        void HelpCommandHandler(TgBot::Message::Ptr message);
        void StartCommandHandler(TgBot::Message::Ptr message);
        void CallbackQueryHandler(TgBot::CallbackQuery::Ptr query);
    }

    void MainLoop();
}
