#pragma once

#include <csetjmp>
#include <csignal>
#include <cstdint>
#include <string>
#include <memory>
#include <vector>

#include <pqxx/pqxx>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define createFileSink(file) (std::make_shared<spdlog::sinks::basic_file_sink_mt>((file), true))
#define createStdoutSink std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

namespace db {
   	extern std::shared_ptr<pqxx::connection> database;
    extern std::size_t adminModePasswordHash;
}

namespace loggingUtils {
    void createLogger(const std::string& loggerName, std::shared_ptr<spdlog::sinks::sink> consoleSink, std::shared_ptr<spdlog::sinks::sink> fileSink, spdlog::level::level_enum level);
}

namespace signalsUtils {
    extern sigjmp_buf programBuf;
    void SIGUSR1Handler(int signal);
}

namespace exceptionsUtils {
    void TerminateHandler();
}


enum class UserLanguage {
    RU = 1, /* Russian */
    EN = 2, /* English */
    BY = 3, /* Belarus */
    UK = 4, /* Ukrain */ /* For how long? ;) */
    PL = 5, /* Poland */
    FI = 6, /* Finland */
    GR = 7, /* Germany */
    /* More soon */
};

enum class UserGender {
    NOT_DEFINED = 0,
    MALE = 1, /* Man */
    FEMALE = 2, /* Girl */
};

/* Small hint of the User structure
 *
 * id: std::int64_t, Unique user ID
 * tgFirstName: std::string, first_name(name) in Telegram
 * tgLastName: std::string, last_name(last name) in Telegram
 * age: std::uint16_t, user's age
 * name: std::string, user's name
 * bio: std::string, user's bio
 * country: UserCountry, user's country
 * language: UserLanguage, user's language
 * city: std::string user's city
 * gender: UserGender, user's gender
 * media: std::vector<std::string>, user's media(photo, video)
 * preferredCountries: std::vector<UserCountry>, user's preferred countries
 * preferredLanguages: std::vector<UserLanguage>, user's preferred languages
 * preferredAges: std::vector<std::uint16_t>, user's preferred ages
 * preferredCities: std::vector<std::string>, user's preferred cities
 * preferredGenders: std::vector<UserGender>, user's preferred genders
 *
 */
struct User {
    std::int64_t id;
    std::string tgFirstName;
    std::string tgLastName;
    std::string username;
    std::uint16_t age;
    std::string name;
    std::string bio;
    UserLanguage language = UserLanguage::EN;
    std::string city;
    UserGender gender = UserGender::NOT_DEFINED;
    std::vector<std::string> media;
    std::vector<UserLanguage> preferredLanguages;
    std::vector<std::uint16_t> preferredAges;
    std::vector<std::string> preferredCities;
    std::vector<UserGender> preferredGenders;
    bool hasProfile = false;
};
