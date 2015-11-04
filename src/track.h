#pragma once
#include "ofxAudioDecoder.h"
#include "ofxAubio.h"
#include <vector>

#define BUFFERSIZE 256

class Track {
    
public:
    struct Data {
        bool onBeat;
        float bpm;
        bool isOnset;
        float pitch;
        float pitchConfidence;
    };
    
    Track(ofxAudioDecoder * decoder);
    
    std::vector<Data> frameData;
    
private:
    void addData(bool onBeat, float bpm, bool isOnset, float pitch, float pitchConfidence);
    
    ofxAubioOnset onset;
    ofxAubioPitch pitch;
    ofxAubioBeat beat;
//    ofxAubioMelBands bands;
};