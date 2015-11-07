#include <stdio.h>
#include "track.h"
#include "filter.h"

#define GAUSSIAN_SIZE 3 //[][][][X][][][]
const float gaussian[] = {0.004, 0.054, 0.242, 0.399, 0.242, 0.054, 0.004};

Track::Track(ofxAudioDecoder * decoder) { //Get information from the entire file
    int nChannels = decoder->getChannels();
//    onset.threshold = 0.5;
    std::vector<float> samples(decoder->getRawSamples());
    
    ofxAubioOnset onset1;
    ofxAubioOnset onset2;
    ofxAubioOnset onset3;
    
    ofxAubioPitch pitch;
    ofxAubioBeat beat;

//    cout << ofSystem("../../../../sox") << endl;
    onset1.setup("hfc", 512, 256, 44100);
    onset1.setThreshold(0.4);
    
    onset2.setup("specflux", 512, 256, 44100);
    onset2.setThreshold(0.3);
    
    onset3.setup("complex", 512, 256, 44100);
    onset3.setThreshold(0.2);
    
    pitch.setup();
    beat.setup();
    
    for (int i = 0; (i * BUFFERSIZE * nChannels) < decoder->getNumSamples(); i++) {
        float* sample = &(samples[i * BUFFERSIZE * nChannels]);
        
        onset1.audioIn(sample, BUFFERSIZE, nChannels);
        onset2.audioIn(sample, BUFFERSIZE, nChannels);
        onset3.audioIn(sample, BUFFERSIZE, nChannels);
        pitch.audioIn(sample, BUFFERSIZE, nChannels);
        beat.audioIn(sample, BUFFERSIZE, nChannels);

        frameData.push_back({
            beat.received(),
            beat.bpm,
            (onset1.received() << 2) | (onset2.received() << 1) | (onset3.received()),
                //^ 0 to 7. [000] - [111]
            onset1.novelty, onset1.thresholdedNovelty, //thresholded stuff.
            pitch.latestPitch,
            pitch.pitchConfidence
        });
    }
    
    std::vector<int> f(frameData.size());
    
    for(int i = 0; i < frameData.size(); i++){
        f[i] = ((int)(round(frameData[i].pitch)));
    }
    
    std::cout << f[412] << std::endl;
    
    int fOut[frameData.size()];
//    medianfilter(fOut, &f[0], frameData.size());
    median_filter_impl_1d(frameData.size(), 31, 70, &f[0], &fOut[0]);
    

    for(int i = 0; i < frameData.size(); i++){
        frameData[i].pitch = fOut[i];
    }
    
        
//    for (Data &d : frameData) {
//        cout << d.onBeat << " : " << d.bpm << " : " << d.isOnset << " : " << d.pitch << " : " << d.pitchConfidence << endl;
//    }
}

string Track::toString(Track::Data &d){
    stringstream s;
    s << d.onBeat << "\t" << d.bpm << "\t" << d.onsets << "\t" << d.pitch << "\t" << d.pitchConfidence << "\t" << d.onsetNovelty << "\t" << d.onsetThresholdedNovelty;
    return s.str();
}

Track::Data Track::readData(int frame){
    Data out = frameData[frame];
    for(int i=lastFrameRead+1; i <= frame; i++){
        out.onBeat |= frameData[i].onBeat;
        out.onsets = (out.onsets | frameData[i].onsets);
    }
    lastFrameRead = frame;
    return out;
}
