#pragma once
#include "ofMain.h"


enum SoundItem{
    ACCELERATOR_WINDUP, ALIENHIT, POWERDOWN, EXPLODE_LONG, HEARTBEAT, LASER, EXPLODE_LOUD, BASS_BLURP, PICKUP, SEISMIC_CHARGE, ZUPERZAP, POWERUP, __COUNT
};

class SoundLibrary
{
public:
    static void playSound(SoundItem item);
    vector<ofSoundPlayer> tracks;
private:
    SoundLibrary();
};
