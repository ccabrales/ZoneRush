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
#include "ScoreRenderer.h"
#include "SoundLibrary.h"


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
    //State machine
    enum GameState { START, LOAD, GAME, END };
    GameState game_state;
//    vector<EnemyPtr> enemyList;

    //Scenes
    TitleScene * titleScene;
    GameScene * gameScene;
    GameOverScene * gameOverScene;

    //Rendering utility
    TrackView tv;
    ofxPostProcessing post;
    shared_ptr<DofPass> pass;
    ScoreRenderer score;

    //AudioSystem
    unique_ptr<AudioLoader> audioLoader;
    Track introTrack;
    ofxAudioDecoder introDecoder;
    
    //puts audio into globalDecoder


    ofxParticleEmitter cloudEmitter;
    GreedyParticleSystem backgroundParticles;
    GreedyParticleSystem backgroundClouds;
    //Events:

    //----------UTILITY FUNCS-----------
    void setupParticleEmitters();
    void setupPostProcessing();
};
