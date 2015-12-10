#include "BulletType.h"

const BulletType* BulletLibrary::getWeaponInfo(int grade, bool player){
    static BulletLibrary inst;
    if (player) {
        return &(inst.playerBullets[grade]);
    } else {
        return &(inst.bullets[grade]);
    }
};

BulletLibrary::BulletLibrary(){
    bullets.push_back( BulletType { 1.0, 0,
        &ofxAssets::image("b1"), STRAIGHT, PLAYER
    });
    bullets.push_back( BulletType { 2.0, 0,
        &ofxAssets::image("b2"), TWOSHOT, PLAYER
    });
    bullets.push_back( BulletType { 3.0, 0,
        &ofxAssets::image("b3"), THREESHOT, PLAYER
    });
    bullets.push_back( BulletType { 5.0, 0,
        &ofxAssets::image("b4"), CLOUD, DOWNLANE
    });
    bullets.push_back( BulletType { 0.9, 0,
        &ofxAssets::image("b2"), STRAIGHT, DOWNLANE
    });
    bullets.push_back( BulletType { 1.5, 0,
        &ofxAssets::image("b4"), TWOSHOT, DOWNLANE
    });
    bullets.push_back( BulletType { 2.5, 0,
        &ofxAssets::image("b1"), THREESHOT, DOWNLANE
    });
    
    
    playerBullets.push_back( BulletType { 1.0, 1,
        &ofxAssets::image("b1"), STRAIGHT, DOWNLANE
    });
    playerBullets.push_back( BulletType { 2.0, 2,
        &ofxAssets::image("b2"), TWOSHOT, DOWNLANE
    });
    playerBullets.push_back( BulletType { 3.0, 3,
        &ofxAssets::image("b3"), THREESHOT, DOWNLANE
    });
    playerBullets.push_back( BulletType { 5.0, 5,
        &ofxAssets::image("b4"), CLOUD, DOWNLANE
    });
};

