#include "DJLibraryService.h"
#include "SessionFileParser.h"
#include "MP3Track.h"
#include "WAVTrack.h"
#include <iostream>
#include <memory>
#include <filesystem>


DJLibraryService::DJLibraryService(const Playlist& playlist) 
    : playlist(playlist),library() {}
/**
 * @brief Load a playlist from track indices referencing the library
 * @param library_tracks Vector of track info from config
 */
void DJLibraryService::buildLibrary(const std::vector<SessionConfig::TrackInfo>& library_tracks) {
    //Todo: Implement buildLibrary method
    //std::cout << "TODO: Implement DJLibraryService::buildLibrary method\n"<< library_tracks.size() << " tracks to be loaded into library.\n";
    for(size_t i=0 ; i<library_tracks.size() ; i++){
        const auto& T = library_tracks[i];
        
        if(T.type== "MP3"){
            MP3Track* newMp3=new MP3Track(T.title,T.artists,T.duration_seconds,T.bpm,T.extra_param1,true);
            this->library.push_back(newMp3);
            
            //std::cout<<"MP3Track created: " <<newMp3->get_bitrate()<<" kbps"<<std::endl;
            
        }else{
            WAVTrack* newWav=new WAVTrack(T.title,T.artists,T.duration_seconds,T.bpm,T.extra_param1,T.extra_param2);
            this->library.push_back(newWav);
            
            //std::cout<<"WAVTrack created: " <<newWav->get_sample_rate()<<"Hz/"<<newWav->get_bit_depth()<<"bit"<<std::endl;
            
        }

    }
    
    std::cout<<"[INFO] Track library built: "<<this->library.size()<<" tracks loaded" <<std::endl;
    
}

/**
 * @brief Display the current state of the DJ library playlist
 * 
 */
void DJLibraryService::displayLibrary() const {
    std::cout << "=== DJ Library Playlist: " 
              << playlist.get_name() << " ===" << std::endl;

    if (playlist.is_empty()) {
        std::cout << "[INFO] Playlist is empty.\n";
        return;
    }

    // Let Playlist handle printing all track info
    playlist.display();

    std::cout << "Total duration: " << playlist.get_total_duration() << " seconds" << std::endl;
}

/**
 * @brief Get a reference to the current playlist
 * 
 * @return Playlist& 
 */
Playlist& DJLibraryService::getPlaylist() {
    // Your implementation here
    return playlist;
}

/**
 * TODO: Implement findTrack method
 * 
 * HINT: Leverage Playlist's find_track method
 */
AudioTrack* DJLibraryService::findTrack(const std::string& track_title) {
    // Your implementation here
    return playlist.find_track(track_title); // Placeholder
}

void DJLibraryService::loadPlaylistFromIndices(const std::string& playlist_name, 
                                               const std::vector<int>& track_indices) {
    // Your implementation here
    
    std::cout<<"[INFO] loading playlist: "<<playlist_name<<std::endl;
    
    
    Playlist* newPL=new Playlist(playlist_name);
    for(size_t i=0;i<track_indices.size();i++){
        size_t index_1=track_indices[i];
        if(index_1==0  || index_1>this->library.size()){
            
            std::cout<<"[WARNING] Invalid track index: "<<index_1<<std::endl;
            
            continue;
        }
        size_t index_0=index_1-1;
        AudioTrack* clonedTrack = library[index_0]->clone().release();
        if(!clonedTrack){
            
          std::cout<<"[ERROR]" <<std::endl;
            
        continue;
        }
        clonedTrack->load();
        clonedTrack->analyze_beatgrid();
        newPL->add_track(clonedTrack);
        this->playlist=*newPL;
        
        
        
        //std::cout<<"added "<<"'"<<clonedTrack->get_title()<<"'"<<"  to playlist "<<"'"<<playlist_name<<"'" <<std::endl;
        
    }
    
    std::cout<<"[INFO] Playlist Loaded:"<<playlist_name<<"("<<this->playlist.get_track_count()<<" tracks)" <<std::endl;
    
}
/**
 * TODO: Implement getTrackTitles method
 * @return Vector of track titles in the playlist
 */
std::vector<std::string> DJLibraryService::getTrackTitles() const {
    // Your implementation here
    std::vector<std::string> output;
    PlaylistNode* curr=playlist.get_head();
    while(curr){
        output.push_back(curr->track->get_title());
        curr=curr->next;
    }
    return output; // Placeholder
}

