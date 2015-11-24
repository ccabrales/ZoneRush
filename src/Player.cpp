#include <stdio.h>
#include "Player.h"

void Player::setup(ofImage * _img) {
    pos.x = ofGetWidth() / 10.0;
    pos.y = ofGetHeight() / 2.0;
    
    img = _img;
    
    width = img->getWidth();
    height = img->getHeight();
    speed = 3;
    lives = 3;
    
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

void Player::update() {
    if (is_left_pressed) {
        pos.x -= speed;
    }
    
    if (is_right_pressed) {
        pos.x += speed;
    }
    
    if (is_up_pressed) {
        pos.y -= speed;
    }
    
    if (is_down_pressed) {
        pos.y += speed;
    }
    emitter.setPosition(pos + ofVec2f(0, img->getHeight() / 2.0));
}

void Player::draw() {
    img->draw(pos);
}

void Player::shoot() {
    
}

void Player::calculate_movement() {
    
}

//bool Player::check_can_shoot() {
//    return false;
//}