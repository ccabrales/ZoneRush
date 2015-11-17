#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "TitleButtons.h"

#include "ofxParticles.h"
#include "track.h"
#include "globals.h"


class TitleScene {
public:
    enum LoadState { TITLE, TRANSITION, LOAD, TOGAME, END };
    void setup();
    void update();
    void backgroundUpdate(const Track::Data* data,     ofxParticleSystem* particleSystem);
    void draw();
    
    //event handling implementations.
//    void willFadeIn();
//    void willDraw();
    void willFadeOut();
    void willExit();
    void windowResized(int w, int h);
    bool isPlaySelected();
    void setLoading(LoadState state);
    LoadState getCurrentState();
    
private:
    ofImage title;
    ofPoint titlePos;
    
    ofImage playButton;
    ofPoint playPos;
    
    ofImage exitButton;
    ofPoint exitPos;
    
    ofImage loadingImage;
    ofPoint loadingPos;
    
    int selectedIndex; //Keeps track of which button is currently selected
    
    ofxParticleEmitter logoEmitter, rightEmitter;
    
    float imageDx; //Used in transitions for moving text
    
    LoadState loadState;
    
    void resetPosition();
};