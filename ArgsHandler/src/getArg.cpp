#include <Args.hpp>

#include <Exception.hpp>

std::optional<std::pair<std::string, std::string>> ah::Args::getArg(const std::string& name) {
    this->lock_.lock();
    if (this->argsMap_.count(name) < 1) {
#ifdef AH_USE_EXCEPTIONS
        this->lock_.unlock();
        throw exceptions::ValueNotFound(name, std::string("Not found value \"" + name + "\""));
#else
        this->lock_.unlock();
        return std::nullopt;
#endif
    }
    this->lock_.unlock();
    return std::make_pair(name, this->argsMap_.at(name));
}

std::pair<std::string, std::string> ah::Args::getArg(const std::string& name, const std::string& defaultValue) {
    this->lock_.unlock();
    if (this->argsMap_.count(name) < 1) {
        this->lock_.unlock();
        return std::make_pair(name, defaultValue);
    } else {
        this->lock_.unlock();
        return std::make_pair(name, this->argsMap_.at(name));
    }
}
