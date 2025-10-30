#include "EnemyTank.h"
#include "Map.h"
#include <cstdlib>

// ����������� ������ �������� ����������� �������� ������
EnemyTank::EnemyTank(float startX, float startY, int startDir)
    : Tank(startX, startY, startDir) {
    // ����� ����� ���������������� ����������� ��� �� ����������, ���� �����
}

void EnemyTank::update(float deltaTime, const Map& map) {
    // �������� update �������� ������ Tank.
    // �� �������� �� ������ ����������� (fireCooldown) � ������� ������ ��������.
    Tank::update(deltaTime, map);

    aiTimer += deltaTime;

    // --- ������ �������� ������� �� ---

    // ������ 0.8 ������� ���� "������", ��� ������ ������
    if (aiTimer > 0.8f) {
        aiTimer = 0.0f; // ���������� ������

        // ���������, �� �������� �� �� � �����
        if (!map.checkTankCollision(this->x, this->y, this->direction)) {
            // ���� ������������. �������� ����� ��������� �����������.
            int oldDirection = this->direction;
            // ����, ����� �������������� �������� ����� �����������
            while (this->direction == oldDirection) {
                this->direction = (rand() % 4) + 1; // ��������� ����������� �� 1 �� 4
            }
        }
        else {
            // ���� ��������. ���� 20% ����, ��� ���� ��� ����� ������ �����������.
            // ��� ������ ��������� ����� �������������.
            if ((rand() % 100) < 20) {
                this->direction = (rand() % 4) + 1;
            }
        }
    }

    // --- ������ �������� ---

    // �������� ��������� � ������� �����������
    if (map.checkTankCollision(this->x, this->y, this->direction)) {
        this->isMoving = true;
    }
    else {
        this->isMoving = false;
    }

    // ������ �������� ����� ����������� �� �������� �������� ����� (� ������ Game),
    // ������� ����� �������� ����� fire(), ����� fireCooldown <= 0.
    // ����� �� ������ ��������� ���������� �����.
}

void EnemyTank::draw() {
    float enemyColor1[3] = { 0.35f, 0.35f, 0.35f };
    float enemyColor2[3] = { 0.65f, 0.65f, 0.65f };
    float enemyColor3[3] = { 0.85f, 0.85f, 0.85f };
    Tank::draw(enemyColor1, enemyColor2, enemyColor3);
}