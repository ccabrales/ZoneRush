#include "ofApp.h"

int tick = 0;
unique_ptr<Track> currentTrack = NULL;
unique_ptr<ofxAudioDecoder> globalDecoder = NULL;


int GreedyParticleSystem::update(float timeStep, float drag){
    int particlesRemoved = 0;
    for(list<ofxParticle*>::iterator it = particles.begin(); it != particles.end(); it++){
        if((*it)->isAlive() && ((*it)->position.x > -2)){
            (*it)->update(timeStep, drag);
        }
        else{
            ofxParticle * p = (*it);
            it = particles.erase(it);
            delete p;
            particlesRemoved++;
        }
    }
    numParticles-=particlesRemoved;
    return particlesRemoved;
}

//--------------------------------------------------------------
void ofApp::setup(){
    // set the size of the window
//    ofSetWindowShape(2560, 1600);
    ofSetEscapeQuitsApp(false);
//    ofSetFullscreen(true);
    ofHideCursor();
        
    //Set up scenes here
    titleScene = new TitleScene;
    titleScene->setup();
    gameScene = new GameScene;
    gameOverScene = new GameOverScene;
    
    game_state = START;
        
    globalDecoder = unique_ptr<ofxAudioDecoder>(new ofxAudioDecoder());
    
    globalDecoder->load("music.mp3");
    currentTrack = unique_ptr<Track>(new Track(globalDecoder.get()));
    tv.setup(currentTrack.get());
    
    //int sampleRate = 44100; int bufferSize = 256; int nBuffers = 4;
//    ofSoundStreamSetup(2, 0, this);
    ofSoundStreamSetup(2, 0, this, 44100, 256, 8);
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>();
    
    cloudEmitter.setPosition(ofVec2f(ofGetWidth()+500, ofGetHeight()/2.0));
    cloudEmitter.life = 40;
    cloudEmitter.lifeSpread = 3;
    cloudEmitter.posSpread = ofVec2f(100, ofGetHeight());
    cloudEmitter.velocityStart= ofVec2f(-180, 0);
    cloudEmitter.velSpread = ofVec2f(60,20);
    cloudEmitter.size = 600;
    cloudEmitter.rotVel = ofVec3f(0,0.03);
    cloudEmitter.rotVelSpread = ofVec3f(0,0.04);
    cloudEmitter.rotSpread = ofVec3f(0,3.58);
    cloudEmitter.numPars = 1;
}

void ofApp::exit(){
    ofSoundStreamStop();
    ofSoundStreamClose();
}

void ofApp::audioOut(float * input, int bufferSize, int nChannels){
    if(globalDecoder == NULL) return;
    
    copy(globalDecoder->getRawSamples().begin()+tick*bufferSize*nChannels, globalDecoder->getRawSamples().begin()+tick*bufferSize*nChannels+bufferSize*nChannels, input);
    tick ++;
    if(tick*nChannels*bufferSize > globalDecoder->getNumSamples()){
        tick = 0;
    }
    tv.updateAudio(input, bufferSize, nChannels);
}

void ofApp::audioIn(float* input, int bufferSize, int nChannels){}

//--------------------------------------------------------------
void ofApp::update(){
    
    Track::Data* d = currentTrack->readData(tick);
    tv.update(d);
    
//    backgroundParticles.update(min(ofGetLastFrameTime(), 1.0/10.0), 1);
    backgroundParticles.update(ofGetLastFrameTime(), 1);
    backgroundClouds.update(ofGetLastFrameTime(), 1);
    switch (game_state) {
        case START:
            titleScene->backgroundUpdate(d, &backgroundParticles);
            break;
        case LOAD:
            titleScene->backgroundUpdate(d, &backgroundParticles);
            //Check for when the thread is done
            checkLoadUpdate();
            break;
        case GAME:
            gameScene->backgroundUpdate(d, &backgroundParticles);
            break;
        default:
            break;
    }
    
    if(d->onBeat && backgroundClouds.getNumParticles() < 5){
        backgroundClouds.addParticles(cloudEmitter);
    }
    //TEMPORARY ENEMY SPAWNER LOL
    //    if(d->onBeat){
    //        vector<EnemyPtr>* newEnemies = EnemyFactory::makeGroup(1, 2, 0);
    //        for(int i = 0; i < newEnemies->size(); i++){
    //            enemyList.push_back((*newEnemies)[i]);
    //        }
    //        delete newEnemies;
    //    }
    //    for(int i=0; i < enemyList.size(); i++){
    //        enemyList[i]->update();
    //    }
    //    
    //    delete d;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // update beat info
    post.begin();
    ofBackground(8, 9, 32);
    ofPushStyle();
    backgroundClouds.draw(ofxAssets::image("cloud").getTexture());
    ofSetLineWidth(4);
    backgroundParticles.draw();
    ofPopStyle();
    
    ofPushStyle();
    switch (game_state) {
        case START:
        case LOAD:
            titleScene->draw();
            break;
        case GAME:
            gameScene->draw();
            break;
        case END:
            break;
        default:
            break;
    }
    ofPopStyle();
    
    for(int i=0; i < enemyList.size(); i++){
        enemyList[i]->draw();
    }
    
    post.end(true);
    tv.draw(tick);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (game_state == START) { //Check for hitting "play" then change scene
        switch (key) {
            case OF_KEY_DOWN:
            case OF_KEY_UP:
                titleScene->update();
                break;
            case OF_KEY_RETURN:
                if (titleScene->isPlaySelected()) {
                    ofFileDialogResult res = ofSystemLoadDialog();
                    if (res.bSuccess) {
                        audioLoader = unique_ptr<AudioLoader>(new AudioLoader());
                        audioLoader->start(res);
                        game_state = LOAD;
                        titleScene->setLoading(titleScene->TRANSITION);
                    }
                } else {
                    std::exit(0);
                }
            default:
                break;
        }
    } else if (game_state == GAME) {
        switch (key) {
            case OF_KEY_LEFT:
                gameScene->player.is_left_pressed = true;  break;
            case OF_KEY_RIGHT:
                gameScene->player.is_right_pressed = true; break;
            case OF_KEY_UP:
                gameScene->player.is_up_pressed = true;    break;
            case OF_KEY_DOWN:
                gameScene->player.is_down_pressed = true;  break;
            default:
                break;
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (game_state == GAME) {
        switch (key) {
            case OF_KEY_LEFT:
                gameScene->player.is_left_pressed = false;     break;
            case OF_KEY_RIGHT:
                gameScene->player.is_right_pressed = false;    break;
            case OF_KEY_UP:
                gameScene->player.is_up_pressed = false;       break;
            case OF_KEY_DOWN:
                gameScene->player.is_down_pressed = false;     break;
            default:
                break;
        }
    }
}

void ofApp::checkLoadUpdate() {
    audioLoader->lock();
    if (audioLoader->isDone) {
        if (titleScene->getCurrentState() == titleScene->LOAD) { //Trigger
            if (audioLoader->hasError) { //Something went wrong :( or user hit cancel
                string errorMsg = audioLoader->errorMsg;
                ofSystemAlertDialog(errorMsg);
                titleScene->setLoading(titleScene->TITLE);
                game_state = START;
            } else { //Loaded successfully! :) so start transition to game
                titleScene->setLoading(titleScene->TOGAME);
            }
        } else if (titleScene->getCurrentState() == titleScene->END) { //Can move on
            titleScene->setLoading(titleScene->TITLE);
            tick = 0;
            globalDecoder.release();
            currentTrack.release();
            globalDecoder = unique_ptr<ofxAudioDecoder>(audioLoader->tempDecoder.get());
            currentTrack = unique_ptr<Track>(audioLoader->loadedTrack.get());
            tv.setup(currentTrack.get());
            game_state = GAME;
            gameScene->setup();
        }
    }
    audioLoader->unlock();

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    post.init(w,h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
