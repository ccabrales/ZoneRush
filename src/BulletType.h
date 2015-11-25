#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "ofxParticles.h"
#include "ofxAssets.h"

enum FirePattern{
    STRAIGHT, THREESHOT, TWOSHOT, CLOUD
};

enum TargetPattern{
    PLAYER, DOWNLANE
};

struct BulletType {
    ofImage* texture;
    FirePattern firePattern;
    TargetPattern targetPattern;
};


class BulletLibrary
{
public:
    static const BulletType* getWeaponInfo(int grade);
    vector<BulletType> bullets;
private:
    BulletLibrary();
};