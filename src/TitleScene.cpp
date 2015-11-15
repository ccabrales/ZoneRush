#include "TitleScene.h"

void TitleScene::setup(){
    title.load("ZoneRush2.png");
    playButton.load("PlaySelected.png");
    exitButton.load("Exit.png");
    resetPosition();
    
    selectedIndex = 0;
    isLoading = false;
    
    rightEmitter.setPosition(ofVec3f(ofGetWidth()-1,ofGetHeight()/2.0));
    rightEmitter.setVelocity(ofVec3f(-310,0.0));
    rightEmitter.posSpread = ofVec3f(0,ofGetHeight());
    rightEmitter.velSpread = ofVec3f(120,20);
    rightEmitter.life = 40;
    rightEmitter.lifeSpread = 10;
    rightEmitter.numPars = 3;
    rightEmitter.size = 12;
    rightEmitter.color = ofColor(100,100,200);
    rightEmitter.colorSpread = ofColor(70,70,70);
    
    logoEmitter.setPosition(ofVec3f(ofGetWidth()-1, titlePos.y + 50));
    logoEmitter.posSpread = ofVec3f(0, -60);
    logoEmitter.setVelocity(ofVec3f(-1810,0.0));

    logoEmitter.velSpread = ofVec3f(320,20);
    logoEmitter.life = 4;
    logoEmitter.lifeSpread = 3;
    logoEmitter.numPars = 2;
//    logoEmitter.size = 12;
    logoEmitter.color = ofColor(140,140,220);
    logoEmitter.colorSpread = ofColor(70,70,70);

    
}


void TitleScene::resetPosition(){
    playPos = ofPoint((ofGetWidth() / 2.0) - (playButton.getWidth() / 2.0), 3 * ofGetHeight() / 5.0);
    exitPos = ofPoint((ofGetWidth() / 2.0) - (exitButton.getWidth() / 2.0), playPos.y+playButton.getHeight() + 10.0);
    titlePos = ofPoint((ofGetWidth() / 2.0) - (title.getWidth() / 2.0), ofGetHeight() / 5.0);
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
    
    rightEmitter.numPars = max((int)(-data->intensity+1) + (data->onBeat?12:0), 2);
    rightEmitter.setVelocity(data->onBeat?ofVec3f(-510,0.0):ofVec3f(-310,0.0));

    particleSystem.addParticles(logoEmitter);
    particleSystem.addParticles(rightEmitter);
    
//    cout << particleSystem.getNumParticles() << endl;
}

void TitleScene::draw(){
    ofPushStyle();
    ofBackground(0, 0, 0);
    ofPushStyle();
    ofSetLineWidth(3.0);
    particleSystem.draw();
    ofPopStyle();

    if (!isLoading) {
        title.draw(titlePos);
        playButton.draw(playPos);
        exitButton.draw(exitPos);
    }
    
    ofPopStyle();
}

void TitleScene::willFadeOut() {
    
}

void TitleScene::willExit(){
    
}

void TitleScene::windowResized(int w, int h) {
    resetPosition();
}

bool TitleScene::isPlaySelected() {
    return selectedIndex == 0;
}

//Toggle the variable flag
void TitleScene::setLoading() {
    isLoading = !isLoading;
}