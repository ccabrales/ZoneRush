#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "BulletType.h"
#include "track.h"
#include "globals.h"
#include "ofxParticles.h"
#include "player.h"
#include "ofxAssets.h"
#include "SoundLibrary.h"

#define BulletSpeed 150


struct EnemyType {
    int minHP;
    int maxHP;
    int score;
    const BulletType* bulletType;
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
    ofxParticleEmitter gun;

    ofRectangle hitbox;
    
    float laserChargeTimer;
    float laserWidth = 80;
    ofPoint laserTargetPoint;
    bool laserCharging = false;
    bool laserFiring = false;

    void setup(float diffScaling);
    void update(const float timeStep, const float drag, ofxParticleSystem* bulletSpace, ofxParticleSystem* explosionSystem, bool onBeat, int* score);
    void fire(ofxParticleSystem* bulletSpace);
    //Deprecated Draw. uses ofxParticleSystem::draw(tex) instead.
    void draw();
    void draw(ofTexture &tex);

    void onsetHandler(const Track::Data& frame);
    void moveHitbox();
//    void calculate_movement(const ofPolyline* archetype);

private:
    void fireLaser();
};


typedef shared_ptr<Enemy> EnemyPtr;

class EnemyFactory
{
public:
    static EnemyPtr make(int typeID, float difficultyScaling);
    static EnemyType* getType(int typeID);
    static EnemyType* getTypeRandom();
    static vector< EnemyPtr >* makeGroup(int type, int size, float variance, float difficultyScaling);
private:
    vector<EnemyType> e_types;
    EnemyFactory();
};


class EnemySystem : public ofxParticleSystem{
public:
    int update(float timeStep, ofxParticleSystem* bulletSystem, ofxParticleSystem * explosionSystem, const Track::Data * data, int* score);
    void draw();
//    void draw();
};


