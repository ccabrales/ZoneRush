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
#include "ofxFatLine.h"
#include "ofxGifDecoder.h"

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
    float laserWidth = 40;
    ofPoint laserTargetPoint;
    ofxFatLine laser;
    bool laserCharging = false;
    bool laserFiring = false;
    ofVec3f oldVel;

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


enum BossState{
    BossEntering, BossFight, BossDestroyed
};

enum BossFiringState{
    BEverythingGoes, BChill, BLasers, BBullets
};


class Boss : public Enemy{
public:
    
    BossState state;
    BossFiringState firemode;
    
    ofxGifDecoder decoder;
    ofxGifFile bossGif;

    
    float enteringDone=0;
    float enterDuration=2.0;
    
    
    float laserTick;
    float countDown;
    
    EnemySystem* internalEnemies;
    
    void setup();
    void update(const float timeStep, ofxParticleSystem* bulletSpace, ofxParticleSystem* explosionSystem, EnemySystem* globalEnemies, const Track::Data* data, int* score);
    void draw();
    
private:
    void newFiringState();
};


