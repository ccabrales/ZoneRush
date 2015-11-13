#pragma once

#include <stdio.h>
#include "ofxMSAInteractiveObject.h"

class PlayButton: public ofxMSAInteractiveObject {
public:
    void setup();
    void exit();
    void update();
    void draw();
    virtual void onRollOver(int x, int y);
    virtual void onRollOut();
    virtual void onPress(int x, int y, int button);
    virtual void onRelease(int x, int y, int button);
    virtual void keyPressed(int key);
    virtual void keyReleased(int key);
};
