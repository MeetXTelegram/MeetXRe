#include <Args.hpp>

#include <regex>

void ah::Args::processWithCmakeStyle() {
    std::regex patternWithoutBrackets("^(.*?)=(.*)$");
    std::regex patternWithBrackets(R"(^(.*?)=\"(.*)\"$)");
    std::smatch matches;
    for (auto& iter : this->argsArray_) {
        if (std::regex_search(iter, matches, patternWithBrackets))
            this->argsMap_[matches[1].str()] = matches[2].str();
        else if (std::regex_search(iter, matches, patternWithoutBrackets))
            this->argsMap_[matches[1].str()] = matches[2].str();
        else
            spdlog::get("ArgsHandler")->log(spdlog::level::debug, "Invalid argument: \"{}\"", iter);
    }
}
