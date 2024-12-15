#include <Logic.hpp>

std::shared_ptr<TgBot::Bot> meetx::bot = nullptr;

std::unordered_map<std::string, std::function<std::string(std::vector<std::string>)>> meetx::runnersList;
