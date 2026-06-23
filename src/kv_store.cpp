#include "kv_store.h"

#include <unordered_map>

struct KVStore::Impl {
    std::unordered_map<std::string, std::string> data;
};

KVStore::KVStore() : pimpl_(std::make_unique<Impl>()) {}
KVStore::~KVStore() = default;

void KVStore::set(const std::string& key, const std::string& value) {
    pimpl_->data[key] = value;
}

std::optional<std::string> KVStore::get(const std::string& key) const {
    auto it = pimpl_->data.find(key);
    if (it != pimpl_->data.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool KVStore::exists(const std::string& key) const {
    return pimpl_->data.find(key) != pimpl_->data.end();
}