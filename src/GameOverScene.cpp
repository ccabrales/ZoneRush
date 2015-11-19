#include "GameOverScene.h"


void GameOverScene::setup(){
    gameOverText.load("GameOver.png");
    score.load("Score.png");
    songPercent.load("SongPercentage.png");
    resetPosition();
    
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

//TODO: get actual distances for how long the score and songPercentage text will be after these images to make exactly center
void GameOverScene::resetPosition() {
    textPos = ofPoint((ofGetWidth() / 2.0) - (gameOverText.getWidth() / 2.0), ofGetHeight() / 5.0);
    scorePos = ofPoint((ofGetWidth() / 2.0) - (score.getWidth() / 2.0), 3 * ofGetHeight() / 5.0);
    songPos = ofPoint((ofGetWidth() / 2.0) - (songPercent.getWidth() / 2.0), scorePos.y+score.getHeight() + 10.0);
}

void GameOverScene::update(){
    
}

void GameOverScene::backgroundUpdate(const Track::Data* data, ofxParticleSystem* particleSystem){
    rightEmitter.numPars = max((int)(-data->intensity+1) + (data->onBeat?12:0), 2);
    rightEmitter.setVelocity(data->onBeat?ofVec3f(-510,0.0):ofVec3f(-310,0.0));
    
    particleSystem->addParticles(rightEmitter);
}


void GameOverScene::draw(){
    ofPushStyle();
    gameOverText.draw(textPos);
    score.draw(scorePos);
    songPercent.draw(songPos);
    ofPopStyle();
}

void GameOverScene::willFadeOut() {
    
}

void GameOverScene::willExit(){
    
}

void GameOverScene::windowResized(int w, int h) {
    
}