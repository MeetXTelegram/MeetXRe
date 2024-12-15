#include <Definitions.hpp>

void loggingUtils::createLogger(const std::string& loggerName, std::shared_ptr<spdlog::sinks::sink> consoleSink, std::shared_ptr<spdlog::sinks::sink> fileSink, spdlog::level::level_enum level) {
    if (!consoleSink || !fileSink) {
        spdlog::log(spdlog::level::critical, "Failed to create logger \"{}\": invalid consoleSink and/or fileSink provided", loggerName);
        std::raise(SIGUSR1);
    }

    auto nLogger = std::make_shared<spdlog::logger>(loggerName, spdlog::sinks_init_list{fileSink, consoleSink});
    nLogger->set_level(level);
    spdlog::register_logger(nLogger);
    if (loggerName == "MAIN_LOGGER" /* Global logger name */)
    	spdlog::set_default_logger(nLogger);
}
