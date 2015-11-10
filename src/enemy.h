#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "BulletType.h"
#include "track.h"

struct EnemyType {
    int minHP;
    int maxHP;
    const BulletType* bulletType;
    ofImage* img;
};

enum EnemyState {
    HEALTHY, DYING, DEAD
};

class Enemy {
public:
    int tick;
    
    ofPoint pos;
    int hp;
    int cd;
    
    EnemyType* type;
    
    void setup();
    void update();
    void draw();
    void shoot();
    
    void onsetHandler(Track::Data& frame);
    
    void calculate_movement();
    
    //    bool check_can_shoot();
private:
};


class EnemyFactory
{
public:
    static Enemy make(int type){
        static EnemyFactory inst;
        Enemy e = Enemy();
        e.type = &(inst.e_types[type]);
        e.setup();
        return e;
    };
    
    
private:
    vector<EnemyType> e_types;

    EnemyFactory(){
        e_types.push_back(EnemyType {
            4, 5, BulletLibrary::getWeaponInfo(0), new ofImage()
        });
        e_types.push_back(EnemyType {
            4, 7, BulletLibrary::getWeaponInfo(0), new ofImage()
        });

    };
};





