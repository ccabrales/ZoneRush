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
    
    
    //TODO: remove next 2 lines test code
    for(int i=0; i < enemyList.size(); i++) enemyList[i]->update();
//    if (player.pos.x > 150) player.lives = 0; //Test code for game over
}

void GameScene::backgroundUpdate(const Track::Data* data, ofxParticleSystem* particleSystem){
    rightEmitter.numPars = max((int)(data->intensity*20) + (data->onBeat?12:0), 2);
    rightEmitter.setVelocity(data->onBeat?ofVec3f(-510,0.0):ofVec3f(-310,0.0));
    
    particleSystem->addParticles(rightEmitter);
    particleSystem->addParticles(player.emitter);
    
    //TODO: remove test spawning code here
    if(data->onBeat){
        vector<EnemyPtr>* newEnemies = EnemyFactory::makeGroup(1, 2, 0, 1.0);
        for(int i = 0; i < newEnemies->size(); i++){
            if (i % 2 == 0) (*newEnemies)[i]->hp = 0; //TODO remove test code
            enemyList.push_back((*newEnemies)[i]);
        }
        delete newEnemies;
    }
    
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