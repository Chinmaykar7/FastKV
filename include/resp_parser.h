#pragma once

#include <string>
#include <vector>

class RespParser {
public:
    bool parse(const std::string& input, std::vector<std::string>& command);

private:
    bool parseArray(const std::string& input, size_t& pos, std::vector<std::string>& command);
    bool parseBulkString(const std::string& input, size_t& pos, std::string& value);
    bool readLine(const std::string& input, size_t& pos, std::string& line);
    bool parseInteger(const std::string& s, int& out);
};