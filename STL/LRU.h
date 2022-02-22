#pragma once
#include <list>
#include <unordered_map>

template <typename KeyT, typename ValueT>
class LRUCache {
public:
    void Put(const KeyT& key, const ValueT& val) {
        auto cache_map_it = cache_map_.find(key);
        if (cache_map_it != cache_map_.end()) {
            cache_map_.erase(cache_map_it);
        }

        cache_.push_back(val);
        cache_map_[key] = --cache_.end();
    }

    bool Get(const KeyT& key, ValueT& value) {
        auto cache_map_it = cache_map_.find(key);
        if (cache_map_it == cache_map_.end()) {
            return false;
        }

        value = *(cache_map_it->second);
        cache_.erase(cache_map_it->second);
        cache_.push_back(value);
        cache_map_[key] = --cache_.end();
        return true;
    }

    bool EraseLRU(ValueT* erase_value = nullptr) {
        if (cache_.size() == 0) {
            return false;
        }

        if (erase_value != nullptr) {
            *erase_value = cache_.front();
        }
        auto value = cache_.front();
        cache_.pop_front();

        auto cache_map_it = cache_map_.find(value->GetKey());
        if (cache_map_it != cache_map_.end()) {
            cache_map_.erase(cache_map_it);
        }
        return true;
    }

    bool Erase(const KeyT& key, ValueT* erase_value = nullptr) {
        auto cache_map_it = cache_map_.find(key);
        if (cache_map_it != cache_map_.end()) {
            if (erase_value) {
                *erase_value = *(cache_map_it->second);
            }

            cache_.erase(cache_map_it->second);
            cache_map_.erase(cache_map_it);
            return true;
        }
        return false;
    }

    std::list<ValueT> Clear() {
        auto backup = cache_;
        cache_map_.clear();
        cache_.clear();
        return backup;
    }

    size_t GetLRUCacheCount() { return cache_.size(); }

private:
    std::unordered_map<KeyT, typename std::list<ValueT>::iterator> cache_map_;
    std::list<ValueT> cache_;
};