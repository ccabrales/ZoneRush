#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "TitleButtons.h"


class TitleScene {
public:
    void setup();
    void update();
    void draw();
    
    //event handling implementations.
//    void willFadeIn();
//    void willDraw();
    void willFadeOut();
    void willExit();
    void windowResized(int w, int h);
    
private:
    ofImage title;
    ofPoint titlePos;
    PlayButton play;
};