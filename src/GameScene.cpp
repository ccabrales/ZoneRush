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

void GameScene::backgroundUpdate(const Track::Data* data, ofxParticleSystem* particleSystem){
    rightEmitter.numPars = max((int)(data->intensity*20) + (data->onBeat?12:0), 2);
    rightEmitter.setVelocity(data->onBeat?ofVec3f(-510,0.0):ofVec3f(-310,0.0));
    
    particleSystem->addParticles(rightEmitter);
    particleSystem->addParticles(player.emitter);
    
    update();
}


void GameScene::draw(){
    player.draw();
    scoreRender.draw(20, ofGetHeight() - 12);
    for(int i=0; i < enemyList.size(); i++){
        enemyList[i]->draw();
    }
}

void GameScene::willFadeOut() {
    
}

void GameScene::willExit(){
    
}

void GameScene::windowResized(int w, int h) {
    
}