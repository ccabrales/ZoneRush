#include "enemy.h"

EnemyFactory::EnemyFactory(){
    e_types.push_back(EnemyType {
        4, 5, BulletLibrary::getWeaponInfo(0), 3, NULL
    });
    e_types.push_back(EnemyType {
        4, 7, BulletLibrary::getWeaponInfo(0), 5, NULL
    });
}


EnemyPtr EnemyFactory::make(int type){
    static EnemyFactory inst;
    EnemyPtr e = EnemyPtr(new Enemy);
    e->type = &(inst.e_types[type]);
    e->setup();
    return e;
}

vector<EnemyPtr >* EnemyFactory::makeGroup(int type, int size, float variance){
    
    vector< EnemyPtr >* output = new vector<EnemyPtr>();
    for(int i = 0; i < size; i++){
        int pathId = (int)ofRandom(0, PathLibrary::size()); /*randomized*/
        EnemyPtr newChallenger = make(type);
        newChallenger->calculate_movement(PathLibrary::getPath(pathId));
        output->push_back(newChallenger);
    }
    return output;
}


void Enemy::setup(){
    hp = (int)(ofRandom(type->minHP, type->maxHP+1));
    //TODO: firerate and timekeeping.
    cd = 0;   //donno how firerate works yet.
    spawnTime = tick; //donno how timekeeping works yet.
    state = HEALTHY;
}

void Enemy::update(){
    float dist = ((float)(tick - spawnTime))*type->speed;
    pos = path.getPointAtLength(dist);
}

void Enemy::draw(){
    ofPushStyle();
    ofSetColor(255,0,0);
    ofCircle(pos, 6);
    ofPopStyle();
}

void Enemy::calculate_movement(const ofPolyline* archetype){
    ofPoint multiplier(ofGetWidth(), ofGetHeight());
    for(int i = 0; i < archetype->size(); i++){
        path.addVertex((*archetype)[i]*multiplier);
    }
}