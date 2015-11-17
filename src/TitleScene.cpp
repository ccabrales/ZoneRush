#include "TitleScene.h"

void TitleScene::setup(){
    title.load("ZoneRush2.png");
    playButton.load("PlaySelected.png");
    exitButton.load("Exit.png");
    loadingImage.load("Loading.png");
    resetPosition();
    
    selectedIndex = 0;
    loadState = TITLE;
    imageDx = -20.0;
    
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
    
    logoEmitter.setPosition(ofVec3f(ofGetWidth()-1, titlePos.y + 50));
    logoEmitter.posSpread = ofVec3f(0, -60);
    logoEmitter.setVelocity(ofVec3f(-2810,0.0));

    logoEmitter.velSpread = ofVec3f(520,20);
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
    loadingPos = ofPoint(ofGetWidth(), (ofGetHeight() / 2.0) - loadingImage.getHeight());
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

void TitleScene::backgroundUpdate(const Track::Data* data, ofxParticleSystem* particleSystem){
    if (loadState == TRANSITION) { //update
        titlePos.x += imageDx;
        playPos.x += imageDx;
        exitPos.x += imageDx;
        loadingPos.x += imageDx;
        
        if (loadingPos.x <= ((ofGetWidth() / 2.0) - (loadingImage.getWidth() / 2.0)) ) {
            loadState = LOAD; //finished transition
        }
    } else if (loadState == TOGAME) {
        loadingPos.x += imageDx;
        if (loadingPos.x <= (-loadingImage.getWidth())) loadState = END;
    }
    
    rightEmitter.numPars = max((int)(-data->intensity+1) + (data->onBeat?12:0), 2);
    rightEmitter.setVelocity(data->onBeat?ofVec3f(-510,0.0):ofVec3f(-310,0.0));

    particleSystem->addParticles(logoEmitter);
    particleSystem->addParticles(rightEmitter);
}

void TitleScene::draw(){
    ofPushStyle();
    
    if (loadState == TITLE || loadState == TRANSITION) {
        title.draw(titlePos);
        playButton.draw(playPos);
        exitButton.draw(exitPos);
    }
    
    if (loadState != TITLE || loadState != END) loadingImage.draw(loadingPos);
    
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
void TitleScene::setLoading(LoadState state) {
    loadState = state;
    if (loadState == TITLE) resetPosition();
}

TitleScene::LoadState TitleScene::getCurrentState() {
    return loadState;
}




