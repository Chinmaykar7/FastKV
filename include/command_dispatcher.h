#pragma once

#include <string>
#include <vector>

class CommandDispatcher {
public:
    std::string dispatch(const std::vector<std::string>& command);
};