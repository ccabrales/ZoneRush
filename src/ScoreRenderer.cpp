#include "ScoreRenderer.h"

void ScoreRenderer::update(int currentScore){
    curr += (((float)(currentScore - curr))*0.6);
}

void ScoreRenderer::draw(float x, float y){
    ofxAssets::font("welbut", 12).drawString(ofToString(curr), x, y);
}