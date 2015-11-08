#pragma once

#include <stdio.h>
#include "ofMain.h"

struct BulletType {
    ofVec2f aim;
    float angle; //radians or degrees tbd.
    ofImage img;
};

class BulletLibrary{
public:
    static BulletType b;
};
