#include "resp_parser.h"

#include <climits>

bool RespParser::parse(const std::string& input, std::vector<std::string>& command) {
    if (input.empty()) {
        return false;
    }

    size_t pos = 0;
    if (input[pos] != '*') {
        return false;
    }

    return parseArray(input, pos, command);
}

bool RespParser::parseArray(const std::string& input, size_t& pos, std::vector<std::string>& command) {
    if (pos >= input.size() || input[pos] != '*') {
        return false;
    }
    pos++;

    std::string countLine;
    if (!readLine(input, pos, countLine)) {
        return false;
    }

    int count;
    if (!parseInteger(countLine, count) || count < 0) {
        return false;
    }

    command.clear();
    command.reserve(count);

    for (int i = 0; i < count; ++i) {
        std::string value;
        if (!parseBulkString(input, pos, value)) {
            return false;
        }
        command.push_back(std::move(value));
    }

    return true;
}

bool RespParser::parseBulkString(const std::string& input, size_t& pos, std::string& value) {
    if (pos >= input.size() || input[pos] != '$') {
        return false;
    }
    pos++;

    std::string lengthLine;
    if (!readLine(input, pos, lengthLine)) {
        return false;
    }

    int length;
    if (!parseInteger(lengthLine, length) || length < 0) {
        return false;
    }

    if (pos + length + 2 > input.size()) {
        return false;
    }

    value = input.substr(pos, length);
    pos += length;

    if (pos + 1 >= input.size() || input[pos] != '\r' || input[pos + 1] != '\n') {
        return false;
    }
    pos += 2;

    return true;
}

bool RespParser::readLine(const std::string& input, size_t& pos, std::string& line) {
    size_t start = pos;
    while (pos < input.size() && input[pos] != '\r') {
        pos++;
    }

    if (pos >= input.size()) {
        return false;
    }

    line = input.substr(start, pos - start);

    if (pos + 1 >= input.size() || input[pos + 1] != '\n') {
        return false;
    }

    pos += 2;
    return true;
}

bool RespParser::parseInteger(const std::string& s, int& out) {
    if (s.empty()) {
        return false;
    }

    size_t i = 0;
    bool negative = false;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    } else if (s[0] == '+') {
        i = 1;
    }

    if (i >= s.size()) {
        return false;
    }

    long long value = 0;
    for (; i < s.size(); ++i) {
        char c = s[i];
        if (c < '0' || c > '9') {
            return false;
        }
        value = value * 10 + (c - '0');
        if (value > INT_MAX) {
            return false;
        }
    }

    out = negative ? -static_cast<int>(value) : static_cast<int>(value);
    return true;
}