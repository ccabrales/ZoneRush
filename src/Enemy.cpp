#include "Enemy.h"

EnemyFactory::EnemyFactory(){
    e_types.push_back(EnemyType {
        4, 5, BulletLibrary::getWeaponInfo(0), true, &ofxAssets::image("s1")
    });
    e_types.push_back(EnemyType {
        4, 7, BulletLibrary::getWeaponInfo(2), false, &ofxAssets::image("s2")
    });
    e_types.push_back(EnemyType {
        3, 8, BulletLibrary::getWeaponInfo(4), false, &ofxAssets::image("s3")
    });

}

EnemyType* EnemyFactory::getType(int typeID){
    static EnemyFactory inst;
    return &(inst.e_types[typeID]);
}



void Enemy::setup(float diffScaling){
    hp = (int)(ofRandom(type->minHP*diffScaling, (type->maxHP+1)*diffScaling));
    cd = 0;
    difficultyScaling = diffScaling;
    state = HEALTHY;
    texture = type->texture;
    size = max(type->texture->getWidth(), type->texture->getHeight());
    moveHitbox();
    gun.size = 10;
}

void Enemy::update(const float timeStep, const float drag, ofxParticleSystem* bulletSpace){
    ofxParticle::update(timeStep, drag);
    moveHitbox();
    cd -= timeStep;
    if(cd <= 0){
        fire(bulletSpace);
    }
    if(hp<=0) ofxParticle::color = ofColor(255,255,255,100);
}

void Enemy::moveHitbox() {
    float width = texture->getWidth();
    float height = texture->getHeight();
    if (hp > 0) {
        hitbox.set(this->position.x - (width/2.0), this->position.y - (height/2.0), width, height);
    } else {
        hitbox.set(-10, -10, 1, 1);
    }
}

#define BulletSpeed 120
void Enemy::fire(ofxParticleSystem* bulletSpace){
    gun.setPosition(this->position);
    gun.life = 130;
    
    if(type->bulletType->targetPattern==PLAYER){
        //point towards player.
        ofVec3f posDir = (player.pos - position).normalize()*difficultyScaling*BulletSpeed;
        gun.setVelocity(posDir);
    }else{
        gun.setVelocity(ofVec3f(-BulletSpeed * difficultyScaling, 0));
        //down the lane
    }
    //further gun setup.
    switch(type->bulletType->firePattern){
        case STRAIGHT:
            gun.numPars = 1;
            cd += 0.6;
            break;
        case CLOUD:
            gun.numPars = 30;
            gun.velSpread = ofVec3f(BulletSpeed*2.0*difficultyScaling, BulletSpeed*2.0*difficultyScaling);
            cd += 1.4;
            break;
        case THREESHOT:
            gun.numPars = 3;
            gun.velSpread = ofVec3f(0.1, BulletSpeed/15.0);
            cd += 0.8;
            break;
        case TWOSHOT:
            gun.numPars = 2;
            gun.velSpread = ofVec3f(0.1, BulletSpeed/12.0);
            cd+=0.6;
            break;
        default:
            gun.numPars = 1;
            cd+=0.2;
            break;
    }
    
    cd /= difficultyScaling / 4.0;
    
    
    bulletSpace->addParticles(gun, type->bulletType->texture);
}

void Enemy::draw(){ //Never called
    ofxParticle::draw(type->texture->getTexture());
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofDrawRectangle(hitbox);
    ofPopStyle();
}

int EnemySystem::update(float timeStep, ofxParticleSystem* bulletSystem){
    int particlesRemoved = 0;
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        if((**it).isAlive() && (**it).position.x > -13 /*&& (*((Enemy*)*it)).hp > 0*/){
            (*((Enemy*)*it)).update(timeStep, 1.0, bulletSystem);
        }
        else{
            Enemy * p = ((Enemy*)(*it));
            it = particles.erase(it);
            delete p;
            particlesRemoved++;
        }
    }
    numParticles-=particlesRemoved;
    return particlesRemoved;
}

//void EnemySystem::draw() {
//    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
//        if((**it).isAlive() && (**it).position.x > -13){
//            Enemy e = (*((Enemy*)*it));
//            e.draw();
//            ofPushStyle();
//            ofSetColor(255, 0, 0);
//            ofDrawRectangle(e.hitbox);
//            ofPopStyle();
//        }
//        else{
//            Enemy * p = ((Enemy*)(*it));
//            it = particles.erase(it);
//            delete p;
//        }
//    }
//
//}

//UNUSED

//EnemyPtr EnemyFactory::make(int typeID, float diffScaling=1.0){
//    static EnemyFactory inst;
//    EnemyPtr e = EnemyPtr(new Enemy);
//    e->type = &(inst.e_types[typeID]);
//    e->setup(diffScaling);
//    return e;
//}
//
//vector<EnemyPtr >* EnemyFactory::makeGroup(int type, int size, float variance, float diffScaling = 1){
//    vector< EnemyPtr >* output = new vector<EnemyPtr>();
//    for(int i = 0; i < size; i++){
//        EnemyPtr newChallenger = make(type, diffScaling);
//        output->push_back(newChallenger);
//    }
//    return output;
//}

