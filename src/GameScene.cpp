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
}

void GameScene::update(){
    player.update();
    float lastFrameTime = ofGetLastFrameTime();
    enemies.update(lastFrameTime, &enemyBullets);
    enemyBullets.update(lastFrameTime, 1);
    playerBullets.update(lastFrameTime, 1);
}

#define DIFFICULTY 2.0
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
        e->velocity = ofVec3f(-1.2, 0);
        e->type = EnemyFactory::getType(rand()%3);
        e->setup(currentDifficulty);
        e->size = 15;
        enemies.particles.push_front(e);
    }
    update();
}


void GameScene::draw(){
    player.draw();
    scoreRender.draw(20, ofGetHeight() - 12);
    enemies.draw();
    enemyBullets.draw();
}

void GameScene::willFadeOut() {
    
}

void GameScene::willExit(){
    
}

void GameScene::windowResized(int w, int h) {
    
}