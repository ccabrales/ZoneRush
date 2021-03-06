#pragma once
#include "ofxAudioDecoder.h"
#include "ofxAubio.h"
#include <vector>
#include "ofMain.h"
#include "globals.h"

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
        float intensity;
    };
    Track(){};
    
    Track(ofxAudioDecoder * decoder);
    static string toString(const Track::Data& d);

    Data* readData(int frame);

    vector<Data> frameData;
    float averageIntensity;
    float varianceIntensity;
private:
    int lastFrameRead = 0;
    void addData(bool onBeat, float bpm, bool isOnset, float pitch, float pitchConfidence, float onsetNovelty, float onsetThresholdNovelty);
//    ofxAubioMelBands bands;
};

