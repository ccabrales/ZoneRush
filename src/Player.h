#pragma once
#include "ofMain.h"
#include "ofxParticles.h"

class Player {
public:
    ofPoint pos;
    float width, height, speed;
    int lives;
    
    bool is_left_pressed, is_right_pressed, is_down_pressed, is_up_pressed;
    
    void setup(ofImage * _img);
    void update();
    void draw();
    void shoot();
    
    void calculate_movement();
    void checkBounds();
    void moveHitbox();
    
//    bool check_can_shoot();
    
    ofImage * img;
    
    ofxParticleEmitter emitter;
    
    ofRectangle hitbox;
    
    int bulletDamage;
    
};