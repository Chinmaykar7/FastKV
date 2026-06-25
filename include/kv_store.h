#pragma once

#include <chrono>
#include <memory>
#include <optional>
#include <string>

class KVStore {
public:
    KVStore();
    ~KVStore();
    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    bool exists(const std::string& key) const;
    int del(const std::string& key);
    bool expire(const std::string& key, std::chrono::seconds ttl);

private:
    struct KVEntry {
        std::string value;
        std::optional<std::chrono::steady_clock::time_point> expiry;
    };

    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};
