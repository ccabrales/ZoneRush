#include "ofApp.h"

int tick = 0;
Track* currentTrack = NULL;

//--------------------------------------------------------------
void ofApp::setup(){
    // set the size of the window
    ofSetWindowShape(750, 250);
    ofSetEscapeQuitsApp(false);
//    ofSetFullscreen(true);
    
    game_state = GAME; //TODO: CHANGE ME BACK TO START
    player_image.load("ship.png");
    player.setup(&player_image);
    
    musicDecoder.load("music.mp3");
    currentTrack = new Track(&musicDecoder);
    tv.setup(currentTrack);
    
    //int sampleRate = 44100; int bufferSize = 256; int nBuffers = 4;
    ofSoundStreamSetup(2, 0, this);
}

void ofApp::exit(){
    ofSoundStreamStop();
    ofSoundStreamClose();
}

void ofApp::audioOut(float * input, int bufferSize, int nChannels){
    copy(musicDecoder.getRawSamples().begin()+tick*bufferSize*nChannels, musicDecoder.getRawSamples().begin()+tick*bufferSize*nChannels+bufferSize*nChannels, input);
    tick ++;
    if(tick*nChannels*bufferSize > musicDecoder.getNumSamples()){
        tick = 0;
    }
    tv.updateAudio(input, bufferSize, nChannels);
}

void ofApp::audioIn(float* input, int bufferSize, int nChannels){}

//--------------------------------------------------------------
void ofApp::update(){
    Track::Data* d = currentTrack->readData(tick);
    tv.update(d);
    
    switch (game_state) {
        case START:
            break;
        case GAME:
            player.update();
            break;
        default:
            break;
    }
    //TEMPORARY ENEMY SPAWNER LOL
    if(d->onBeat){
        vector<EnemyPtr>* newEnemies = EnemyFactory::makeGroup(1, 2, 0);
        for(int i = 0; i < newEnemies->size(); i++){
            enemyList.push_back((*newEnemies)[i]);
        }
        delete newEnemies;
    }
    for(int i=0; i < enemyList.size(); i++){
        enemyList[i]->update();
    }
    
    delete d;
}

//--------------------------------------------------------------
void ofApp::draw(){
    // update beat info
    tv.draw(tick);

    
    ofPushStyle();
    switch (game_state) {
        case START:
            break;
        case GAME:
            player.draw();
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

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (game_state == GAME) {
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
    if (game_state == START) { //TODO: Change this to only change the game state when we want it to --> selecting "play" from the menu
        game_state = GAME;
    } else if (game_state == GAME) {
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

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
