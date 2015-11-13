#include "TitleButtons.h"

void PlayButton::setup() {
    printf("MyTestObject::setup() - hello!\n");
    disableMouseEvents();
    enableKeyEvents();
}

void PlayButton::exit() {
    printf("MyTestObject::exit() - goodbye!\n");
}

void PlayButton::update() {
    
}

void PlayButton::draw() {
    
}

void PlayButton::onRollOver(int x, int y) {
    printf("MyTestObject::onRollOver(x: %i, y: %i)\n", x, y);
}

void PlayButton::onRollOut() {
    printf("MyTestObject::onRollOut()\n");
}

void PlayButton::onPress(int x, int y, int button) {
    printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
}

void PlayButton::onRelease(int x, int y, int button) {
    printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
}

void PlayButton::keyPressed(int key) {
    printf("MyTestObject::keyPressed(key: %i)\n", key);
}

void PlayButton::keyReleased(int key) {
    printf("MyTestObject::keyReleased(key: %i)\n", key);
}