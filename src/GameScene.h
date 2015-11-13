

#pragma once

#include <stdio.h>
#include "ofMain.h"

class GameScene {
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