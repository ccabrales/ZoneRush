#pragma once

#include <stdio.h>
#include "ofMain.h"

struct BulletType {
    ofVec2f aim;
    float angle; //radians or degrees tbd.
    ofImage* img;
};


class BulletLibrary
{
public:
    static BulletLibrary& getInstance( ){
        static BulletLibrary inst;
        return inst;
    };
    
    BulletType StraightShot[1] = {BulletType{ ofVec2f(-1.0, 0.0), 0, new ofImage() }};
    BulletType ClusterShot[3] = {
        BulletType{ofVec2f(-0.4, 0.4), 45, new ofImage() },
        BulletType{ofVec2f(-0.5656, 0), 0, new ofImage() },
        BulletType{ofVec2f(-0.4, -0.4), -45, new ofImage() }
    };
    

private:
    BulletLibrary();
    
    static BulletLibrary* instance;
    
};


class Logger{
public:
    static Logger* Instance();
    bool openLogFile(std::string logFile);
    void writeToLogFile();
    bool closeLogFile();
    
private:
    Logger(){};  // Private so that it can  not be called
    Logger(Logger const&){};             // copy constructor is private
    Logger& operator=(Logger const&){};  // assignment operator is private
    static Logger* m_pInstance;
};
