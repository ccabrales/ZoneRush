#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "ofxParticles.h"

struct BulletType {
    ofImage* texture;
};

enum FirePattern{
    STRAIGHT, THREESHOT, TWOSHOT, CLOUD
};

class BulletLibrary
{
public:
    static const BulletType* getWeaponInfo(int grade);
    
    vector<BulletType> bullets;

private:
    BulletLibrary();
};