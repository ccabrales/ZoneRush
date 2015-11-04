//
//  track.cpp
//  ZoneRushMaxima
//
//  Created by Casey Cabrales on 11/3/15.
//
//

#include <stdio.h>
#include "track.h"

Track::Track(ofxAudioDecoder * decoder) { //Get information from the entire file
    int nChannels = decoder->getChannels();
//    onset.threshold = 0.5;
    std::vector<float> samples(decoder->getRawSamples());
//    cout << ofSystem("../../../../sox") << endl;
    onset.setup();
    pitch.setup();
    beat.setup();
    
    for (int i = 0; (i * BUFFERSIZE * nChannels) < decoder->getNumSamples(); i++) {
        float* sample = &(samples[i * BUFFERSIZE * nChannels]);
        
        onset.audioIn(sample, BUFFERSIZE, nChannels);
        pitch.audioIn(sample, BUFFERSIZE, nChannels);
        beat.audioIn(sample, BUFFERSIZE, nChannels);

        addData(beat.received(), beat.bpm, onset.received(), pitch.latestPitch, pitch.pitchConfidence);
        
    }
    
//    for (Data &d : frameData) {
//        cout << d.onBeat << " : " << d.bpm << " : " << d.isOnset << " : " << d.pitch << " : " << d.pitchConfidence << endl;
//    }
}

void Track::addData(bool onBeat, float bpm, bool isOnset, float pitch, float pitchConfidence) {
    Data d;
    d.onBeat = onBeat;
    d.bpm = bpm;
    d.isOnset = isOnset;
    d.pitch = pitch;
    d.pitchConfidence = pitchConfidence;
    
    frameData.push_back(d);
}