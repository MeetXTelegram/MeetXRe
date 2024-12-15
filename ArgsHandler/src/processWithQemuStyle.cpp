#include <Args.hpp>

#include <Exception.hpp>

void ah::Args::processWithQemuStyle() {
    this->lock_.lock();
    if (this->argsArray_.empty() || !this->rawArgsArray_)
        throw exceptions::InvalidArgsArray(this->argsArray_, "All arrays are crashed");
    for (auto iter = this->argsArray_.begin(); iter != this->argsArray_.end();) {
        if (iter == this->argsArray_.begin()) {
            this->argsMap_[*iter] = *iter;
            ++iter;
            continue;
        }
        if (iter + 1 != this->argsArray_.end()) {
            this->argsMap_[*iter] = *(iter + 1);
            iter += 2;
        } else
            this->argsMap_[*iter] = *iter;
    }
    this->lock_.unlock();
}
