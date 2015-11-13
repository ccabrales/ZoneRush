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
    bool isPlaySelected();
    
private:
    ofImage title;
    ofPoint titlePos;
    
    ofImage playButton;
    ofPoint playPos;
    
    ofImage exitButton;
    ofPoint exitPos;
    
    int selectedIndex; //Keeps track of which button is currently selected
};