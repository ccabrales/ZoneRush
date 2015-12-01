#include <stdio.h>
#include "Player.h"

void Player::setup(ofImage * _img) {
    pos.x = ofGetWidth() / 10.0;
    pos.y = ofGetHeight() / 2.0;
    
    img = _img;

    width = img->getWidth();
    height = img->getHeight();
    speed = 130;
    lives = 3;
    bulletDamage = 1;
    
    gunPos.x = pos.x + width/2.0;
    gunPos.y = pos.y + height/2.0;

    bulletCd = 0;
    moveHitbox();

    upgradeGun(0);

    is_left_pressed = false;
    is_right_pressed = false;
    is_down_pressed = false;
    is_up_pressed = false;

    emitter.setPosition(pos);
    emitter.setVelocity(ofVec3f(-310,0.0));
    emitter.posSpread = ofVec3f(0,-10);
    emitter.velSpread = ofVec3f(120,20);
    emitter.life = 0.5;
    emitter.lifeSpread = 0;
    emitter.numPars = 3;
    emitter.color = ofColor(220,210,130);
    emitter.colorSpread = ofColor(70,70,70);
}

void Player::upgradeGun(int grade){
    currentGun = BulletLibrary::getWeaponInfo(grade);
    gunEmitter.life = 400;
    gunEmitter.size = 10;
    switch(currentGun->firePattern){
        case STRAIGHT:
            gunEmitter.numPars = 1;
            break;
        case CLOUD:
            gunEmitter.numPars = 30;
            gunEmitter.velSpread = ofVec3f(BulletSpeed*2.0, BulletSpeed*2.0);
            break;
        case THREESHOT:
            gunEmitter.numPars = 3;
            gunEmitter.velSpread = ofVec3f(0.1, BulletSpeed/5.0);
            break;
        case TWOSHOT:
            gunEmitter.numPars = 2;
            gunEmitter.velSpread = ofVec3f(0.1, BulletSpeed/8.0);
            break;
        default:
            gunEmitter.numPars = 1;
            break;
    }
}

void Player::update(float frameTime) {
    if (is_left_pressed) {
        pos.x -= speed*frameTime;
        gunPos.x -= speed*frameTime;
    }

    if (is_right_pressed) {
        pos.x += speed*frameTime;
        gunPos.x += speed*frameTime;;
    }

    if (is_up_pressed) {
        pos.y -= speed*frameTime;
        gunPos.y -= speed*frameTime;
    }

    if (is_down_pressed) {
        pos.y += speed*frameTime;
        gunPos.y += speed*frameTime;
    }
    checkBounds();
    emitter.setPosition(pos + ofVec2f(0, img->getHeight() / 2.0));
    moveHitbox();

    bulletCd -= frameTime;
}

void Player::draw() {
    img->draw(pos);
    //ofDrawRectangle(hitbox);
}

void Player::shoot(GreedyParticleSystem* playerBullet) {
    if(bulletCd < 0){
        gunEmitter.setPosition(gunPos);
        gunEmitter.setVelocity(ofVec3f(BulletSpeed*1.2, 0));

        bulletCd = currentGun->cd / 8.0;
        playerBullet->addParticles(gunEmitter, currentGun->texture);
    }
}

void Player::checkBounds() { //make sure player doesn't go out of screen, and reposition if so
    if (pos.x < 0) {
        pos.x = 0;
        gunPos.x = pos.x + width/2.0;
    }
    if (pos.x > ofGetWidth() - img->getWidth()) {
        pos.x = ofGetWidth() - img->getWidth();
        gunPos.x = pos.x + width/2.0;
    }
    if (pos.y < 0) {
        pos.y = 0;
        gunPos.y = pos.y + height/2.0;
    }
    if (pos.y > ofGetHeight() - img->getHeight()) {
        pos.y = ofGetHeight() - img->getHeight();
        gunPos.y = pos.y + height/2.0;
    }
}

void Player::moveHitbox() {
    hitbox.set(pos.x + (width/4.0), pos.y + (height/4.0), width / 2.0, height/2.0);
}

//bool Player::check_can_shoot() {
//    return false;
//}
