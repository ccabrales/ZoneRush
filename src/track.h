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
        int onsets;
        float onsetNovelty;
        float onsetThresholdedNovelty;
        float pitch;
        float pitchConfidence;
    };
    
    Track(ofxAudioDecoder * decoder);
    static string toString(Track::Data& d);
    
    Data readData(int frame);
    
    std::vector<Data> frameData;
    
private:
    int lastFrameRead = 0;
    void addData(bool onBeat, float bpm, bool isOnset, float pitch, float pitchConfidence, float onsetNovelty, float onsetThresholdNovelty);
//    ofxAubioMelBands bands;
};