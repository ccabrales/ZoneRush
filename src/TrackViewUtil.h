#pragma once

#include <stdio.h>
#include <ofMain.h>
#include "track.h"
class TrackView {
public:
    void setup(Track* track);
    void update(const Track::Data* data);
    void updateAudio(float * input, int bufferSize, int nChannels);
    void draw(int currentAudioTick);
private:
    
    Track* track;
    float onsetDecay[3];
    float beatDecay = 0;
    ofxAubioMelBands bands;
    ofPolyline bandPlot;
    ofPolyline pitchPlot;
    ofPolyline intensityPlot;

};
