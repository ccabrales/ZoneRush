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
    float cd;
    int damage;
    ofImage* texture;
    FirePattern firePattern;
    TargetPattern targetPattern;
};


class BulletLibrary
{
public:
    static const BulletType* getWeaponInfo(int grade, bool player);
    vector<BulletType> bullets;
    vector<BulletType> playerBullets;
private:
    BulletLibrary();
};