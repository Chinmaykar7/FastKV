#include "command_dispatcher.h"

#include <algorithm>
#include <cctype>

std::string CommandDispatcher::dispatch(const std::vector<std::string>& command) {
    if (command.empty()) {
        return "-ERR empty command\r\n";
    }

    std::string cmd = command[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(),
                   [](unsigned char c) { return std::toupper(c); });

    if (cmd == "PING") {
        return "+PONG\r\n";
    }

    return "-ERR unknown command\r\n";
}