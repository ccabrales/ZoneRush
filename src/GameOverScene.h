#pragma once

#include <stdio.h>
#include "ofxParticles.h"
#include "track.h"

class GameOverScene {
public:
    void setup();
    void update();
    void draw();
    
    //event handling implementations.
    //    void willFadeIn();
    //    void willDraw();
    void willFadeOut();
    void willExit();
    void windowResized(int w, int h);
    void backgroundUpdate(const Track::Data* data, ofxParticleSystem* particleSystem);
    
private:
    ofxParticleEmitter rightEmitter;

};