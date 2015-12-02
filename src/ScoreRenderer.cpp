#include "ScoreRenderer.h"

void ScoreRenderer::update(int currentScore){
//    curr += (((float)(currentScore - curr))*0.6);
    curr = currentScore;
}

void ScoreRenderer::draw(float x, float y){
    ofxAssets::font("welbut", 12).drawString(ofToString(curr), x, y);
}

void ScoreRenderer::drawFinal(ofPoint loc) {
    ofxAssets::font("welbut", 36).drawString(ofToString(curr), loc.x, loc.y);
}

void ScoreRenderer::drawText(string text, ofPoint loc) {
    ofxAssets::font("welbut", 36).drawString(ofToString(text), loc.x, loc.y);
}