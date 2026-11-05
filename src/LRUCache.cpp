#include "LRUCache.h"
#include <iostream>

LRUCache::LRUCache(size_t capacity)
    : slots(capacity), max_size(capacity), access_counter(0) {}

bool LRUCache::contains(const std::string& track_id) const {
    return findSlot(track_id) != max_size;
}

AudioTrack* LRUCache::get(const std::string& track_id) {
    size_t idx = findSlot(track_id);
    if (idx == max_size) return nullptr;
    return slots[idx].access(++access_counter);
}

/**
 * TODO: Implement the put() method for LRUCache
 */
bool LRUCache::put(PointerWrapper<AudioTrack> track) {
    bool result=false;
    if(!track){
        return 0;
    }
    size_t isThere= findSlot((track.get())->get_title());
    if(isThere!=max_size){
        slots[isThere].access(++access_counter);
        result=false;
    }
     if(this->isFull()){
        evictLRU();
        result=true;
    }
    size_t newSlot = findEmptySlot();
    slots[newSlot].store(std::move(track) , ++access_counter);
    
    return result; 
}

bool LRUCache::evictLRU() {
    size_t lru = findLRUSlot();
    if (lru == max_size || !slots[lru].isOccupied()) return false;
    slots[lru].clear();
    return true;
}

size_t LRUCache::size() const {
    size_t count = 0;
    for (const auto& slot : slots) if (slot.isOccupied()) ++count;
    return count;
}

void LRUCache::clear() {
    for (auto& slot : slots) {
        slot.clear();
    }
}

void LRUCache::displayStatus() const {
    std::cout << "[LRUCache] Status: " << size() << "/" << max_size << " slots used\n";
    for (size_t i = 0; i < max_size; ++i) {
        if(slots[i].isOccupied()){
            std::cout << "  Slot " << i << ": " << slots[i].getTrack()->get_title()
                      << " (last access: " << slots[i].getLastAccessTime() << ")\n";
        } else {
            std::cout << "  Slot " << i << ": [EMPTY]\n";
        }
    }
}

size_t LRUCache::findSlot(const std::string& track_id) const {
    for (size_t i = 0; i < max_size; ++i) {
        if (slots[i].isOccupied() && slots[i].getTrack()->get_title() == track_id) return i;
    }
    return max_size;

}

/**
 * TODO: Implement the findLRUSlot() method for LRUCache
 */
size_t LRUCache::findLRUSlot() const {
    if (this->size() == 0) {
        return max_size;
    }

    // Initialize tracking variables
    uint64_t min_access_time = 0;
    size_t lru_index = max_size; // Initialize index to indicate "not found"

    // 1. Iterate through ALL possible slots (from 0 up to max_size - 1)
    for (size_t i = 0; i < max_size; ++i) {
        
        // 2. CRITICAL: Only check occupied slots
        if (slots[i].isOccupied()) {
            
            uint64_t current_time = slots[i].getLastAccessTime();

            // 3. Initialize min_access_time on the first occupied slot found
            if (lru_index == max_size) {
                min_access_time = current_time;
                lru_index = i;
            } 
            // 4. Update the minimum time and store the index
            else if (current_time < min_access_time) {
                min_access_time = current_time;
                lru_index = i;
            }
        }
    }
    
    // 5. Return the index of the LRU slot
    return lru_index;// Placeholder
}

size_t LRUCache::findEmptySlot() const {
    for (size_t i = 0; i < max_size; ++i) {
        if (!slots[i].isOccupied()) return i;
    }
    return max_size;
}

void LRUCache::set_capacity(size_t capacity){
    if (max_size == capacity)
        return;
    //udpate max size
    max_size = capacity;
    //update the slots vector
    slots.resize(capacity);
}
