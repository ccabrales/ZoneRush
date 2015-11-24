#pragma once

#include <stdio.h>
#include "ofxParticles.h"
#include "track.h"
#include "ScoreRenderer.h"

class GameOverScene {
public:
    void setup(int endScore, int finalPercentage);
    void update();
    void draw();
    
    //event handling implementations.
    //    void willFadeIn();
    //    void willDraw();
    void willFadeOut();
    void willExit();
    void windowResized(int w, int h);
    void backgroundUpdate(const Track::Data* data, ofxParticleSystem* particleSystem);
    void resetPosition();
    
private:
    ofxParticleEmitter rightEmitter;

    ofImage gameOverText;
    ofPoint textPos;
    
    ofImage score;
    ofPoint scorePos;
    
    ofImage songPercent;
    ofPoint songPos;
    
    ofImage menu;
    ofPoint menuPos;
    
    ScoreRenderer scoreRender;
    int finalScore;
    ofPoint finalScorePos;
    
    ofPoint percentPos;
    string percentage;
};