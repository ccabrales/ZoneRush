#pragma once
#include "ofMain.h"
#include "ofxParticles.h"
#include "BulletType.h"
#include "Enemy.h"

class Player {
public:
    ofPoint pos;
    float width, height, speed;
    int lives;

    bool is_left_pressed, is_right_pressed, is_down_pressed, is_up_pressed;

    void setup(ofImage * _img);
    void update(float frameTime);
    void upgradeGun(int grade);
    void draw();
    void shoot(GreedyParticleSystem* playerBullet);

    void checkBounds();
    void moveHitbox();

    ofImage * img;

    ofxParticleEmitter emitter;
    ofxParticleEmitter gunEmitter;

    ofRectangle hitbox;

    const BulletType* currentGun;

    int bulletDamage;
    float bulletCd;


};
