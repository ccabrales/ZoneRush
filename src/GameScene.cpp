#include "GameScene.h"

void GameScene::setup(){
    player.setup(&ofxAssets::image("player"));
    score = 0;
    invincibility = 0.0;
    
    livesImg = ofxAssets::image("3Life");
    livesPos = ofPoint(0, ofGetHeight() - 95);

    rightEmitter.setPosition(ofVec3f(ofGetWidth()-1,ofGetHeight()/2.0));
    rightEmitter.setVelocity(ofVec3f(-310,0.0));
    rightEmitter.posSpread = ofVec3f(0,ofGetHeight());
    rightEmitter.velSpread = ofVec3f(120,20);
    rightEmitter.life = 13;
    rightEmitter.lifeSpread = 0;
    rightEmitter.numPars = 3;
    rightEmitter.size = 12;
    rightEmitter.color = ofColor(100,100,200);
    rightEmitter.colorSpread = ofColor(70,70,70);

    enemyBullets.setup(ofRectangle(-3,-3,ofGetWidth()+6, ofGetHeight()+6));
    playerBullets.setup(ofRectangle(-3,-3,ofGetWidth()+6, ofGetHeight()+6));
    
    boss.setup();
}

void GameScene::update(){ //unused
}

#define DIFFICULTY 4.0
//the higher the easier.
inline float modBPM(const Track::Data* data){
    if(data->bpm > 200) return .5*data->bpm;
    if(data->bpm < 100) return 2.0*data->bpm;
    return data->bpm;
}

void GameScene::backgroundUpdate(const Track::Data* data, ofxParticleSystem* particleSystem){
    rightEmitter.numPars = max((int)(data->intensity*20) + (data->onBeat?12:0), 2);
    rightEmitter.setVelocity(data->onBeat?ofVec3f(-510,0.0):ofVec3f(-310,0.0));
    particleSystem->addParticles(rightEmitter);
    particleSystem->addParticles(player.emitter);

    //spawn possible enemies
    float currentDifficulty = ofMap(tick, 0, currentTrack->frameData.size(), 1.0, 3.0);
    float spawnRate = ((data->intensity)/5.0 + ((float)data->onBeat) / 7.0 + ((float)data->onsets / 32.0))/DIFFICULTY;
    if(ofRandom(1.0) < spawnRate){
        Enemy * e = new Enemy();
        e->position = ofVec3f(ofGetWidth()+10, ofWrap(data->pitch * 13, 0, ofGetHeight()));
        e->life = 500;
        e->velocity = ofVec3f(min(-ofGetWidth() * (modBPM(data)/60.0) / ((int)ofRandom(14,18)), -80.0), 0);
        e->type = EnemyFactory::getTypeRandom();
        e->setup(currentDifficulty);
        enemies.particles.push_front(e);
    }
    
    if(((float)tick)/(float)currentTrack->frameData.size() > 0.03) bossSpawned = true;
    
    bool playerExplode = checkPlayerHit() | checkEnemyHits();
    if (playerExplode) invincibility = 1.5;
    
    float lastFrameTime = ofGetLastFrameTime();
    player.update(lastFrameTime, &explosions, playerExplode);
    player.shoot(&playerBullets);
    if(bossSpawned)
        boss.update(lastFrameTime, &enemyBullets, &explosions, &enemies, data, &score);
    
    enemies.update(lastFrameTime, &enemyBullets, &explosions, data, &score);
    enemyBullets.update(lastFrameTime, 1);
    playerBullets.update(lastFrameTime, 1);
    
    explosions.gravitateTo(player.pos, 10 ,32, 12, true);
    
    explosions.update(lastFrameTime, 0.25);

    scoreRender.update(score);
    invincibility -= lastFrameTime;
}


void GameScene::draw(){
    ofPushStyle();
    if (invincibility > 0) ofSetColor(255,255,255,100);
    player.draw();
    ofPopStyle();
    
    scoreRender.draw(20, ofGetHeight() - 12);
    ofPushStyle();
    enemies.draw();
    ofPopStyle();
    boss.draw();
    enemyBullets.draw();
    playerBullets.draw();
    
    livesImg.draw(livesPos, 134, 40);
    
    ofPushStyle();
    ofSetLineWidth(4.0);
    explosions.draw();
    ofPopStyle();

    ofPushStyle();
    ofSetColor(255,244,255);
    ofFill();
    ofSetLineWidth(2.0);
    ofLine(0, 5, ofGetWidth(), 5);
    ofRect(0, 0,
           ((ofGetWidth()*tick)/currentTrack->frameData.size()), 5);
    ofPopStyle();
}

bool GameScene::checkPlayerHit() {
    //Check enemy bullets on player
    list<ofxParticle *>::iterator bullets;
    for(bullets=enemyBullets.particles.begin(); bullets!=enemyBullets.particles.end(); bullets++) {
        ofxParticle* bullet = (*bullets);
        ofVec3f loc = bullet->position;
        if (invincibility <= 0 && player.hitbox.inside(loc)) { //player is hit
            bullet->life = 0;
            player.lives--;
            livesImg = ofxAssets::image(to_string(player.lives) + "Life");
            score -= 100; //died
            return true;
        }
    }
    return false;
}

bool GameScene::checkEnemyHits() {
    list<ofxParticle *>::iterator ships; //Iterate over the ships
    bool playerHit = false;
    for(ships=enemies.particles.begin(); ships!=enemies.particles.end(); ships++) {
        Enemy * ship = ((Enemy*)(*ships));
        if (invincibility <= 0 && player.hitbox.intersects(ship->hitbox)) { //player hit enemy ship
            ship->hp = 0;
            player.lives--;
            livesImg = ofxAssets::image(to_string(player.lives) + "Life");
            score -= 100;
            playerHit = true;
            continue;
        }

        list<ofxParticle *>::iterator bullets;
        for(bullets=playerBullets.particles.begin(); bullets!=playerBullets.particles.end(); bullets++) {
            ofxParticle* bullet = (*bullets);
            ofVec3f loc = bullet->position;
            if (ship->hitbox.inside(loc)) {
                ship->hp -= player.bulletDamage;
                if(ship->hp == 0){
                    player.exp += 1;
                    livesImg = ofxAssets::image(to_string(player.lives) + "Life");
                }
                bullet->life = 0;
                continue;
            }
        }
        
        //check if ship's lasers are colliding into player.
        if(ship->laserFiring){
            ofVec3f dir = ship->laserTargetPoint-ship->position;
            ofVec3f posPlayer = player.hitbox.getCenter() - ship->position;
            
            float dist = posPlayer.cross(dir).length() / dir.length();
            if(invincibility <= 0 && dist < ship->laserWidth /2.0){
                player.lives --;
                livesImg = ofxAssets::image(to_string(player.lives) + "Life");
                score -= 100;
                playerHit = true;
                
            }
        }

    }

    return playerHit;

}