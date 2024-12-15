#pragma once

#include <csignal>
#include <string>

#include <spdlog/spdlog.h>

#include <Definitions.hpp>

namespace mlsys {
    inline std::string castLanguageToFilename(UserLanguage ul) {
        switch (ul) {
            case UserLanguage::BY: {
                return "by_BY.json";
            }

            case UserLanguage::EN: {
                return "en_EN.json";
            }

            case UserLanguage::FI: {
                return "fi_FI.json";
            }

            case UserLanguage::GR: {
                return "de_DE.json";
            }

            case UserLanguage::PL: {
                return "pl_PL.json";
            }

            case UserLanguage::RU: {
                return "ru_RU.json";
            }

            case UserLanguage::UK: {
                return "uk_UK.json";
            }

            default: {
                spdlog::get("MLSystem")->log(spdlog::level::err, "Failed to cast language: undefined language({})", static_cast<int>(ul));
                return "";
            }
        }
    }

    std::string getReplic(const std::string& file, const std::string& section, const std::string& replic);
}
