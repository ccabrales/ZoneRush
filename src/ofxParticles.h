//
//  ofxParticles.h
//  assimpExample
//
//  Created by Timothy Scaffidi on 6/14/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//  Modified by Xiaonan Tong.


#pragma once
#include <list>

#include "ofMain.h"


inline ofVec3f ofRandVec3f();

class ofxParticle {
public:
    ofVec3f position;
    ofVec3f velocity; // pixels/sec
    ofVec3f acceleration; // pixels/sec/sec
    ofColor color;
    
    ofVec3f rotation;
    ofVec3f rotationalVelocity;
    
    float mass;
    float size;
    float life;
    float lifeStart;
    float dt;
    int particleID;
    
    
    bool operator < (const ofxParticle &b);
    
    ofxParticle();
    
    ofxParticle(ofVec3f pos, ofVec3f vel, float size_, float life_);
    
    ofxParticle(const ofxParticle &mom);
    
    ~ofxParticle(){}
    
    ofxParticle & operator=(const ofxParticle &mom);
    
    void constrainToRect(ofRectangle bounds, const float k, const float dist, const float drag);
    
    void applyForce(ofVec3f force);
    
    // void spring(ofPoint p, const float k, const float springDist, const float drag){ spring(p, k, springDist, drag); }
    float springBoth(ofxParticle * p, const float k, const float springDist, const float drag, const float sprintSnap);
    
    
    void attractTo(ofxParticle p, const float accel, const float minDist, const bool consumeParticle){ attractTo(p.position, accel, minDist, consumeParticle); }
    
    void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle);
    
    
    void gravitateTo(ofxParticle p, const float gravity, const float minDist, const bool consumeParticle){ gravitateTo(p.position, gravity, p.mass, minDist, consumeParticle); }
    
    void gravitateTo(ofPoint p, const float gravity, const float mass2, const float minDist, const bool consumeParticle);
    
    
    void gravitateBoth(ofxParticle * p, const float gravity, const float minDist, bool consumeParticle);
    
    void rotateAround(ofxParticle p, const float accel, const float minDist, const bool consumeParticle) { rotateAround(p.position, accel, minDist, consumeParticle); }
    
    void rotateAround(ofPoint p, const float accel, const float minDist, const bool consumeParticle);
    
    void update(const float timeStep, const float drag);
    
    void draw();
    
    void draw(ofTexture &tex);
    
    bool isAlive();
};

class ofxParticleEmitter {
public:
    ofxParticleEmitter();
    ~ofxParticleEmitter(){}
    ofVec3f positionStart;
    ofVec3f positionEnd;
    ofVec3f posSpread;
    ofVec3f velocityStart;
    ofVec3f velocityEnd;
    ofVec3f velSpread;
    ofVec3f rotation;
    ofVec3f rotSpread;
    ofVec3f rotVel;
    ofVec3f rotVelSpread;
    float size;
    float sizeSpread;
    float life;
    float lifeSpread;
    int numPars;
    ofColor color;
    ofColor colorSpread;
    
    ofxParticleEmitter & setPosition(ofVec3f pos);
    ofxParticleEmitter & setPosition(ofVec3f posStart, ofVec3f posEnd);
    
    ofxParticleEmitter & setVelocity(ofVec3f vel);
    ofxParticleEmitter & setVelocity(ofVec3f velStart, ofVec3f velEnd);
};

class ofxParticleSystem {
public:
    ofxParticleSystem(){
        numParticles = 0;
        totalParticlesEmitted = 0;
    }
    ~ofxParticleSystem(){}
    
    void addParticles(ofxParticleEmitter & src);
    
    void attractTo(ofPoint p, const float accel, const float minDist, const bool consumeParticle);
    
    void gravitateTo(ofPoint p, const float gravity, const float mass, const float minDist, const float consumeParticle);
    
    void rotateAround(ofPoint p, const float accel, const float minDist, const float consumeParticle);
    
    void applyVectorField(float * field, int fieldWidth, int fieldHeight, int numComponents, ofRectangle areaOfInfluence, float force);
    
    int update(float timeStep, float drag);
    
    void draw();
    void draw(ofTexture &tex);
    void draw(ofTexture &tex, ofTexture &tex2);
    
    int getNumParticles();
    
protected:
    list<ofxParticle*> particles;
    int numParticles;
    int totalParticlesEmitted;
};
