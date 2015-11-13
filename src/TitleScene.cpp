#include "TitleScene.h"


void TitleScene::setup(){
    ofSetEscapeQuitsApp(false);
    title.load("ZoneRush.png");
    titlePos = ofPoint((ofGetWidth() / 2.0) - (title.getWidth() / 2.0), ofGetHeight() / 4.0);
}

void TitleScene::update(){
    
}

void TitleScene::draw(){
    ofBackground(0, 0, 0);
    title.draw(titlePos);
}

void TitleScene::willFadeOut() {
    
}

void TitleScene::willExit(){
    
}

void TitleScene::windowResized(int w, int h) {
    titlePos = ofPoint((w / 2.0) - (title.getWidth() / 2.0), h / 4.0);
}