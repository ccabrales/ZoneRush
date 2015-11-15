#include "ofApp.h"

int tick = 0;
Track* currentTrack = NULL;
ofxAudioDecoder* globalDecoder = NULL;



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
    
    game_state = START;
    
    player_image.load("ship.png");
    player.setup(&player_image);
    
    if(globalDecoder != NULL) delete globalDecoder;
    globalDecoder = new ofxAudioDecoder();
    
    globalDecoder->load("music.mp3");
    currentTrack = new Track(globalDecoder);
    tv.setup(currentTrack);
    
    //int sampleRate = 44100; int bufferSize = 256; int nBuffers = 4;
    ofSoundStreamSetup(2, 0, this);
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>();
    
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
    
    Track::Data* d;
    switch (game_state) {
        case START:
            titleScene->backgroundUpdate(d);
            break;
        case LOAD:
            titleScene->backgroundUpdate(d);
            //Check for when the thread is done
            if (audioLoader->isDone) {
                //clean up thread info?
                //Check the audioLoader error information to see if should go back to title scene and display error
                //Set the new music decoder and necessary information
                //Do transition from loading screen to game state --> probably if statement?
                //Change game state to GAME
                //Make sure music doesn't start playing until the game is totally loaded, and transition is finished
                game_state = GAME;
//                titleScene->setLoading();
                titleScene->setLoading(titleScene->TITLE);
            }
            break;
        case GAME:
            player.update();
            d = currentTrack->readData(tick);
            tv.update(d);
            break;
        default:
            break;
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

    
    ofPushStyle();
    switch (game_state) {
        case START:
        case LOAD:
            titleScene->draw();
            break;
        case GAME:
//            player.draw();
            tv.draw(tick);

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
                    audioLoader = unique_ptr<AudioLoader>(new AudioLoader());
                    audioLoader->start(res);
                    game_state = LOAD;
                    titleScene->setLoading(titleScene->TRANSITION);
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
