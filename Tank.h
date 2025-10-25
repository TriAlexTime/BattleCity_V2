#pragma once

#include "GameObject.h"
#include <memory> // ��� std::unique_ptr (����� ���������)

class Projectile;

class Tank : public GameObject {
public:
    Tank(float startX, float startY, int startDir);

    // 'override' - ��� ��������� �����������, ��� �� ��������� �������������� ����� �������� ������.

    void update(float deltaTime) override;
    void draw() override;
    bool isAlive() const override;


    void takeDamage(int dmg);

    /*
     �������� ����������.
     @return ����� ��������� �� ����� ������, ���� ������� �������� (��� �����������), ����� nullptr.
    */
    std::unique_ptr<Projectile> fire();

    // ������ ��� ��������
    int getHealth() const { return health; }

protected:

    int health = 1;
    int damage = 1;
    float speed = 8.0f;
    bool isMoving = false;

    // ������ �����������, ����� ���� �� ������� ��������
    float fireCooldown = 0.0f;
    const float fireRate = 0.5f; // 1 ������� ������ 0.5 ��� (�������)
};