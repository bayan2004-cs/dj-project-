#include "DJControllerService.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>

DJControllerService::DJControllerService(size_t cache_size)
    : cache(cache_size) {}//bnai
/**
 * TODO: Implement loadTrackToCache method
 */
int DJControllerService::loadTrackToCache(AudioTrack& track) {
    // Your implementation here 
    int output;
    if(cache.contains(track.get_title())){
        cache.get(track.get_title());//call chache.get() to refresh MRU status & return 1 
        output= 1;
    }else{
     
    PointerWrapper<AudioTrack> cloned_track_ptr = track.clone();

    if (!cloned_track_ptr.get()) {
        
        std::cout << "Error: clone() returned nullptr for track: " 
                  << track.get_title() << std::endl;//am i need to put "endl"?
        
        return 0;

        }
        cloned_track_ptr->load();
        cloned_track_ptr->analyze_beatgrid();
        bool eviction_result = cache.put(track.clone());
        if (eviction_result){
            output= -1;
        }else{
            output= 0;
        }
    }
    displayCacheStatus();
    return output;
 }


void DJControllerService::set_cache_size(size_t new_size) {
    cache.set_capacity(new_size);
}
//implemented
void DJControllerService::displayCacheStatus() const {
    std::cout << "\n=== Cache Status ===\n";
    cache.displayStatus();
    std::cout << "====================\n";
}

/**
 * TODO: Implement getTrackFromCache method
 */
AudioTrack* DJControllerService::getTrackFromCache(const std::string& track_title) {
    // Your implementation here
    AudioTrack* track_ptr = cache.get(track_title);
    return track_ptr;
}

