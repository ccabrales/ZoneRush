#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "BulletType.h"
#include "track.h"
#include "EnemyPath.h"
#include "globals.h"


struct EnemyType {
    int minHP;
    int maxHP;
    const BulletType* bulletType;
    int speed;
    ofImage* img;
};

enum EnemyState {
    HEALTHY, DYING, DEAD
};

class Enemy {
public:
    int spawnTime;
    
    ofPoint pos;
    int hp;
    int cd;
    
    EnemyType* type;
    EnemyState state;
    ofPolyline path;
    
    void setup();
    void update();
    void draw();
    void shoot();
    
    void onsetHandler(const Track::Data& frame);
    void calculate_movement(const ofPolyline* archetype);
    
private:
};

typedef shared_ptr<Enemy> EnemyPtr;

class EnemyFactory
{
public:
    static EnemyPtr make(int type);
    
    static vector< EnemyPtr >* makeGroup(int type, int size, float variance);
private:
    vector<EnemyType> e_types;
    EnemyFactory();
};





