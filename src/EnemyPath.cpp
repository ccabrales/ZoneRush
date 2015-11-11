//
//  EnemyPath.cpp
//  ZoneRushMaxima
//
//  Created by Xiaonan Tong on 11/10/15.
//
//

#include "EnemyPath.h"

PathLibrary::PathLibrary(){
    
    paths.push_back(ofPolyline({P(1.1,0.5), P(-0.1, 0.5)}));
    //straight across.
    paths.push_back(ofPolyline({P(1.1,0.5), P(1.0, 0.4), P(0.94, 0.6), P(0.9, 0.2), P(0.3, 0.2), P(-0.1, 0.2)}));
    //go up and across.
    paths.push_back(ofPolyline({P(1.1,0.5), P(1.0, 0.6), P(0.94, 0.4), P(0.9, 0.8), P(0.3, 0.8), P(-0.1, 0.8)}));
    //go down and across
    paths.push_back(ofPolyline({P(1.1,0.5), P(1.0, 0.4), P(0.9, 0.2), P(0.3, 0.2)}));
    paths.push_back(ofPolyline({P(1.1,0.5), P(1.0, 0.4), P(0.9, 0.2), P(0.3, 0.2)}));
    
}

int PathLibrary::size(){
    static PathLibrary inst;
    return inst.paths.size();
}

const ofPolyline* PathLibrary::getPath(int grade){
    static PathLibrary inst;
    return &(inst.paths[grade]);
};