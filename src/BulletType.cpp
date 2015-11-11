#include "BulletType.h"

const BulletType* BulletLibrary::getWeaponInfo(int grade){
    static BulletLibrary inst;
    return &(inst.bullets[grade]);
};

BulletLibrary::BulletLibrary(){
    bullets.push_back( BulletType {
        1.2, 0, 1., new ofImage()
    });
    bullets.push_back( BulletType {
        1.4, 10., 3., new ofImage()
    });
    bullets.push_back( BulletType {
        0.8, 7., 2., new ofImage()
    });
};