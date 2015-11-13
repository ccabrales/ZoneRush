#include "TitleScene.h"


void TitleScene::setup(){
    title.load("ZoneRush2.png");
    titlePos = ofPoint((ofGetWidth() / 2.0) - (title.getWidth() / 2.0), ofGetHeight() / 5.0);
    
    playButton.load("PlaySelected.png");
    playPos = ofPoint((ofGetWidth() / 2.0) - (playButton.getWidth() / 2.0), 3 * ofGetHeight() / 5.0);
    
    exitButton.load("Exit.png");
    exitPos = ofPoint((ofGetWidth() / 2.0) - (exitButton.getWidth() / 2.0), playPos.y+playButton.getHeight() + 10.0);
    
    selectedIndex = 0;
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

void TitleScene::draw(){
    ofPushStyle();
    ofBackground(0, 0, 0);
    title.draw(titlePos);
    playButton.draw(playPos);
    exitButton.draw(exitPos);
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