#pragma once

#include <stdio.h>
#include "ofMain.h"
#include "globals.h"
#include "enemy.h"

class SpawnController
{
public:
    
    static bool shouldSpawn();
    static bool spawnEnemyCluster(vector<EnemyPtr> * currentEnemies);
    
private:
    static float relativeLikeliness;
    //declares a relative likeliness value of a next spawn.
    static const int lookaheadFrames = 1;
    //dynamic adjustment frame for lookahead.
    SpawnController();
};
