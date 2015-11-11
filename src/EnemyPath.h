//
//  EnemyPath.hpp
//  ZoneRushMaxima
//
//  Created by Xiaonan Tong on 11/10/15.
//
//

#ifndef EnemyPath_hpp
#define EnemyPath_hpp

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


#endif /* EnemyPath_hpp */
