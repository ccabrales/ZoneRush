#pragma once

#include "ofMain.h"
#include "ofxAubio.h"
#include "ofxGui.h"
#include "ofxAudioDecoder.h"
#include "track.h"
#include "player.h"
#include "enemy.h"
#include "TrackViewUtil.h"
#include "GameEvent.h"
#include "globals.h"
#include "enemy.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ofxPostProcessing.h"

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
    bool checkFileExtension(ofFileDialogResult res);
    void convertFileAndReload(string filePath);
    
    enum GameState { START, LOADING, GAME, END };
    
    TrackView tv;
    ofxAudioDecoder musicDecoder;
    
    Player player;
    GameState game_state;
    
    ofImage player_image;

    vector<EnemyPtr> enemyList;
    
    TitleScene * titleScene;
    GameScene * gameScene;
    
    ofxPostProcessing post;
    
    vector<string> acceptableFileExts {"mp3", "m4a", "wav"};
    vector<string> convertFileExts {"aiff", "aif", "flac"};
    //Events:
};
