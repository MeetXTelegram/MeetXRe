#include <Logic.hpp>

CreateProfileStep meetx::createProfileStep = CreateProfileStep::NONE;

std::shared_ptr<TgBot::Bot> meetx::bot = nullptr;

std::unordered_map<std::string, std::function<std::string(std::vector<std::string>)>> meetx::runnersList;

std::unordered_map<std::string, std::unordered_map<std::string,
                   void(*)(TgBot::CallbackQuery::Ptr query,
                   std::vector<std::string>)>> meetx::actionsList;