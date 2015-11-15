#pragma once

//#include <stdio.h>
#include "ofMain.h"
#include "ofxAudioDecoder.h"
#include "track.h"

class AudioLoader:public ofThread{
public:
    void start(ofFileDialogResult res);

    std::unique_ptr<Track> loadedTrack;
    std::unique_ptr<ofxAudioDecoder> tempDecoder;
    
    bool isDone = false;
    bool hasError = false;
    ofFileDialogResult result;
    string errorMsg;
    
protected:
    
    void threadedFunction();
    void createError(string msg);
    void setTrack(Track* newTrack);
    void setDecoder(ofxAudioDecoder* newDecoder);
    bool convertFile(string& filePath);
    
};