#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "BulletType.h"
#include "track.h"
#include "EnemyPath.h"
#include "globals.h"
#include "ofxParticles.h"

struct EnemyType {
    int minHP;
    int maxHP;
    const BulletType* bulletType;
    float speed;
    //movement:
    bool followPlayer;
    ofImage* texture;
};

enum EnemyState {
    HEALTHY, DYING, DEAD
};

class Enemy : public ofxParticle{
public:
//    ofPoint pos;
    int hp;
    float cd;
    float difficultyScaling;
    
    EnemyType* type;
    EnemyState state;
//    ofPolyline path;
    
    void setup(float diffScaling);
    void update(const float timeStep, const float drag, ofxParticleSystem* bulletSpace);
    void draw();
    void shoot();
    
    void onsetHandler(const Track::Data& frame);
//    void calculate_movement(const ofPolyline* archetype);
    
private:
};

typedef shared_ptr<Enemy> EnemyPtr;

class EnemyFactory
{
public:
    static EnemyPtr make(int typeID);
    
    static vector< EnemyPtr >* makeGroup(int type, int size, float variance);
private:
    vector<EnemyType> e_types;
    EnemyFactory();
};





