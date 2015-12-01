#pragma once
#include "ofMain.h"


enum SoundItem{
    ACCELERATOR_WINDUP, ALIENHIT, POWERDOWN, EXPLODE_L, HEARTBEAT,
};

class SoundLibrary
{
public:
    static void playSound(SoundItem item);
    vector<ofSoundPlayer> tracks;
private:
    SoundLibrary();
};
