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
    if(cd <= 0 && !laserCharging && !laserFiring){
        if(type->bulletType->firePattern==LASERSHOT) fireLaser();
        else                fire(bulletSpace);
    }
    if(laserCharging || laserFiring){
        laserChargeTimer -= timeStep;
        if(laserCharging && laserChargeTimer <= 0 && onBeat){
            laserChargeTimer = 0.3 + 1.0/(difficultyScaling);
            laserFiring = true;
            //draw the polygon:
            laserCharging = false;
            SoundLibrary::playSound(SoundItem::LASER);
            ofFloatColor c = ofColor(255,180,180);
            ofFloatColor e = ofColor(255,255,255);

            laser.updateColor(0, c);
            laser.updateColor(1, e);

        }else if(laserFiring && laserChargeTimer <= 0){
            laserFiring = false;
            velocity = oldVel;
        }
        if(laserFiring){
            laser.updateWeight(0,laserWidth*1.4/2.0*laserChargeTimer/(0.3 + 1.0/(difficultyScaling)));
            laser.updateWeight(1,laserWidth/2.0*laserChargeTimer/(0.3 + 1.0/(difficultyScaling)));
        }
        laser.update();
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
        ex.velSpread = ofVec3f(250, 250);
        ex.numPars = 80;
        explosionSystem->addParticles(ex);
        SoundLibrary::playSound(SoundItem::EXPLODE_LOUD);
        *score += type->score;
        this->life = 0;
    }
}



void Enemy::fireLaser(){
    SoundLibrary::playSound(SoundItem::POWERUP);

    laserTargetPoint = (player.hitbox.getCenter() - ofxParticle::position).normalize()*3500+ofxParticle::position;
    laser = ofxFatLine();
    laser.add(position, ofFloatColor::whiteSmoke, 2);
    laser.add(laserTargetPoint, ofFloatColor::cyan, 2);
    laserChargeTimer = 1.2;
    oldVel = velocity;
    velocity = ofVec3f(0,0,0);
    laserCharging = true;
    cd = 5;
}


void Enemy::moveHitbox() {
    float width = texture!=NULL? texture->getWidth():20;
    float height = texture!=NULL?texture->getHeight():40;
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
            gun.numPars = 10;
            gun.setVelocity(ofVec2f(0,0));
            gun.velSpread = ofVec3f(BulletSpeed*2.0, BulletSpeed*2.0);
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
        laser.draw();
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

void EnemySystem::draw(){
    list<ofxParticle*>::iterator it = particles.begin();
    if(it == particles.end()) return;
    if((**it).texture != NULL){
        for(; it != particles.end(); it++){
            (*(Enemy*)(*it)).draw((**it).texture->getTexture());
        }
    }else{
        for(; it != particles.end(); it++){
            (*(Enemy*)(*it)).draw();
        }
    }
}

void Boss::setup(){
    firemode = BChill;
    state=BossEntering;
    decoder.decode("space_transparent_ship.gif");
    bossGif = decoder.getFile();
    internalEnemies = new EnemySystem();
    maxHP = 500;
    hp = maxHP;
    animationProgress = 0;
}

//Todo: check other systems to not spawn while boss is here...

inline float modBPM(const Track::Data* data){
    if(data->bpm > 200) return .5*data->bpm;
    if(data->bpm < 100) return 2.0*data->bpm;
    return data->bpm;
}


void Boss::update(const float timeStep, ofxParticleSystem* bulletSpace, ofxParticleSystem* explosionSystem,EnemySystem* globalEnemies, const Track::Data* data, int* score){
    
    if(hp < 0 && state!=BossDead){
        
        ofxParticleEmitter ex;
        ex.setPosition(ofVec2f(ofGetWidth()-300, ofGetHeight()/2.0));
        ex.posSpread = ofVec3f(400, bossGif.getHeight()/2.0);
        ex.setVelocity(ofVec3f(0,0));
        ex.velSpread = ofVec3f(800,500);
        ex.life = 13;
        ex.lifeSpread = 2;
        ex.color = ofColor(255, 150, 150);
        ex.colorSpread = ofColor(0, 100, 100);
        ex.numPars = 80;
        explosionSystem->addParticles(ex);
        SoundLibrary::playSound(SoundItem::SEISMIC_CHARGE);
        
        *score += 30;
        
        this->life = 0;

        hitbox.set(-2,-2,1,1);
        state = BossDestroyed;
        internalEnemies->ofxParticleSystem::update(4000, 1.0);
        animationProgress += timeStep+enterDuration*enterDuration/13.0;
        enterDuration+=0.1;
        if(enterDuration > 10){
            state = BossDead;
        }
        return;
    }
    
    hitbox.set(ofGetWidth()-(400), ofGetHeight()/2.0 - bossGif.getHeight()/2.0, bossGif.getWidth(), bossGif.getHeight());
    
    animationProgress += timeStep;
//    animationFrame = ofxTween::map(cos(animationProgress/1.5), -1.0, 1.0, 0, bossGif.getNumFrames(), true, cubic);
    animationFrame = ofMap(sin(animationProgress/1.5), -1.0, 1.0, 6, bossGif.getNumFrames()-2);
    
    switch(state){
        case BossEntering:
            enteringDone += timeStep;
            if(enteringDone > enterDuration && data->onBeat){
                state = BossFight;
                enteringDone = 0;
                newFiringState(data);
            }
            break;
        case BossFight:
            enteringDone += timeStep; //add time.
            if(enteringDone >= 8.0*(modBPM(data)/60.0) && data->onBeat){
                newFiringState(data);
                enteringDone = 0;
            }
            break;
        case BossDestroyed:
        case BossDead:
            break;
    }
    
    internalEnemies->update(timeStep, bulletSpace, explosionSystem, data, score);
}

void Boss::newFiringState(const Track::Data* data){
    int newState;
    do{
        newState = rand()%4;
    }while(newState==(int)firemode);
    
    firemode = (BossFiringState) newState;
    
    delete internalEnemies;
    internalEnemies = new EnemySystem();
    
    ofVec2f bossPos = ofVec2f(ofGetWidth()-(400), ofGetHeight()/2.0 - bossGif.getHeight()/2.0);
    
    switch(firemode){
        case BEverythingGoes:
            for(int i = 0; i < 16; i++){
                int enemynum = i%4;
                Enemy* e = new Enemy();
                switch(enemynum){
                    case 0:
                        e->position = bossPos +ofVec2f(130, bossGif.getHeight()/2); break;
                    case 1:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()/4, bossGif.getHeight()/2+50); break;
                    case 2:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()/4, bossGif.getHeight()/2-50); break;
                    case 3:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()*2.0/5.0, bossGif.getHeight()/2); break;
                }
                e->velocity = ofVec2f(0,0);
                e->state = EnemyState::HEALTHY;
                e->type = EnemyFactory::getTypeRandom();
                e->life = 40;
                e->setup(2.0);
                e->cd = ((float)i)*0.5*(modBPM(data)/60.0);
                

                internalEnemies->particles.push_front(e);
            }
            //spawns both laser and bullet enemies indiscrimantly and also shoots big laser.
            break;
        case BLasers:
            for(int i = 0; i < 8; i++){
                int enemynum = i%4;
                Enemy* e = new Enemy();
                switch(enemynum){
                    case 0:
                        e->position = bossPos +ofVec2f(130, bossGif.getHeight()/2); break;
                    case 1:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()/4, bossGif.getHeight()/2+50); break;
                    case 2:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()/4, bossGif.getHeight()/2-50); break;
                    case 3:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()*2.0/5.0, bossGif.getHeight()/2); break;
                }
                e->velocity = ofVec2f(0,0);
                e->type = EnemyFactory::getType(7);
                e->setup(2.0);
                e->cd = ((float)i)*0.25*(modBPM(data)/60.0);
                e->life = 40;
                internalEnemies->particles.push_front(e);
            }

            //spawns laser dudes and shoots big laser.
        case BBullets:
            for(int i = 0; i < 12; i++){
                int enemynum = i%4;
                Enemy* e = new Enemy();
                switch(enemynum){
                    case 0:
                        e->position = bossPos +ofVec2f(130, bossGif.getHeight()/2); break;
                    case 1:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()/4, bossGif.getHeight()/2+50); break;
                    case 2:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()/4, bossGif.getHeight()/2-50); break;
                    case 3:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()*2.0/5.0, bossGif.getHeight()/2); break;
                }
                e->velocity = ofVec2f(0,0);
                e->type = EnemyFactory::getType(rand()%7);
                e->setup(2.0);
                e->life = 40;
                e->cd = ((float)i)*0.25*(modBPM(data)/60.0);
                internalEnemies->particles.push_front(e);
            }

            //spawn bullet dudes.
        case BChill:
            for(int i = 0; i < 4; i++){
                int enemynum = i%4;
                Enemy* e = new Enemy();
                switch(enemynum){
                    case 0:
                        e->position = bossPos +ofVec2f(130, bossGif.getHeight()/2); break;
                    case 1:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()/4, bossGif.getHeight()/2+50); break;
                    case 2:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()/4, bossGif.getHeight()/2-50); break;
                    case 3:
                        e->position = bossPos + ofVec2f(bossGif.getWidth()*2.0/5.0, bossGif.getHeight()/2); break;
                }
                e->velocity = ofVec2f(0,0);
                e->type = EnemyFactory::getTypeRandom();
                e->setup(2.0);
                e->life = 40;
                e->cd = ((float)i)*0.25*(modBPM(data)/60.0);
                internalEnemies->particles.push_front(e);
            }

            //spawns bullet dudes at reduced rate.
            break;
    }
}

void Boss::draw(){
    ofPushStyle();
    switch(state){
        case BossEntering:
            bossGif.drawFrame(animationFrame, ofGetWidth()-min(400.0*(enteringDone/enterDuration), 400.0), ofGetHeight()/2.0 - bossGif.getHeight()/2.0);
            break;
        case BossFight:
            internalEnemies->draw();
            bossGif.drawFrame(animationFrame, ofGetWidth()-(400), ofGetHeight()/2.0 - bossGif.getHeight()/2.0);
            
            //draw healthbar
            ofPushStyle();
            ofSetColor(255,200,200);
            ofLine(ofGetWidth()/2-maxHP, ofGetHeight()-12, ofGetWidth()/2+maxHP, ofGetHeight()-12);
            ofSetColor(200,0,0);
            ofFill();
            ofDrawRectangle(ofRectangle(ofGetWidth()/2-maxHP, ofGetHeight()-12, hp*2, 12));
            

            ofPopStyle();
            break;
        case BossDestroyed:
            bossGif.drawFrame(animationFrame, ofGetWidth()-(400), ofGetHeight()/2.0 - bossGif.getHeight()/2.0);
            break;
        case BossDead:
            break;
    }
    ofPopStyle();
}

Boss::~Boss(){
    delete internalEnemies;
}

