#pragma once

#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <spdlog/spdlog.h>

namespace ah {
    class Args {
    public:
        Args(int argc, char* argv[]) : argsQuantity_(static_cast<std::size_t>(argc)), rawArgsArray_(argv) {
            for (std::size_t i = 0; i < this->argsQuantity_; ++i) {
                if (argv[i]) {
                    spdlog::get("ArgsHandler")->log(spdlog::level::trace, "New element added: \"{}\"", argv[i]);
                    this->argsArray_.emplace_back(argv[i]);
                }
            }
        }

        enum class ArgsStyle {
            LIKE_QEMU, /* --argument value */
            LIKE_CMAKE, /* --argument=value */
        };

        void process(ArgsStyle style) {
            switch (style) {
                case ArgsStyle::LIKE_CMAKE: {
                    this->processWithCmakeStyle();
                    break;
                }

                case ArgsStyle::LIKE_QEMU: {
                    this->processWithQemuStyle();
                    break;
                }
            }
        }

        const std::unordered_map<std::string, std::string>& getMap() { return this->argsMap_; }
        const std::vector<std::string>& getArray() { return this->argsArray_; }

        std::optional<std::pair<std::string, std::string>> getArg(const std::string& name);
        std::pair<std::string, std::string> getArg(const std::string& name, const std::string& defaultValue);
    private:
        void processWithQemuStyle();
        void processWithCmakeStyle();

        std::mutex lock_;
        std::size_t argsQuantity_;
        char** rawArgsArray_;
        std::vector<std::string> argsArray_;
        std::unordered_map<std::string, std::string> argsMap_;
    };
}
