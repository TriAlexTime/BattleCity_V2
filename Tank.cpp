#include "Tank.h"
#include "Projectile.h"
#include "Map.h"
#include <GL/glut.h>
#include "Game.h"
#include "Graphics.h"

// ����������� �������������� ���� � ��������� �����������
Tank::Tank(float startX, float startY, int startDir) {
    this->x = startX;
    this->y = startY;
    this->direction = startDir;

    // ��������� �������� �� ���������
    this->health = 1;
    this->damage = 1;
    this->speed = 80.0f; // �������� � �������� � �������
    this->isMoving = false;
    this->fireCooldown = 0.0f;
}

void Tank::update(float deltaTime, const Map& map) {
    // 1. ��������� ������������
    if (fireCooldown > 0.0f) {
        fireCooldown -= deltaTime;
    }

    // 2. ��������� ���������
    if (isMoving) {
        // ������ ��� ���������, ��������� ������������ � ������
        if (map.checkTankCollision(this->x, this->y, this->direction)) {
            switch (direction) {
            case 1: // UP
                y -= speed * deltaTime;
                break;
            case 2: // DOWN
                y += speed * deltaTime;
                break;
            case 3: // LEFT
                x -= speed * deltaTime;
                break;
            case 4: // RIGHT
                x += speed * deltaTime;
                break;
            }
        }
    }
}

// ����� ��� ��������
std::unique_ptr<Projectile> Tank::fire(Owner owner) {
    // �������� ����� ������ ���� ����������� ���������
    if (fireCooldown <= 0.0f) {
        fireCooldown = fireRate; // ���������� ������ �����������

        // ������������ ��������� ������� �������, ����� �� ��������� � ���� �����, � �� � ��� ������
        float projX = x;
        float projY = y;
        float offset = 22.0f; // �������� �� ������ �����

        switch (direction) {
        case 1: projY -= offset; break; // UP
        case 2: projY += offset; break; // DOWN
        case 3: projX -= offset; break; // LEFT
        case 4: projX += offset; break; // RIGHT
        }

        // ������� ����� ������ ������� � ������� ������ ���������
        return std::make_unique<Projectile>(projX, projY, direction, this->damage, owner);
    }

    // ���� �������� ������, ���������� ������ ���������
    return nullptr;
}


void Tank::takeDamage(int dmg) {
    if (health > 0) {
        health -= dmg;
    }
}

bool Tank::isAlive() const {
    return health > 0;
}

// ������� draw() ����� ����� ����������� � �������������� ����������� ����������.
// ��� ����� ������ �� drawTank() �� battlecity.c
void Tank::draw(float color1[], float color2[], float color3[]) {
    if (!isAlive()) return;

    // ������ ������ ��������� �� drawTank() � battlecity.c
    // ��� ����� (������� ��� ����������) ������������ �� �����
    int type = (this->damage == 1) ? 1 : 2;

    if (type == 1) { // ������� ����
        glColor3fv(color2);
        drawRectangle(x - 14, y - 9, x + 14, y + 9);
        drawRectangle(x - 9, y - 14, x + 9, y + 14);

        glColor3fv(color1);
        drawRectangle(x - 8, y - 6, x + 8, y + 6);
        drawRectangle(x - 6, y - 8, x + 6, y + 8);

        if (direction <= 2) { // UP or DOWN
            drawRectangle(x - 20, y - 18, x - 14, y + 18);
            drawRectangle(x + 14, y - 18, x + 20, y + 18);
            glColor3fv(color3);
            if (direction == 1) drawRectangle(x + 3, y - 21, x - 3, y - 9); // UP
            else drawRectangle(x + 3, y + 21, x - 3, y + 9); // DOWN
        }
        else { // LEFT or RIGHT
            drawRectangle(x - 18, y - 20, x + 18, y - 14); // ������ � ���������, ������ ���� y-20, y-14
            drawRectangle(x - 18, y + 14, x + 18, y + 20);
            glColor3fv(color3);
            if (direction == 3) drawRectangle(x - 21, y - 3, x - 9, y + 3); // LEFT
            else drawRectangle(x + 21, y - 3, x + 9, y + 3); // RIGHT
        }
    }
    else { // ���������� ���� (type == 2)
        if (direction <= 2) { // UP or DOWN
            glColor3fv(color2);
            drawRectangle(x - 14, y - 18, x + 14, y + 18);
            glColor3fv(color1);
            drawRectangle(x - 8, y - 9, x + 8, y + 9);
            drawRectangle(x - 6, y - 11, x + 6, y + 11);
            drawRectangle(x - 20, y - 20, x - 14, y + 20);
            drawRectangle(x + 14, y - 20, x + 20, y + 20);
            glColor3fv(color3);
            if (direction == 1) { // UP
                drawRectangle(x + 5, y - 21, x + 1, y - 11);
                drawRectangle(x - 5, y - 21, x - 1, y - 11);
            }
            else { // DOWN
                drawRectangle(x + 5, y + 21, x + 1, y + 11);
                drawRectangle(x - 5, y + 21, x - 1, y + 11);
            }
        }
        else { // LEFT or RIGHT
            glColor3fv(color2);
            drawRectangle(x - 18, y - 14, x + 18, y + 14);
            glColor3fv(color1);
            drawRectangle(x - 9, y - 8, x + 9, y + 8);
            drawRectangle(x - 11, y - 6, x + 11, y + 6);
            drawRectangle(x - 20, y - 20, x + 20, y - 14); // ������ � ���������
            drawRectangle(x - 20, y + 14, x + 20, y + 20);
            glColor3fv(color3);
            if (direction == 3) { // LEFT
                drawRectangle(x - 21, y - 5, x - 11, y - 1);
                drawRectangle(x - 21, y + 5, x - 11, y + 1);
            }
            else { // RIGHT
                drawRectangle(x + 21, y - 5, x + 11, y - 1);
                drawRectangle(x + 21, y + 5, x + 11, y + 1);
            }
        }
    }
}

void Tank::upgrade() {
    if (this->damage < 2) {
        this->damage = 2;
    }
}

void Tank::setPosition(float newX, float newY) {
    this->x = newX;
    this->y = newY;
}

bool Tank::getIsMoving() const {
    return this->isMoving;
}