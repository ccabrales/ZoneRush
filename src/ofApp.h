#pragma once

#include "ofMain.h"
#include "ofxAubio.h"
#include "ofxGui.h"
#include "ofxAudioDecoder.h"
#include "track.h"
#include "player.h"

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
        enum GameState { START, GAME, END };
    
        int tick;
        bool useMic = false;
    
        
    
        Track* currentTrack;
        float onsetDecay[3];
        float beatDecay = 0;
        ofxAudioDecoder musicDecoder;
        ofxAubioMelBands bands;

        ofPolyline bandPlot;
        ofPolyline pitchPlot;
        ofPolyline intensityPlot;
    
        Player player;
        GameState game_state;

};
