#pragma once

#include "ofMain.h"
#include "ofxAubio.h"
#include "ofxGui.h"
#include "globals.h"
#include "ofxAudioDecoder.h"
#include "track.h"
#include "player.h"
#include "enemy.h"
#include "TrackViewUtil.h"
#include "GameEvent.h"
#include "enemy.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "ofxPostProcessing.h"
#include "AudioLoader.h"
#include "ofxAssets.h"

class GreedyParticleSystem: public ofxParticleSystem{
public:
    int update(float timeStep, float drag);
};

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
    void checkLoadUpdate();
    
    void audioOut(float * input, int bufferSize, int nChannels);
    void audioIn(float * input, int bufferSize, int nChannels);
    
private:
    enum GameState { START, LOAD, GAME, END };
    
    TrackView tv;
    
    GameState game_state;
    
    vector<EnemyPtr> enemyList;
    
    TitleScene * titleScene;
    GameScene * gameScene;
    GameOverScene * gameOverScene;
    
    ofxPostProcessing post;
    
    unique_ptr<AudioLoader> audioLoader;
    
    GreedyParticleSystem backgroundParticles;

    //Events:
};
