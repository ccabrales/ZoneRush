//
//  GameEvent.cpp
//  customEventExample
//
//  Created by Todd Vanderlin on 2/25/12.
//  Copyright (c) 2012 vanderlin.cc. All rights reserved.
//

#include "GameEvent.h"

GameEvent::GameEvent(){
    //doesn't do anything
}

ofFastEvent<Track::Data&> GameEvent::onset;
ofFastEvent<Track::Data&> GameEvent::beat;
