#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "ofxScene.h"


class TitleScene: public ofxScene{
public:
    void setup();
    void update();
    void draw();
    
    //event handling implementations.
//    void willFadeIn();
//    void willDraw();
    void willFadeOut();
    void willExit();
};