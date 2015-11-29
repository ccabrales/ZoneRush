
#pragma once
#include "ofMain.h"
#include "ofxSoundObjects.h"

class SineWaveGenerator: public ofxSoundObject {
public:
    void setup(float freq, float amplitude = 1.0, float phase = 0){
        this->freq = freq;
        this->amplitude = amplitude;
        this->phase = phase;
    }
    
    void process(ofSoundBuffer &in, ofSoundBuffer &out) {
        float m = TWO_PI* freq / in.getSampleRate();
//        frameIndex*myBuffer.getNumChannels()) + channelIndex
        int numFrames = out.getNumFrames();
        int numChannels = out.getNumChannels();
        for(int i = 0; i < numFrames; i++) {
            float s = sin(m*i + phase) * amplitude;
            for(int c = 0; c < numChannels; c++){
                out[i*numChannels + c ] = s;
            }
        }
        phase = m*numFrames + phase;
    }
    
    float freq;
    float amplitude;
    float phase;
};