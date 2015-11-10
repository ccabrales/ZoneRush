#include <stdio.h>
#include "player.h"

void Player::setup(ofImage * _img) {
    pos.x = ofGetWidth() / 10.0;
    pos.y = ofGetHeight() / 2.0;
    
    img = _img;
    
    width = img->getWidth();
    height = img->getHeight();
    speed = 3;
    lives = 3;
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