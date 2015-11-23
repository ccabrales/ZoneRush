#include <stdio.h>
#include "track.h"
#include "filter.h"

#define GAUSSIAN_SIZE 3 //[][][][X][][][]
const float gaussian[] = {0.004, 0.054, 0.242, 0.399, 0.242, 0.054, 0.004};

/**
 Given a decoder that has loaded an audio file, parse the audio content and store it within the class for later access.
 @param decoder - AudioDecoder with loaded mp3 file.
 */
Track::Track(ofxAudioDecoder * decoder) {
    int nChannels = decoder->getChannels();
    std::vector<float> samples(decoder->getRawSamples());
    
    ofxAubioOnset onset1;
    ofxAubioOnset onset2;
    ofxAubioOnset onset3;
    
    ofxAubioPitch pitch;
    ofxAubioBeat beat;
    ofxAubioMelBands bands;

    onset1.setup("hfc", 512, 256, 44100);
    onset1.setThreshold(0.4);
    
    onset2.setup("specflux", 512, 256, 44100);
    onset2.setThreshold(0.3);
    
    onset3.setup("complex", 512, 256, 44100);
    onset3.setThreshold(0.2);
    
    pitch.setup();
    beat.setup();
    bands.setup();
    //bands have 40 energies counts.
    
    //Audio Analysis
    //==============
    for (int i = 0; (i * BUFFERSIZE * nChannels) < decoder->getNumSamples(); i++) {
        float* sample = &(samples[i * BUFFERSIZE * nChannels]);
        
        onset1.audioIn(sample, BUFFERSIZE, nChannels);
        onset2.audioIn(sample, BUFFERSIZE, nChannels);
        onset3.audioIn(sample, BUFFERSIZE, nChannels);
        pitch.audioIn(sample, BUFFERSIZE, nChannels);
        beat.audioIn(sample, BUFFERSIZE, nChannels);
        bands.audioIn(sample, BUFFERSIZE, nChannels);
        
        //---------------------------------------
        float numerator = 1;
        float denom = 0;
        for(int m = 0; m < 40; m++){
            numerator *= pow(bands.energies[m], 1.0/40.0);
            denom += bands.energies[m] / 40.0;
        }
        float sfmIntensity = log(numerator) - log(denom);
        //Measuring the ‘complexity’ of sound. (Singh 2011)
        //---------------------------------------
        
        //-----Running average of SFM Complexity:
        float avgIntensity = 0;
        if(i>0)
            avgIntensity = sfmIntensity * 0.48 + frameData[i-1].intensity*0.52;
        else
            avgIntensity = sfmIntensity;
        //---------------------------------------
        
        frameData.push_back({
            beat.received(),
            beat.bpm,
            (onset1.received() << 2) | (onset2.received() << 1) | (onset3.received()),
                //^ 0 to 7. [000] - [111]
            onset1.novelty, onset1.thresholdedNovelty, //thresholded stuff.
            pitch.latestPitch,
            pitch.pitchConfidence,
            isnan(avgIntensity)?-1:avgIntensity
        });
    }
    
    
    //Pitch Smoothing.
    //================
    std::vector<int> f(frameData.size());
    
    int numFrames = frameData.size();
    
    for(int i = 0; i < numFrames; i++){
        f[i] = ((int)(round(frameData[i].pitch)));
        frameData[i].intensity = pow(10, frameData[i].intensity);
        if(i < numFrames - 3 && frameData[i].onsets > 0){
            frameData[i].onsets = frameData[i].onsets | frameData[i+1].onsets | frameData[i+2].onsets | frameData[i+1].onsets;
            //collapse multiple onsets into the first one.
        }
        if(i>3 && frameData[i-1].onsets + frameData[i-2].onsets + frameData[i-3].onsets > 0){
            frameData[i].onsets = 0;
            //remove onset information if it occurs already.
        }
    }
//    
//    int fOut[frameData.size()];
//    median_filter_impl_1d(frameData.size(), 31, 70, &f[0], &fOut[0]);
//    
//    for(int i = 0; i < frameData.size(); i++){
//        frameData[i].pitch = fOut[i];
//    }
//    
    //Spawn Point Analysis.
    //???????????
    
    
    
}

/**
 Converts the Track Data frame to its string representation. Expensive operation, debug purpose only.
 @param track
 @returns string output
 */
string Track::toString(const Track::Data &track){
    stringstream s;
    s << track.onBeat << "\t" << track.bpm << "\t" << track.onsets << "\t" << track.pitch << "\t" << track.pitchConfidence << "\t" << track.onsetNovelty << "\t" << track.onsetThresholdedNovelty << "\t" << track.intensity;
    return s.str();
}


/**
 Reads the Data at the given frame, cumulatively from the last frame we called readData on.
 @param frame the frameID to read up to.
 @returns returns a Data pointer.
 */
Track::Data* Track::readData(int frame){
    Data* out = new Data(frameData[frame]);
    for(int i=lastFrameRead+1; i <= frame; i++){
        out->onBeat |= frameData[i].onBeat;
        out->onsets = (out->onsets | frameData[i].onsets);
    }
    lastFrameRead = frame;
    return out;
}
