#pragma once

#include <csignal>
#include <chrono>
#include <sstream>
#include <functional>

#include <spdlog/spdlog.h>

#include <Args.hpp>
#include <Definitions.hpp>
#include <PSQLWrapper.hpp>

namespace init {
    static const std::string meetxFolder = std::string(std::getenv("HOME") + std::string("/.meetx"));
    static const std::string meetxLogsFolder = meetxFolder + "/logs";
    static const std::string meetxTranslationsFolder = meetxFolder + "/translations";

    inline void initLoggers() {
        auto nowTime = std::chrono::system_clock::now();
        auto nowTime_C = std::chrono::system_clock::to_time_t(nowTime);
        std::ostringstream formattedTime; formattedTime << std::put_time(std::localtime(&nowTime_C), "%F_%H_%M");

        auto stdoutSink = createStdoutSink;
        auto fileSink = createFileSink(meetxLogsFolder + "/MEETX_LOG_" + formattedTime.str() + ".txt");

        loggingUtils::createLogger("SignalHandler", stdoutSink, fileSink, spdlog::level::trace);
        loggingUtils::createLogger("MAIN_LOGGER", stdoutSink, fileSink, spdlog::level::trace);
        loggingUtils::createLogger("ArgsHandler", stdoutSink, fileSink, spdlog::level::trace);
        loggingUtils::createLogger("TerminateHandler", stdoutSink, fileSink, spdlog::level::trace);
        loggingUtils::createLogger("PSQL", stdoutSink, fileSink, spdlog::level::trace);
        loggingUtils::createLogger("TgBotBackend", stdoutSink, fileSink, spdlog::level::trace);
        loggingUtils::createLogger("MLSystem", stdoutSink, fileSink, spdlog::level::trace);
    }

    inline void MeetXInit(ah::Args& argsHandler) {
        if (!std::filesystem::exists(meetxFolder))
            std::filesystem::create_directory(meetxFolder);
        if (!std::filesystem::exists(meetxLogsFolder))
            std::filesystem::create_directory(meetxLogsFolder);
        if (!std::filesystem::exists(meetxTranslationsFolder))
            std::filesystem::create_directory(meetxTranslationsFolder);

        std::signal(SIGUSR1, signalsUtils::SIGUSR1Handler);
        std::set_terminate(exceptionsUtils::TerminateHandler);
        if (!argsHandler.getArg("dbname").has_value()) {
            spdlog::log(spdlog::level::critical, "Failed to init MeetX: the \"dbname\" argument is not provided");
            std::raise(SIGUSR1);
        }
        auto dbname = argsHandler.getArg("dbname").value().second;

        if (!argsHandler.getArg("password").has_value()) {
            spdlog::log(spdlog::level::critical, "Failed to init MeetX: the \"password\" argument is not provided");
            std::raise(SIGUSR1);
        }
        auto password = argsHandler.getArg("password").value().second;
        std::hash<std::string> passwordHasher;
        db::adminModePasswordHash = passwordHasher(password);

        if (!argsHandler.getArg("user").has_value()) {
            spdlog::log(spdlog::level::critical, "Failed to init MeetX: the \"user\" argument is not provided");
            std::raise(SIGUSR1);
        }
        auto user = argsHandler.getArg("user").value().second;

        try {
            db::database = std::make_shared<pqxx::connection>(std::string("dbname=" + dbname + " user=" + user + " password=" + password));
            pqxx::work w(*db::database);
            w.exec("CREATE TABLE IF NOT EXISTS users("
                   "id BIGINT PRIMARY KEY,"
                   "tgFirstName VARCHAR NOT NULL,"
                   "tgLastName VARCHAR,"
                   "age SMALLINT,"
                   "name VARCHAR NOT NULL,"
                   "bio VARCHAR,"
                   "gender SMALLINT NOT NULL,"
                   "language SMALLINT NOT NULL,"
                   "city VARCHAR,"
                   "media JSONB,"
                   "preferredLanguages JSONB,"
                   "preferredAges JSONB,"
                   "preferredCities JSONB,"
                   "preferredGenders JSONB,"
                   "hasProfile BOOLEAN NOT NULL DEFAULT FALSE"
                   ");");

            w.exec("CREATE TABLE IF NOT EXISTS banned_users("
                   "id BIGINT PRIMARY KEY,"
                   "reason VARCHAR,"
                   "at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP"
                   ");");
            w.commit();

        } catch (std::exception& dberr) {
            spdlog::log(spdlog::level::critical, "Failed to init MeetX: pqxx created an exception: {}", dberr.what());
            std::raise(SIGUSR1);
        }
    }
}
