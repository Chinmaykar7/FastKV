#pragma once

#include <memory>
#include <optional>
#include <string>

class KVStore {
public:
    KVStore();
    ~KVStore();
    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};