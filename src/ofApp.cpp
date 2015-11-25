#include "ofApp.h"

int tick = 0;
unique_ptr<Track> currentTrack = NULL;
unique_ptr<ofxAudioDecoder> globalDecoder = NULL;
int score = 0;
Player player;

//--------------------------------------------------------------
void ofApp::setup(){
    // set the size of the window
//    ofSetWindowShape(2560, 1600);
    ofSetEscapeQuitsApp(false);
//    ofSetFullscreen(true);
    //TODO: enable fullscreen in next demo.
    ofHideCursor();
    
    game_state = START;
    
    //Scene Setup:
    gameScene = new GameScene;
    gameOverScene = new GameOverScene;
    titleScene = new TitleScene;
    titleScene->setup();
    
    
    //Audio Setup:
    globalDecoder = unique_ptr<ofxAudioDecoder>(new ofxAudioDecoder());
    globalDecoder->load("music.mp3");
    
    //Audio Parsing...
    //TODO this section is not really necessary. remove in release build
    currentTrack = unique_ptr<Track>(new Track(globalDecoder.get()));
    tv.setup(currentTrack.get());
    
    ofSoundStreamSetup(2, 0, this, 44100, 256, 8);

    setupPostProcessing();
    setupParticleEmitters();
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


//--------------------------------------------------------------
void ofApp::update(){
    
    Track::Data* d = currentTrack->readData(tick);
    tv.update(d);
    
    
    //backgroundParticles.update(min(ofGetLastFrameTime(), 1.0/10.0), 1);
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
            if (player.lives <= 0) {
                game_state = END;
                gameOverScene->setup(gameScene->score, 97); //TODO: fix hard code and give actual percentage of song completed
            }
            break;
        case END:
            gameOverScene->backgroundUpdate(d, &backgroundParticles);
        default:
            break;
    }
    
    if(d->onsets>0 || d->onBeat){
        pass->setEnabled(true);
        if(d->onBeat && backgroundClouds.getNumParticles() < 5){
            backgroundClouds.addParticles(cloudEmitter);
        }
    }else{
        pass->setEnabled(false);
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
            gameOverScene->draw();
            break;
        default:
            break;
    }
    ofPopStyle();
    
//    for(int i=0; i < enemyList.size(); i++){
//        enemyList[i]->draw();
//    }
    
    post.end(true);
    
    tv.draw(tick);
    
//    ofxAssets::image("s1").draw(30, 41);
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    post.init(w,h);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (game_state == START) {
        //Check for hitting "play" then change scene
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
                player.is_left_pressed = true;  break;
            case OF_KEY_RIGHT:
                player.is_right_pressed = true; break;
            case OF_KEY_UP:
                player.is_up_pressed = true;    break;
            case OF_KEY_DOWN:
                player.is_down_pressed = true;  break;
            default:
                break;
        }
    } else if (game_state == END) {
        switch (key) {
            case OF_KEY_RETURN:
                game_state = START;
                break;
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
                player.is_left_pressed = false;     break;
            case OF_KEY_RIGHT:
                player.is_right_pressed = false;    break;
            case OF_KEY_UP:
                player.is_up_pressed = false;       break;
            case OF_KEY_DOWN:
                player.is_down_pressed = false;     break;
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
void ofApp::audioIn(float* input, int bufferSize, int nChannels){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
//--------------------------------------------------------------

void ofApp::setupPostProcessing(){
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>();
    pass = post.createPass<DofPass>();
    pass->setMaxBlur(0.05);
    pass->setAperture(0.2);
    //    pass = post.createPass<GodRaysPass>();
    //    pass->setAttenuationOffset(0.9);
}

void ofApp::setupParticleEmitters(){
    //----------SETUP--------Particle System pass:
    backgroundClouds.setup(ofRectangle(-200,-200,ofGetWidth()+400, ofGetHeight()+400));
    backgroundParticles.setup(ofRectangle(-3,-3,ofGetWidth()+6, ofGetHeight()+6));
    
    
    cloudEmitter.setPosition(ofVec2f(ofGetWidth()+500, ofGetHeight()/2.0));
    cloudEmitter.life = 40;
    cloudEmitter.lifeSpread = 3;
    cloudEmitter.posSpread = ofVec2f(100, ofGetHeight());
    cloudEmitter.velocityStart= ofVec2f(-180, 0);
    cloudEmitter.velSpread = ofVec2f(60,20);
    cloudEmitter.size = 600;
    cloudEmitter.sizeSpread = 140;
    cloudEmitter.rotVel = ofVec3f(0,0,2.2);
    cloudEmitter.rotVelSpread = ofVec3f(0,0,4.04);
    cloudEmitter.rotSpread = ofVec3f(0,0,180.58);
    cloudEmitter.numPars = 1;
}
