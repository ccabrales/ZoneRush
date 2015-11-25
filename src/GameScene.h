

#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "ofxParticles.h"
#include "track.h"
#include "ofxAssets.h"
#include "Player.h"
#include "Enemy.h"
#include "ScoreRenderer.h"

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
    
    Player player;
    int score;
    
private:
    void checkPlayerHit();
    void checkEnemyHits();
    
    ofxParticleEmitter rightEmitter;
    
    ofxParticleEmitter enemyEmitter;
    ofxParticleEmitter playerEmitter;
    vector<EnemyPtr> enemyList;
    
    ScoreRenderer scoreRender;
};