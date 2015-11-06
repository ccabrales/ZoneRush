#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // set the size of the window
    ofSetWindowShape(750, 250);
    
    musicDecoder.load("music.mp3");
    currentTrack = new Track(&musicDecoder);

    int nOutputs = 2;
    int nInputs = 0;
    //int sampleRate = 44100;
    //int bufferSize = 256;
    //int nBuffers = 4;

    // setup onset object
//    onset.setup();
    //onset.setup("mkl", 2 * bufferSize, bufferSize, sampleRate);

    // setup pitch object
//    pitch.setup();
    //pitch.setup("yinfft", 8 * bufferSize, bufferSize, sampleRate);

    // setup beat object
//    beat.setup();
    //beat.setup("default", 2 * bufferSize, bufferSize, samplerate);
    //

    // setup mel bands object
    bands.setup();

    ofSoundStreamSetup(nOutputs, nInputs, this);

    for (int i = 0; i < 40; i++) {
        bandPlot.addVertex( 50 + i * 650 / 40., 240 - 100 * bands.energies[i]);
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
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    Track::Data d = currentTrack->readData(tick);
    
    // update beat info
    ofSetColor(ofColor::black);
    ofDrawBitmapString("beat", ofPoint(90,150));

    if (d.onBeat) {
        ofSetColor(ofColor::green);
        ofRect(90,150,50,50);
        
        beatDecay = 1.0;
    }else{
        ofSetColor(0,255,0,beatDecay*255);
        ofRect(90,150,50,50);

    }
    
    ofSetColor(ofColor::black);
    ofDrawBitmapString("onset. #3, #2, #1", ofPoint(270,150));

    // update onset info
    for(int i = 0; i < 3; i++){
        if ((d.onsets & (1<<i)) > 0) {
            ofSetColor(ofColor::red);
            onsetDecay[i] = ((float)(d.onsets & (1<<i)));
            ofRect(270 + i*90,150,50,50);
        } else {
            ofSetColor(255,0,0,onsetDecay[i]*255);
            ofRect(270 + i*90,150,50,50);
        }
    }
    ofSetColor(ofColor::blue);
    ofRect(330, 110, 20.0*(onsetDecay[0]+onsetDecay[1]+onsetDecay[2]), 10);

    ofSetColor(ofColor::orange);
    ofSetLineWidth(3.);
    //bandPlot.clear();
    for (int i = 0; i < bandPlot.size(); i++) {
        bandPlot[i].y = 240 - 100 * bands.energies[i];
    }
    bandPlot.draw();
    
    ofSetColor(ofColor::black);
    ofDrawBitmapString(Track::toString(d), ofPoint(20,20));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
