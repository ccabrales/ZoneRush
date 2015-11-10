#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // set the size of the window
    ofSetWindowShape(750, 250);
    
    game_state = START;
//    player_image.load("ship.png");
//    player.setup(&player_image);
    
    musicDecoder.load("music.mp3");
    currentTrack = new Track(&musicDecoder);

    int nOutputs = 2;
    int nInputs = 0;
    //int sampleRate = 44100;
    //int bufferSize = 256;
    //int nBuffers = 4;

    bands.setup();

    ofSoundStreamSetup(nOutputs, nInputs, this);

    for (int i = 0; i < 40; i++) {
        bandPlot.addVertex( 0 + i * 2, 240 - 100 * bands.energies[i]);
    }
    
    for(int i = 0; i< 90; i++){
        pitchPlot.addVertex(0, 90 - i );
        intensityPlot.addVertex(0, 90-i);
    }
}

void ofApp::exit(){
    ofSoundStreamStop();
    ofSoundStreamClose();
}

void ofApp::audioOut(float * input, int bufferSize, int nChannels){
    
    copy(musicDecoder.getRawSamples().begin()+tick*bufferSize*nChannels, musicDecoder.getRawSamples().begin()+tick*bufferSize*nChannels+bufferSize*nChannels, input);
    tick ++;
    if(tick*nChannels*bufferSize > musicDecoder.getNumSamples()){
        useMic = true;
        tick = 0;
    }
    bands.audioIn(input, bufferSize, nChannels);
}

void ofApp::audioIn(float* input, int bufferSize, int nChannels)
{
    
}


//--------------------------------------------------------------
void ofApp::update(){
    onsetDecay[0] *= 0.8;
    onsetDecay[1] *= 0.8;
    onsetDecay[2] *= 0.8;
    beatDecay *= 0.9;

    Track::Data d = currentTrack->readData(tick);
    if (d.onBeat) {
        beatDecay = 1.0;
    }
    
    for(int i = 0; i < 3; i++){
        if ((d.onsets & (1<<i)) > 0) {
            ofSetColor(ofColor::red);
            onsetDecay[i] = ((float)(d.onsets & (1<<i)));
        } else {
            ofSetColor(255,0,0,onsetDecay[i]*255);
        }
        ofRect(10,35+13*i,30,10);
    }

    // ofDrawBitmapString(Track::toString(d), ofPoint(10,ofGetHeight()-2));
    
    switch (game_state) {
        case START:
            break;
        case GAME:
            player.update();
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    int wh = ofGetHeight();
        
    // update beat info
    ofSetColor(ofColor::black);
    ofDrawBitmapString("Beat", ofPoint(10,10));

    ofSetColor(0,255,0,beatDecay*255);
    ofRect(10,10,30,13);

    // update onset info
    for(int i = 0; i < 3; i++){
        ofSetColor(255,0,0,onsetDecay[i]*255);
        ofRect(10,35+13*i,30,10);
    }

    ofSetColor(ofColor::black);

    ofDrawBitmapString("onset.\n#3\n#2\n#1", ofPoint(10,30));

    ofSetColor(ofColor::blue);
    ofRect(10, 80, 10, 10.0*(onsetDecay[0]+onsetDecay[1]+onsetDecay[2]));

    ofSetLineWidth(2.);
    for (int i = 0; i < bandPlot.size(); i++) {
        bandPlot[i].y = wh - 100 * bands.energies[i];
    }
    
    for (int i = 0; i < 90 && tick-10+i > 0 && tick-10+i < currentTrack->frameData.size(); i++){
        Track::Data& dp = currentTrack->frameData[tick-10+i];
        
        pitchPlot[i].x = -20 + (2 * dp.pitch);
        intensityPlot[i].x = (30.*(1.0-dp.intensity));
        if(dp.onBeat)
            ofLine(intensityPlot[i].x-7, intensityPlot[i].y, intensityPlot[i].x+7, intensityPlot[i].y);
        if(dp.onsets > 0)
            ofCircle(pitchPlot[i].x, 90 - i, 3);
        
    }
    ofSetColor(ofColor::orange);
    bandPlot.draw();
    
    ofSetColor(ofColor::indianRed);
//    pitchPlot.draw();
    ofDrawBitmapString("pitch", ofPoint(pitchPlot[10]));

    ofSetColor(ofColor::orchid);
    intensityPlot.draw();
    ofDrawBitmapString("power", ofPoint(intensityPlot[10]));

    ofSetColor(ofColor::black);
    ofLine(10, 80, 140, 80);
    
    
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
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (game_state == GAME) {
        switch (key) {
            case OF_KEY_LEFT:
                player.is_left_pressed = true;
                break;
            case OF_KEY_RIGHT:
                player.is_right_pressed = true;
                break;
            case OF_KEY_UP:
                player.is_up_pressed = true;
                break;
            case OF_KEY_DOWN:
                player.is_down_pressed = true;
                break;
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
                player.is_left_pressed = false;
                break;
            case OF_KEY_RIGHT:
                player.is_right_pressed = false;
                break;
            case OF_KEY_UP:
                player.is_up_pressed = false;
                break;
            case OF_KEY_DOWN:
                player.is_down_pressed = false;
                break;
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
