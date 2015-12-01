#include "GameScene.h"

void GameScene::setup(){
    player.setup(&ofxAssets::image("player"));
    score = 0;

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
}

void GameScene::update(){ //unused
}

#define DIFFICULTY 4.0
//the higher the easier.


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
        e->velocity = ofVec3f(-80, 0);
        e->type = EnemyFactory::getType(rand()%3);
        e->setup(currentDifficulty);
        enemies.particles.push_front(e);
    }
    
    checkPlayerHit();
    checkEnemyHits();
    
    float lastFrameTime = ofGetLastFrameTime();
    player.update(lastFrameTime);
    player.shoot(&playerBullets);
    enemies.update(lastFrameTime, &enemyBullets, &explosions, data);
    enemyBullets.update(lastFrameTime, 1);
    playerBullets.update(lastFrameTime, 1);
    explosions.update(lastFrameTime, 0.8);
}


void GameScene::draw(){
    player.draw();
    scoreRender.draw(20, ofGetHeight() - 12);
    enemies.draw();
    enemyBullets.draw();
    playerBullets.draw();
    
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

void GameScene::checkPlayerHit() {
    //Check enemy bullets on player
    list<ofxParticle *>::iterator bullets;
    for(bullets=enemyBullets.particles.begin(); bullets!=enemyBullets.particles.end(); bullets++) {
        ofxParticle* bullet = (*bullets);
        ofVec3f loc = bullet->position;
        if (player.hitbox.inside(loc)) { //player is hit
            bullet->life = 0;
            player.lives--;
            //TODO reset player position
            //TODO reset enemies on screen
            //TODO update score (lose points? something?)
            break;
        }
    }
}

void GameScene::checkEnemyHits() {
    list<ofxParticle *>::iterator ships; //Iterate over the ships
    for(ships=enemies.particles.begin(); ships!=enemies.particles.end(); ships++) {
        Enemy * ship = ((Enemy*)(*ships));
        if (player.hitbox.intersects(ship->hitbox)) { //player hit enemy ship
            ship->hp = 0;
            player.lives--;
            //TODO reset player position
            //TODO animate explosions of player and ship
            //TODO reset enemies on screen
            //TODO update score (lose points? something?)
            continue;
        }

        list<ofxParticle *>::iterator bullets;
        for(bullets=playerBullets.particles.begin(); bullets!=playerBullets.particles.end(); bullets++) {
            ofxParticle* bullet = (*bullets);
            ofVec3f loc = bullet->position;
            if (ship->hitbox.inside(loc)) {
                ship->hp -= player.bulletDamage;
                bullet->life = 0;
                //TODO the same stuff as above
                //TODO also increment score
                break;
            }
        }
    }


}

void GameScene::willFadeOut() {

}

void GameScene::willExit(){

}

void GameScene::windowResized(int w, int h) {

}
