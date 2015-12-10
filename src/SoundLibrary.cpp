#include "SoundLibrary.h"

void SoundLibrary::playSound(SoundItem item){
    static SoundLibrary inst;
    inst.tracks[item].play();
};
SoundLibrary::SoundLibrary(){
    vector<string> filenames = {"accelerator-wind.aif", "alienhit.wav", "big-powerdown.aif", "explode2.wav", "heartbeat-superverb.aif", "laser-blast.wav", "lexplode.wav", "lowblup.aif", "pickup.wav", "seismic_charge.wav", "zuperzap.aif", "power-up-ray.wav"};
    vector<float> volumes = {1, 1, 1, 1, 1, 0.6, 0.6, 1, 1, 1, 1, 1};
    if(filenames.size() != SoundItem::__COUNT){
        cout << "Unexpected Sound Library setup. Failing..." << endl;
        assert(false);
    }
    for(int i= 0; i < SoundItem::__COUNT; i++ ){
        tracks.push_back(ofSoundPlayer());
        tracks.back().load("sfx/"+filenames[i]);
        tracks.back().setVolume(volumes[i]);
        tracks.back().setMultiPlay(true);
        tracks.back().stop();
    }
    
};
