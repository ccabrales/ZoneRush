#pragma once

#include <stdio.h>
#include "ofMain.h"

enum BULLET{
    
};

class ImageLibrary
{
public:
    static ofImage* getBullet(int imageNumber);
    static ofImage* getShip(int shipNumber);
    static ofImage* getOtherResource(int resourceNum);
    static int size();
private:
    vector<ofImage*> images;
    ImageLibrary();
};
