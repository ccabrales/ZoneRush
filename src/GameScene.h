

#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "ofxParticles.h"
#include "track.h"
#include "ofxAssets.h"
#include "Player.h"
#include "Enemy.h"
#include "ScoreRenderer.h"
#include "globals.h"

class GameScene {
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
    
    int score;
    
private:
    ofxParticleEmitter rightEmitter;
    
    vector<EnemyPtr> enemyList;
    
    ScoreRenderer scoreRender;
    
    ofxParticleSystem enemyBullets;
    EnemySystem enemies;
    ofxParticleSystem playerBullets;

};