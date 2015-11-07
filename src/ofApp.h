#pragma once

#include "ofMain.h"
#include "ofxAubio.h"
#include "ofxGui.h"
#include "ofxAudioDecoder.h"
#include "track.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void exit();

        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);

        void audioOut(float * input, int bufferSize, int nChannels);
        void audioIn(float * input, int bufferSize, int nChannels);

    private:
        int tick;
        bool useMic = false;
    
        Track* currentTrack;
        float onsetDecay[3];
        float beatDecay = 0;
        ofxAudioDecoder musicDecoder;

        ofxAubioOnset onset;
        ofxAubioPitch pitch;
        ofxAubioBeat beat;
        ofxAubioMelBands bands;

//        ofxPanel pitchGui;
//        ofxFloatSlider midiPitch;
//        ofxFloatSlider pitchConfidence;
//
//        ofxPanel beatGui;
//        ofxFloatSlider bpm;

//        ofxPanel onsetGui;
//        ofxFloatSlider gotOnset;
//        ofxFloatSlider onsetThreshold;
//        ofxFloatSlider onsetNovelty;
//        ofxFloatSlider onsetThresholdedNovelty;

//        ofxPanel bandsGui;
        ofPolyline bandPlot;
        ofPolyline pitchPlot;

};
