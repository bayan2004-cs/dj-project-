#include "MixingEngineService.h"
#include <iostream>
#include <memory>


/**
 * TODO: Implement MixingEngineService constructor
 */
MixingEngineService::MixingEngineService()
    : decks(), active_deck(1), auto_sync(false), bpm_tolerance(0)
{
    // Your implementation here
    decks[0] = nullptr;
    decks[1]=nullptr;

    std::cout << ("[MixingEngineService] Initialized with 2 empty decks.") << std::endl;
    
}

/**
 * TODO: Implement MixingEngineService destructor
 */
MixingEngineService::~MixingEngineService() {
    // Your implementation here
    
    std::cout << ("[MixingEngineService] Cleaning up decks...") << std::endl;
    
    int i=0;
    while(i<2){
        if(decks[i]!=nullptr){
            delete decks[i];
            decks[i]=nullptr;
        }
        i++;
    }
}



/**
 * TODO: Implement loadTrackToDeck method
 * @param track: Reference to the track to be loaded
 * @return: Index of the deck where track was loaded, or -1 on failure
 */
int MixingEngineService::loadTrackToDeck(const AudioTrack& track) {
    // Your implementation here
    std::cout << "\n===loading track to deck===" << std::endl;

    // 1. CLONE & FAILURE CHECK (Correct)
    PointerWrapper<AudioTrack> cloned_track = track.clone();
    if (!cloned_track) {
        std::cout << "[ERROR] Track:" << track.get_title() << " failed to clone." << std::endl;
        return -1;
    }

    size_t target_deck = 1 - active_deck;
    
    // 2. UNLOAD TARGET DECK (Correct)
    std::cout << "[Deck Switch] Target deck: " << target_deck << std::endl; // Added log from step (e)
    if (decks[target_deck]) {
        delete decks[target_deck];
        decks[target_deck] = nullptr;
    }

    // 3. PREPARATION & BPM SYNC (Corrected Logic)
    cloned_track->load();
    cloned_track->analyze_beatgrid();
    
    // Check if a track is currently active AND auto-sync is enabled (Corrected condition)
    if (decks[active_deck] != nullptr && auto_sync) {
        // Step (h): Check tolerance and sync if necessary
        if (!can_mix_tracks(cloned_track)) {
            sync_bpm(cloned_track);
        }
    }

    // --- DECK ASSIGNMENT & TRANSITION (MOVED OUTSIDE IF BLOCK) ---
    
    // Save the pointer to the deck that was active BEFORE the switch
    size_t old_active_deck = active_deck;

    // 4. ASSIGN OWNERSHIP & LOG (Corrected: Use 'track' title BEFORE release)
    // Save the title BEFORE release, as 'cloned_track' will be empty afterward.
    std::string track_title = cloned_track->get_title(); 
    
    // Transfer ownership to the target deck (Step (i))
    decks[target_deck] = cloned_track.release();
    
    std::cout << "[Load Complete] '" << track_title << "' is now loaded on deck " << target_deck << std::endl;

    // 5. UNLOAD PREVIOUS DECK (Step (j))
    if (decks[old_active_deck] != nullptr) {
        std::cout << "[Unload] Unloading previous deck " << old_active_deck << " (" << decks[old_active_deck]->get_title() << ")" << std::endl;
        
        delete decks[old_active_deck];
        decks[old_active_deck] = nullptr;
    }
    
    // 6. SWITCH ACTIVE DECK (Step (k))
    active_deck = target_deck;

    // 7. FINAL LOGS & RETURN (Step (l))
    std::cout << "[Active Deck] Switched to deck " << target_deck << std::endl;
    displayDeckStatus();
    
    return target_deck;
    
}

/**
 * @brief Display current deck status
 */
void MixingEngineService::displayDeckStatus() const {
    std::cout << "\n=== Deck Status ===\n";
    for (size_t i = 0; i < 2; ++i) {
        if (decks[i])
            std::cout << "Deck " << i << ": " << decks[i]->get_title() << "\n";
        else
            std::cout << "Deck " << i << ": [EMPTY]\n";
    }
    std::cout << "Active Deck: " << active_deck << "\n";
    std::cout << "===================\n";
}

/**
 * TODO: Implement can_mix_tracks method
 * 
 * Check if two tracks can be mixed based on BPM difference.
 * 
 * @param track: Track to check for mixing compatibility
 * @return: true if BPM difference <= tolerance, false otherwise
 */
bool MixingEngineService::can_mix_tracks(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] == nullptr){
        return false;
    }
    if(track.get() == nullptr){
        return false;
    }
    int a_bpm = decks[active_deck]->get_bpm();//a_bpm=active bpm
    int n_bpm = track.get()->get_bpm();//n_bpm= new bpm 
    int difference = (a_bpm - n_bpm);
    if(difference < 0){
        difference = -difference;
    }
    if(difference > bpm_tolerance){
        return true;
    }else{
        return false;
    }
}

/**
 * TODO: Implement sync_bpm method
 * @param track: Track to synchronize with active deck
 */
void MixingEngineService::sync_bpm(const PointerWrapper<AudioTrack>& track) const {
    // Your implementation here
    if(decks[active_deck] == nullptr){
        return;
    }
    if(!track){
        return;
    }
    int a_bpm = decks[active_deck] -> get_bpm();//a_bpm=active bpm
    int o_bpm =track -> get_bpm();//o_bpm = original n=bpm
    int average = (a_bpm + o_bpm) / 2 ;
    track->setBpm(average);
    std::cout<<"[Sync BPM] Syncing BPM from "<<o_bpm<<" to "<<average<<std::endl;

}

