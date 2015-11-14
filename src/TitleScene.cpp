#include "TitleScene.h"

void TitleScene::setup(){
    title.load("ZoneRush2.png");
    titlePos = ofPoint((ofGetWidth() / 2.0) - (title.getWidth() / 2.0), ofGetHeight() / 5.0);
    
    playButton.load("PlaySelected.png");
    playPos = ofPoint((ofGetWidth() / 2.0) - (playButton.getWidth() / 2.0), 3 * ofGetHeight() / 5.0);
    
    exitButton.load("Exit.png");
    exitPos = ofPoint((ofGetWidth() / 2.0) - (exitButton.getWidth() / 2.0), playPos.y+playButton.getHeight() + 10.0);
    
    selectedIndex = 0;
    
    rightEmitter.setPosition(ofVec3f(ofGetWidth()-1,ofGetHeight()/2.0));
    rightEmitter.setVelocity(ofVec3f(-310,0.0));
    rightEmitter.posSpread = ofVec3f(0,ofGetHeight());
    rightEmitter.velSpread = ofVec3f(120,20);
    rightEmitter.life = 50;
    rightEmitter.lifeSpread = 10;
    rightEmitter.numPars = 3;
    rightEmitter.size = 12;
    rightEmitter.color = ofColor(100,100,200);
    rightEmitter.colorSpread = ofColor(70,70,70);
    
}

void TitleScene::update(){
    selectedIndex = 1 - selectedIndex; //Swap between 0 and 1 for the selected index
    switch (selectedIndex) {
        case 0:
            playButton.load("PlaySelected.png");
            exitButton.load("Exit.png");
            break;
        case 1:
            playButton.load("Play.png");
            exitButton.load("ExitSelected.png");
            break;
        default:
            break;
    }
    
}

void TitleScene::backgroundUpdate(const Track::Data* data){
    particleSystem.update(min(ofGetLastFrameTime(), 1.0/10.0), 1);
    
    rightEmitter.numPars = max((int)(-data->intensity) + (data->onBeat?12:0), 2);
    rightEmitter.setVelocity(data->onBeat?ofVec3f(-510,0.0):ofVec3f(-310,0.0));

    
    particleSystem.addParticles(rightEmitter);
    
    cout << particleSystem.getNumParticles() << endl;
}

void TitleScene::draw(){
    ofPushStyle();
    ofBackground(0, 0, 0);
    ofPushStyle();
    ofSetLineWidth(3.0);
    particleSystem.draw();
    ofPopStyle();

    title.draw(titlePos);
    playButton.draw(playPos);
    exitButton.draw(exitPos);

    ofNoFill();
    ofSetCircleResolution(180);
    ofSetColor(255, 0, 0, 50);
//    ofCircle(ofGetWidth()/2, ofGetHeight()/2, sqrt(gravAcc));
    ofSetColor(0, 0, 255, 50);
//    ofCircle(ofGetWidth()/2, ofGetHeight()/2, sqrt(rotAcc));

    ofPopStyle();
}

void TitleScene::willFadeOut() {
    
}

void TitleScene::willExit(){
    
}

void TitleScene::windowResized(int w, int h) {
    titlePos = ofPoint((w / 2.0) - (title.getWidth() / 2.0), h / 5.0);
}

bool TitleScene::isPlaySelected() {
    return selectedIndex == 0;
}