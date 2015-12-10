#include "Enemy.h"

EnemyFactory::EnemyFactory(){
    
    e_types.push_back(EnemyType {
        4, 5, 100, BulletLibrary::getWeaponInfo(0, false), true, &ofxAssets::image("s1")
    });
    e_types.push_back(EnemyType {
        2, 3, 110, BulletLibrary::getWeaponInfo(1, false), false, &ofxAssets::image("s2")
    });
    e_types.push_back(EnemyType {
        3, 8, 230, BulletLibrary::getWeaponInfo(2, false), false, &ofxAssets::image("s3")
    });
    e_types.push_back(EnemyType {
        3, 8, 420, BulletLibrary::getWeaponInfo(3, false), false, &ofxAssets::image("s4")
    });
    e_types.push_back(EnemyType {
        3, 8, 500, BulletLibrary::getWeaponInfo(4, false), false, &ofxAssets::image("s5")
    });
    e_types.push_back(EnemyType {
        3, 8, 540, BulletLibrary::getWeaponInfo(5, false), false, &ofxAssets::image("s6")
    });
    e_types.push_back(EnemyType {
        3, 8, 300, BulletLibrary::getWeaponInfo(6, false), false, &ofxAssets::image("s3")
    });
    e_types.push_back(EnemyType {
        3, 8, 1300, BulletLibrary::getWeaponInfo(7, false), false, &ofxAssets::image("s7")
    });

}

EnemyType* EnemyFactory::getType(int typeID){
    static EnemyFactory inst;
    return &(inst.e_types[typeID]);
}

EnemyType* EnemyFactory::getTypeRandom(){
    static EnemyFactory inst;
    return &(inst.e_types[rand()%inst.e_types.size()]);
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
//    explosion.size = 5;
}

void Enemy::update(const float timeStep, const float drag, ofxParticleSystem* bulletSpace, ofxParticleSystem* explosionSystem, bool onBeat, int* score){
    ofxParticle::update(timeStep, drag);
    moveHitbox();
    cd -= timeStep;
    if(cd <= 0){
        if(type->bulletType->firePattern==LASERSHOT) fireLaser();
        else                fire(bulletSpace);
    }
    if(laserCharging || laserFiring){
        laserChargeTimer -= timeStep;
        if(laserCharging && laserChargeTimer <= 0 ){
            laserChargeTimer = 1.3;
            laserFiring = true;
            //draw the polygon:
            laserCharging = false;
        }else if(laserFiring && laserChargeTimer <= 0){
            laserFiring = false;
        }
    }
    
    
    
    if(hp<=0) ofxParticle::color = ofColor(255,255,255,100);
    if (onBeat && hp <= 0) { //Explosion
        ofxParticleEmitter ex;
        ex.setPosition(ofxParticle::position);
        ex.setVelocity(ofxParticle::velocity);
        ex.life = 13;
        ex.lifeSpread = 2;
        ex.color = ofColor(255, 150, 150);
        ex.colorSpread = ofColor(0, 100, 100);
        ex.velSpread = ofVec3f(50, 50);
        ex.numPars = 80;
        explosionSystem->addParticles(ex);
        SoundLibrary::playSound(SoundItem::EXPLODE_LOUD);
        *score += type->score;
        this->life = 0;
    }
}



void Enemy::fireLaser(){
    laserTargetPoint = (player.hitbox.getCenter() - ofxParticle::position).normalize()*3500+ofxParticle::position;
    laserChargeTimer = 2.3;
    laserCharging = true;
    cd = 5;
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

void Enemy::fire(ofxParticleSystem* bulletSpace){
    
    
    if (hp <= 0) return;
    gun.setPosition(this->position);
    gun.life = 130;

    if(type->bulletType->targetPattern==PLAYER){
        //point towards player.
        ofVec3f posDir = (player.hitbox.getCenter() - position).normalize()*difficultyScaling*BulletSpeed;
        gun.setVelocity(posDir);
    }else{
        gun.setVelocity(ofVec3f(-BulletSpeed * difficultyScaling, 0));
        //down the lane
    }
    //further gun setup.
    switch(type->bulletType->firePattern){
        case STRAIGHT:
            gun.numPars = 1;
            break;
        case CLOUD:
            gun.numPars = 30;
            gun.velSpread = ofVec3f(BulletSpeed*2.0*difficultyScaling, BulletSpeed*2.0*difficultyScaling);
            break;
        case THREESHOT:
            gun.numPars = 3;
            gun.velSpread = ofVec3f(0.1, BulletSpeed/15.0);
            break;
        case TWOSHOT:
            gun.numPars = 2;
            gun.velSpread = ofVec3f(0.1, BulletSpeed/12.0);
            break;
        default:
            gun.numPars = 1;
            break;
    }
    cd += type->bulletType->cd;

    cd /= difficultyScaling / 4.0;


    bulletSpace->addParticles(gun, type->bulletType->texture);
}

void Enemy::draw(){ //Never called
    ofxParticle::draw(type->texture->getTexture());
}

void Enemy::draw(ofTexture &tex){
    float w = tex.getWidth();
    float h = tex.getHeight();
    if(w > h){
        h = h/w*size;
        w = size;
    }
    else{
        w = w/h*size;
        h = size;
    }
    ofSetColor(color);
    ofPushMatrix();
    ofTranslate(position);
    ofRotateX(rotation.x);
    ofRotateY(rotation.y);
    ofRotateZ(rotation.z);
    tex.draw(w*-0.5,h*-0.5,w,h);
    ofPopMatrix();
    
    if(laserFiring || laserCharging){
        ofPushStyle();
        ofSetColor(ofColor::whiteSmoke);
        ofSetLineWidth(laserFiring?laserWidth:2.0);
        ofLine(position, laserTargetPoint);
        ofPopStyle();
    }
}


int EnemySystem::update(float timeStep, ofxParticleSystem* bulletSystem, ofxParticleSystem* explosionSystem, const Track::Data *data, int* score){
    int particlesRemoved = 0;
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        if((**it).isAlive() && (**it).position.x > -13 /*&& (*((Enemy*)*it)).hp > 0*/){
            (*((Enemy*)*it)).update(timeStep, 1.0, bulletSystem, explosionSystem, data->onBeat, score);
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

