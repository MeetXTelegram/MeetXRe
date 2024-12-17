#include <MLSystem.hpp>

#include <csignal>
#include <filesystem>
#include <fstream>
#include <unordered_map>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

inline std::string getFilesPath() {
    auto logger = spdlog::get("MLSystem");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"MLSystem\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    if (!std::getenv("HOME")) {
        logger->log(spdlog::level::err, "Failed to get files path: the HOME enivronment variable is not defined");
        std::raise(SIGUSR1);
    } else {
        if (!std::filesystem::exists(std::string(std::getenv("HOME")) + "/.meetx") || std::filesystem::exists(std::string("HOME") + "/.meetx/translations")) {
            logger->log(spdlog::level::err, "Failed to get files path: '.meetx'/'.meetx/translations' directory does not exists");
            std::raise(SIGUSR1);
        } else
            return std::string(std::string(std::getenv("HOME")) + "/.meetx/translations");
    }
}

static std::unordered_map<std::string, nlohmann::json> cached;

std::string mlsys::getReplic(const std::string& file, const std::string& section, const std::string& replic) {
    auto logger = spdlog::get("MLSystem");
    if (!logger) {
        spdlog::log(spdlog::level::critical, "The \"MLSystem\" logger is not registered, correct communication is not possible");
        std::exit(-1);
    }

    std::string targetFilePath = getFilesPath() + "/" + file;

    if (file.empty() || section.empty() || replic.empty()) {
        logger->log(spdlog::level::warn, "The file, section, or replic is not specified\nFile: {}; Section: {}; Replic: {}", file, section, replic);
        return "If you see this text, the getReplic call was incorrect(the file, section, or replica is not specified)";
    }

    if (cached.contains(file)) {
        auto restoredJson = cached[file];
        if (restoredJson.contains(section))
            if (restoredJson[section].contains(replic)) {
                logger->log(spdlog::level::info, "The replica is obtained from a cached nlohmann::json object!");
                return restoredJson[section][replic].get<std::string>();
            }
    }

    if (!std::filesystem::exists(targetFilePath)) {
        logger->log(spdlog::level::warn, "The translation file was not found\nFile: {}", file);
        return "If you see this text, the getReplic call was incorrect(the translation file was not found)";
    }

    std::ifstream jsonFile(targetFilePath);
    if (!jsonFile.is_open()) {
        logger->log(spdlog::level::warn, "Failed to open translations file\nFile: {}", file);
        return "If you see this text, the getReplic call was incorrect(file stream open failed)";
    }
    nlohmann::json j; jsonFile >> j;
    jsonFile.close();
    if (j.contains(section)) {
        if (j[section].contains(replic)) {
            cached[file] = j;
            return j[section][replic].get<std::string>();
        } else {
            logger->log(spdlog::level::warn, "The replic was not found\nFile: {}; Section: {}; Replic: {}", file, section, replic);
            return "If you see this text, the getReplic call was incorrect(replic was not found)";
        }
    } else {
        logger->log(spdlog::level::warn, "The section was not found in file\nFile: {}; Section: {}", file, section);
        return "If you see this text, the getReplic call was incorrect(section was not found)";
    }
}
