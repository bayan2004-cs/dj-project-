#include "WAVTrack.h"
#include <iostream>
using namespace std;

WAVTrack::WAVTrack(const std::string& title, const std::vector<std::string>& artists, 
                   int duration, int bpm, int sample_rate, int bit_depth)
    : AudioTrack(title, artists, duration, bpm), sample_rate(sample_rate), bit_depth(bit_depth) {

    std::cout << "WAVTrack created: " << sample_rate << "Hz/" << bit_depth << "bit" << std::endl;
}

// ========== TODO: STUDENTS IMPLEMENT THESE VIRTUAL FUNCTIONS ==========

void WAVTrack::load() {
    // TODO: Implement realistic WAV loading simulation
    // NOTE: Use exactly 2 spaces before the arrow (→) character
    cout<<"[WAVTrack::load] loading WAV:\"" <<  get_title()<<"\" at " << this->sample_rate << "Hz/"<< this->bit_depth<<"bit (uncompressed)..."<<endl;
    long size= get_duration() * this->sample_rate * (this->bit_depth / 8) * 2;
    cout<<"  → Estimated file size: "<< size <<" bytes""\n";
    cout<<"  → Fast Loading due to uncompressed format.""\n";



}

void WAVTrack::analyze_beatgrid() {
    std::cout << "[WAVTrack::analyze_beatgrid] Analyzing beat grid for: \"" << title << "\"\n";
    // TODO: Implement WAV-specific beat detection analysis
    // Requirements:
    // 1. Print analysis message with track title
    // 2. Calculate beats: (duration_seconds / 60.0) * bpm
    // 3. Print number of beats and mention uncompressed precision
    // should print "  → Estimated beats: <beats>  → Precision factor: 1.0 (uncompressed audio)"

    //cout<<"[WAVTrack::analyze_beatgrid] Analyzing beat grid for: "<< get_title()<<"\n";
    double beats = (get_duration()/60.0)*get_bpm();
    cout<<"  → Estimated beats:" <<(int) beats ;
    cout<<"  → Precision factor: 1 (uncompressed audio)"<<std::endl;
}

double WAVTrack::get_quality_score() const {
    // TODO: Implement WAV quality scoring
    // NOTE: Use exactly 2 spaces before each arrow (→) character
    // NOTE: Cast beats to integer when printing
    int baseScore=70;
    if(sample_rate>=44100){
        baseScore+=10;
    }
    if(sample_rate>=96000){
        baseScore+=5;
    }
    if(bit_depth>=16){
        baseScore+=10;
    }
    if(bit_depth>=24){
        baseScore+=5;
    }
    if(baseScore>100){
        baseScore=100;
    }
    cout<<"[WAVTrack::get_quality_score] \" "<<this->title<< " \" score= "<<baseScore<<"/100"<<endl;
    return baseScore; // Replace with your implementation
}

PointerWrapper<AudioTrack> WAVTrack::clone() const {
    // TODO: Implement the clone method
    WAVTrack* ptrW = new WAVTrack(*this);
    return PointerWrapper<AudioTrack>(ptrW);
    // Replace with your implementation
}