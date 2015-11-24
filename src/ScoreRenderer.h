#pragma once

#include "ofxAssets.h"

class ScoreRenderer{
public:
    void update(int currentScore);
    void draw(float x, float y);
    void drawFinal(ofPoint loc);
    void drawText(string text, ofPoint loc);
private:
    int curr = 0;
};

