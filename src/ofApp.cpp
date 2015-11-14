#include "ofApp.h"

int tick = 0;
Track* currentTrack = NULL;

//--------------------------------------------------------------
void ofApp::setup(){
    // set the size of the window
    //ofSetWindowShape(750, 250);
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
    
    musicDecoder.load("music.mp3");
    currentTrack = new Track(&musicDecoder);
    tv.setup(currentTrack);
    
    //int sampleRate = 44100; int bufferSize = 256; int nBuffers = 4;
    ofSoundStreamSetup(2, 0, this);
    
    post.init(ofGetWidth(), ofGetHeight());
    post.createPass<BloomPass>();
//    post.createPass<ZoomBlurPass>();
//    bp->enable();
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
    
    
    Track::Data* d;
    switch (game_state) {
        case START:
            titleScene->backgroundUpdate(d);
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
                    if (checkFileExtension(res)) { //only move forward if we have a good file
                        game_state = GAME;
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
bool ofApp::checkFileExtension(ofFileDialogResult res){
    string fileName = res.getPath();
    string filePath = res.getPath();
    string ext = ofFilePath::getFileExt(filePath);
    game_state = LOADING;
    if (ext == "") {
        return false;
    }
    
    //Acceptable format, move along
    if (std::find(acceptableFileExts.begin(), acceptableFileExts.end(), ext)
            != acceptableFileExts.end()) {
        musicDecoder.load(filePath);
        //TODO: do not use Music Decoder here since it's just a test.
        
        if (musicDecoder.getChannels() != 2 || musicDecoder.getSampleRate() != 44100) {
            convertFileAndReload(filePath);
        }
    } else if (std::find(convertFileExts.begin(), convertFileExts.end(), ext)
                    != convertFileExts.end()) {
        convertFileAndReload(filePath);
    } else {
        ofSystemAlertDialog("Unsupported file type chosen. Please select from the following: mp3, m4a, wav, aiff, aif, flac, ogg");
        game_state = START;
        return false;
    }
    
    currentTrack = new Track(&musicDecoder);
    //TODO: similarly don't use music decoder here.
    tv.setup(currentTrack);
    return true;
}

//--------------------------------------------------------------
void ofApp::convertFileAndReload(string filePath) {
    
    string output = ofSystem("../../../sox "+filePath+" -c 2 -r 44100 -t wav ../../../data/temp.wav");
    //this file will be overwritten every time.
    cout << output << endl;
    //sort of wants to busywait here. Hmm.
    
    //now do something with the file.
    
    /*sample success: 
        ../../../sox WARN rate: rate clipped 432 samples; decrease volume?
        ../../../sox WARN dither: dither clipped 816 samples; decrease volume?
     */
    
    //it's actually really difficult to tell if it succeeded or not. My best bet is do something afterwards. like "sox stuff && echo \"yay\" " so you can check it for 'yay'. I'm sure if it fails it'll not print echo.
    
    
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
