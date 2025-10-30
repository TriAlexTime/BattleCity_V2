#include "PlayerTank.h"
#include "Map.h"
#include <GL/glut.h>

PlayerTank::PlayerTank(float startX, float startY, int startDir)
    : Tank(startX, startY, startDir) {
}

void PlayerTank::handleInput(int key, bool isPressed) {
    if (isPressed) {
        this->isMoving = true;
        switch (key) {
            // Используй константы GLUT
        case GLUT_KEY_UP: this->direction = 1; break;
        case GLUT_KEY_DOWN: this->direction = 2; break;
        case GLUT_KEY_LEFT: this->direction = 3; break;
        case GLUT_KEY_RIGHT: this->direction = 4; break;
        default: this->isMoving = false; break;
        }
    }
    else {
        this->isMoving = false;
    }
}

void PlayerTank::update(float deltaTime, const Map& map) {
    Tank::update(deltaTime, map);
}

void PlayerTank::draw() {
    float playerColor1[3] = { 0.65f, 0.45f, 0.1f };
    float playerColor2[3] = { 1.0f, 0.7f, 0.2f };
    float playerColor3[3] = { 1.0f, 0.8f, 0.3f };
    Tank::draw(playerColor1, playerColor2, playerColor3);
}
