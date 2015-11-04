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
    
    ofxAubioOnset onset;
    ofxAubioPitch pitch;
    ofxAubioBeat beat;

//    cout << ofSystem("../../../../sox") << endl;
    onset.setup();
    onset.setThreshold(0.1);
    pitch.setup();
    beat.setup();
    
    for (int i = 0; (i * BUFFERSIZE * nChannels) < decoder->getNumSamples(); i++) {
        float* sample = &(samples[i * BUFFERSIZE * nChannels]);
        
        onset.audioIn(sample, BUFFERSIZE, nChannels);
        pitch.audioIn(sample, BUFFERSIZE, nChannels);
        beat.audioIn(sample, BUFFERSIZE, nChannels);

        addData(beat.received(), beat.bpm, onset.received(), pitch.latestPitch, pitch.pitchConfidence, onset.novelty, onset.thresholdedNovelty);
        
    }
    
//    for (Data &d : frameData) {
//        cout << d.onBeat << " : " << d.bpm << " : " << d.isOnset << " : " << d.pitch << " : " << d.pitchConfidence << endl;
//    }
}

string Track::toString(Track::Data &d){
    stringstream s;
    s << d.onBeat << " : " << d.bpm << " : " << d.isOnset << " : " << d.pitch << " : " << d.pitchConfidence << " : " << d.onsetNovelty << " : " << d.onsetThresholdedNovelty;
    return s.str();
}

void Track::addData(bool onBeat, float bpm, bool isOnset, float pitch, float pitchConfidence, float onsetNovelty, float onsetThresholdNovelty) {
    Data d;
    d.onBeat = onBeat;
    d.bpm = bpm;
    d.isOnset = isOnset;
    d.pitch = pitch;
    d.pitchConfidence = pitchConfidence;
    d.onsetNovelty = onsetNovelty;
    d.onsetThresholdedNovelty = onsetThresholdNovelty;
    
    frameData.push_back(d);
}