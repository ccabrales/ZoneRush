#include "GameOverScene.h"


void GameOverScene::setup(){
    rightEmitter.setPosition(ofVec3f(ofGetWidth()-1,ofGetHeight()/2.0));
    rightEmitter.setVelocity(ofVec3f(-310,0.0));
    rightEmitter.posSpread = ofVec3f(0,ofGetHeight());
    rightEmitter.velSpread = ofVec3f(120,20);
    rightEmitter.life = 13;
    rightEmitter.lifeSpread = 0;
    rightEmitter.numPars = 3;
    rightEmitter.size = 12;
    rightEmitter.color = ofColor(100,100,200);
    rightEmitter.colorSpread = ofColor(70,70,70);
}

void GameOverScene::update(){
    
}

void GameOverScene::backgroundUpdate(const Track::Data* data, ofxParticleSystem* particleSystem){
    rightEmitter.numPars = max((int)(-data->intensity+1) + (data->onBeat?12:0), 2);
    rightEmitter.setVelocity(data->onBeat?ofVec3f(-510,0.0):ofVec3f(-310,0.0));
    
    particleSystem->addParticles(rightEmitter);
}


void GameOverScene::draw(){
    
}

void GameOverScene::willFadeOut() {
    
}

void GameOverScene::willExit(){
    
}

void GameOverScene::windowResized(int w, int h) {
    
}