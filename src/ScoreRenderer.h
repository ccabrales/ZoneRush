#pragma once

#include "ofxAssets.h"

class ScoreRenderer{
public:
    void update(int currentScore);
    void draw(float x, float y);
private:
    int curr = 0;
};

