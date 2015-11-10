//
//  TrackViewUtil.hpp
//  ZoneRushMaxima
//
//  Created by Xiaonan Tong on 11/10/15.
//
//

#ifndef TrackViewUtil_hpp
#define TrackViewUtil_hpp

#include <stdio.h>
#include <ofMain.h>
#include "track.h"
class TrackView {
public:
    void setup(Track* track);
    void update(const Track::Data& data);
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

#endif /* TrackViewUtil_hpp */
