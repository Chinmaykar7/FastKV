#include "kv_store.h"

#include <unordered_map>

struct KVStore::Impl {
    mutable std::unordered_map<std::string, KVEntry> data;
};

KVStore::KVStore() : pimpl_(std::make_unique<Impl>()) {}
KVStore::~KVStore() = default;

void KVStore::set(const std::string& key, const std::string& value) {
    pimpl_->data[key] = KVEntry{value, std::nullopt};
}

bool KVStore::removeIfExpired(const std::string& key) const {
    auto it = pimpl_->data.find(key);
    if (it == pimpl_->data.end()) {
        return false;
    }
    if (!it->second.expiry.has_value()) {
        return false;
    }
    if (std::chrono::steady_clock::now() >= it->second.expiry.value()) {
        pimpl_->data.erase(it);
        return true;
    }
    return false;
}

std::optional<std::string> KVStore::get(const std::string& key) const {
    removeIfExpired(key);
    auto it = pimpl_->data.find(key);
    if (it != pimpl_->data.end()) {
        return it->second.value;
    }
    return std::nullopt;
}

bool KVStore::exists(const std::string& key) const {
    removeIfExpired(key);
    return pimpl_->data.find(key) != pimpl_->data.end();
}

int KVStore::del(const std::string& key) {
    removeIfExpired(key);
    return pimpl_->data.erase(key);
}

bool KVStore::expire(const std::string& key, std::chrono::seconds ttl) {
    auto it = pimpl_->data.find(key);
    if (it == pimpl_->data.end()) {
        return false;
    }
    it->second.expiry = std::chrono::steady_clock::now() + ttl;
    return true;
}
