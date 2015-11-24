#include "Enemy.h"

EnemyFactory::EnemyFactory(){
    e_types.push_back(EnemyType {
        4, 5, BulletLibrary::getWeaponInfo(0), 0.7, NULL
    });
    e_types.push_back(EnemyType {
        4, 7, BulletLibrary::getWeaponInfo(0), 0.9, NULL
    });
}


EnemyPtr EnemyFactory::make(int typeID){
    static EnemyFactory inst;
    EnemyPtr e = EnemyPtr(new Enemy);
    e->type = &(inst.e_types[typeID]);
    e->setup();
    return e;
}

vector<EnemyPtr >* EnemyFactory::makeGroup(int type, int size, float variance){
    
    vector< EnemyPtr >* output = new vector<EnemyPtr>();
    for(int i = 0; i < size; i++){
        int pathId = (int)ofRandom(0, PathLibrary::size()); /*randomized*/
        EnemyPtr newChallenger = make(type);
//        newChallenger->calculate_movement(PathLibrary::getPath(pathId));
        output->push_back(newChallenger);
    }
    return output;
}


void Enemy::setup(float diffScaling){
    hp = (int)(ofRandom(type->minHP*diffScaling, (type->maxHP+1)*diffScaling));
    cd = 0;
    difficultyScaling = diffScaling;
    state = HEALTHY;
}

void Enemy::update(const float timeStep, const float drag, ofxParticleSystem* bulletSpace){
    ofxParticle::update(timeStep, drag);
    cd -= timeStep;
    if(cd <= 0){
        //fire
        //TODO
        cd = 2.3;
    }
}
//void Enemy::update(){
//    float dist = ((float)(tick - spawnTime))*type->speed;
//    pos = path.getPointAtLength(dist);
//}

void Enemy::draw(){
//    ofPushStyle();
//    ofSetColor(3,3,3,100);
//    ofSetColor(255,0,0);
//    ofCircle(pos, 6);
//    ofPopStyle();
}

//void Enemy::calculate_movement(const ofPolyline* archetype){
//    ofPoint multiplier(ofGetWidth(), ofGetHeight());
//    for(int i = 0; i < archetype->size(); i++){
//        path.addVertex((*archetype)[i]*multiplier);
//    }
//}