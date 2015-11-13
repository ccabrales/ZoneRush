#pragma once

#include <stdio.h>
#include "ofMain.h"

struct BulletType {
    float speed;//speed in pixels per tick of the missile.
    float arc;  //angle in degrees to fire in. e.g. 10 = -5 to +5 deg.
    float num;  //number of bullets into this arc. 2 = line of two shots.
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