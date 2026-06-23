#pragma once

#include <string>
#include <vector>

class KVStore;

class CommandDispatcher {
public:
    explicit CommandDispatcher(KVStore& store);
    std::string dispatch(const std::vector<std::string>& command);

private:
    KVStore& store_;
};