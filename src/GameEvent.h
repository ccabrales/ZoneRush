#pragma once
#include "ofMain.h"
#include "track.h"

class GameEvent : public ofEventArgs {
    
public:
    GameEvent();
    
    static ofFastEvent<Track::Data&> onset;
    static ofFastEvent<Track::Data&> beat;
};

