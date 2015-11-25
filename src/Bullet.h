#pragma once

#include <stdio.h>
#include "BulletType.h"
#include "ofMain.h"
#include "ofxParticles.h"

class Bullet : public ofxParticle {
public:
    ofImage * tex;
    void draw();
};
