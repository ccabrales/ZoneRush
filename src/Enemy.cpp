#include "Enemy.h"

EnemyFactory::EnemyFactory(){
    e_types.push_back(EnemyType {
        4, 5, BulletLibrary::getWeaponInfo(0), 0.7, NULL
    });
    e_types.push_back(EnemyType {
        4, 7, BulletLibrary::getWeaponInfo(0), 0.9, NULL
    });
}


EnemyPtr EnemyFactory::make(int typeID, float diffScaling=1.0){
    static EnemyFactory inst;
    EnemyPtr e = EnemyPtr(new Enemy);
    e->type = &(inst.e_types[typeID]);
    e->setup(diffScaling);
    return e;
}

vector<EnemyPtr >* EnemyFactory::makeGroup(int type, int size, float variance, float diffScaling = 1){
    vector< EnemyPtr >* output = new vector<EnemyPtr>();
    for(int i = 0; i < size; i++){
        EnemyPtr newChallenger = make(type, diffScaling);
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

void Enemy::draw(){
    ofxParticle::draw(type->texture->getTexture());
}
