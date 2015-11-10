#pragma once

#include <stdio.h>
#include "ofMain.h"

struct BulletType {
    float speed;//speed in pixels per tick of the missile.
    float arc;  //angle in degrees to fire in. e.g. 10 = -5 to +5 deg.
    float num;  //number of bullets into this arc. 2 = line of two shots.
    ofImage* texture;
};


class BulletLibrary
{
public:
    static const BulletType* getWeaponInfo(int grade){
        static BulletLibrary inst;
        return &(inst.bullets[grade]);
    };
    
    vector<BulletType> bullets;

private:
    BulletLibrary(){
        bullets.push_back( BulletType {
            1.2, 0, 1., new ofImage()
        });
        bullets.push_back( BulletType {
            1.4, 10., 3., new ofImage()
        });
        bullets.push_back( BulletType {
            0.8, 7., 2., new ofImage()
        });
    };
    
    static BulletLibrary* instance;
};

