#pragma once

#include "ofMain.h"

extern int tick;
class Track;


extern unique_ptr<Track> currentTrack;
class ofxAudioDecoder;
extern unique_ptr<ofxAudioDecoder> globalDecoder;

extern int currentScore;

class Player;
extern Player player;
