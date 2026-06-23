#include "command_dispatcher.h"
#include "kv_store.h"

#include <algorithm>
#include <cctype>

CommandDispatcher::CommandDispatcher(KVStore& store) : store_(store) {}

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

    if (cmd == "SET") {
        if (command.size() != 3) {
            return "-ERR wrong number of arguments for 'set' command\r\n";
        }
        store_.set(command[1], command[2]);
        return "+OK\r\n";
    }

    if (cmd == "GET") {
        if (command.size() != 2) {
            return "-ERR wrong number of arguments for 'get' command\r\n";
        }
        auto value = store_.get(command[1]);
        if (value.has_value()) {
            return "$" + std::to_string(value->size()) + "\r\n" + *value + "\r\n";
        }
        return "$-1\r\n";
    }

    if (cmd == "EXISTS") {
        if (command.size() != 2) {
            return "-ERR wrong number of arguments for 'exists' command\r\n";
        }
        return ":" + std::to_string(store_.exists(command[1]) ? 1 : 0) + "\r\n";
    }

    return "-ERR unknown command '" + command[0] + "'\r\n";
}