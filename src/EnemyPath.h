#pragma once

#include <stdio.h>
#include <ofMain.h>

#define P(x,y) ofPoint(x,y)

class PathLibrary
{
public:
    static const ofPolyline* getPath(int grade);
    static int size();
    vector<ofPolyline> paths;
    
private:
    PathLibrary();
};
