#pragma once

#include <exception>
#include <string>
#include <variant>
#include <vector>

namespace ah {
    namespace exceptions {
        class ValueNotFound : public std::exception {
        public:
            ValueNotFound(const std::string& name, const std::string& whatt) : name_(name), what_(whatt) {}

            const std::string& getName() const noexcept { return this->name_; }
            const char* what() const noexcept override { return this->what_.c_str(); }
        private:
            std::string name_;
            std::string what_;
        };

        class InvalidArgsArray : public std::exception {
        public:
            InvalidArgsArray(std::variant<std::vector<std::string>, char**> array, const std::string whatt) : what_(whatt) {
                if (std::holds_alternative<std::vector<std::string>>(array))
                    this->array_ = std::get<std::vector<std::string>>(array);
                else {
                    auto rawArray = std::get<char**>(array);
                    for (std::size_t i = 0; rawArray[i]; ++i) {
                        this->array_.emplace_back(rawArray[i]);
                    }
                }
            }

            std::vector<std::string> getArray() const noexcept { return this->array_; }
            const char* what() const noexcept override { return this->what_.c_str(); }
        private:
            std::string what_;
            std::vector<std::string> array_;
        };
    }
}
