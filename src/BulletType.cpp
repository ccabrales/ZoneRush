#include "BulletType.h"

const BulletType* BulletLibrary::getWeaponInfo(int grade){
    static BulletLibrary inst;
    return &(inst.bullets[grade]);
};

BulletLibrary::BulletLibrary(){
    bullets.push_back( BulletType {
        &ofxAssets::image("b1"), STRAIGHT, PLAYER
    });
    bullets.push_back( BulletType {
        &ofxAssets::image("b2"), TWOSHOT, PLAYER
    });
    bullets.push_back( BulletType {
        &ofxAssets::image("b3"), THREESHOT, PLAYER
    });
    bullets.push_back( BulletType {
        &ofxAssets::image("b4"), CLOUD, DOWNLANE
    });
    bullets.push_back( BulletType {
        &ofxAssets::image("b2"), STRAIGHT, DOWNLANE
    });
    bullets.push_back( BulletType {
        &ofxAssets::image("b4"), TWOSHOT, DOWNLANE
    });
    bullets.push_back( BulletType {
        &ofxAssets::image("b1"), THREESHOT, DOWNLANE
    });
};