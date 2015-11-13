#include "TrackViewUtil.h"

void TrackView::setup(Track* track){
    this->track = track;
    bands.setup();
    
    for (int i = 0; i < 40; i++) {
        bandPlot.addVertex( 0 + i * 2, 240 - 100 * bands.energies[i]);
    }
    
    for(int i = 0; i< 90; i++){
        pitchPlot.addVertex(0, 90 - i );
        intensityPlot.addVertex(0, 90-i);
    }
}

void TrackView::updateAudio(float* input, int bufSize, int nC){
    bands.audioIn(input, bufSize, nC);
}

void TrackView::update(const Track::Data* d){
    onsetDecay[0] *= 0.8;
    onsetDecay[1] *= 0.8;
    onsetDecay[2] *= 0.8;
    beatDecay *= 0.9;

    if (d->onBeat) {
        beatDecay = 1.0;
    }
    
    for(int i = 0; i < 3; i++){
        if ((d->onsets & (1<<i)) > 0) {
            onsetDecay[i] = ((float)(d->onsets & (1<<i)));
        }
    }
}

void TrackView::draw(int tick){
    int wh = ofGetHeight();
    ofPushStyle();

    ofSetColor(255,255,0,beatDecay*255);
    ofRect(10,10,30,13);
    
    ofSetColor(ofColor::black);
    ofDrawBitmapString("BPM", ofPoint(10,10));
    ofDrawBitmapString(ofToString(round(track->frameData[tick].bpm)), ofPoint(10, 22));
    
    // update onset info
    for(int i = 0; i < 3; i++){
        ofSetColor(255,0,0,onsetDecay[i]*255);
        ofRect(10,39+13*i,30,10);
    }
    
    ofSetColor(ofColor::black);
    
    ofDrawBitmapString("ONSET\n#3\n#2\n#1", ofPoint(10,36));
    
    ofSetColor(ofColor::blue);
    ofRect(10, 80, 10, 10.0*(onsetDecay[0]+onsetDecay[1]+onsetDecay[2]));
    
    ofSetLineWidth(2.);
    for (int i = 0; i < bandPlot.size(); i++) {
        bandPlot[i].y = wh - 100 * bands.energies[i];
    }
    
    for (int i = 0; i < 90 && tick-10+i > 0 && tick-10+i < track->frameData.size(); i++){
        Track::Data& dp = track->frameData[tick-10+i];
        
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
    ofPopStyle();
}