#pragma once
#include "ofMain.h"


enum SoundLibrary{
    ACCELERATOR_WINDUP, ALIENHIT, POWERDOWN, EXPLODE_L, HEARTBEAT, 
};

class BulletLibrary
{
public:
    static const BulletType* getWeaponInfo(int grade);
    vector<BulletType> bullets;
private:
    BulletLibrary();
};
