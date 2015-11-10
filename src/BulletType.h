#pragma once

#include <stdio.h>
#include "ofMain.h"

struct BulletType {
    ofVec2f aim;
    float angle; //radians or degrees tbd.
    ofImage* img;
};

static struct BulletLibrary {
    static BulletType StraightShot = (BulletType){ ofVec2f(-1.0, 0.0), 0, new ofImage() };
//    static BulletType 
    
};
