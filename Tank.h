#pragma once

#include "GameObject.h"
#include <memory> // ��� std::unique_ptr (����� ���������)
#include "Projectile.h"

class Projectile;

class Tank : public GameObject {
public:
    Tank(float startX, float startY, int startDir);

    // 'override' - ��� ��������� �����������, ��� �� ��������� �������������� ����� �������� ������.

    void update(float deltaTime, const Map& map) override;
    bool isAlive() const override;

    void draw() override = 0; // ���� ����� ����� �������� �������

    void takeDamage(int dmg);

    /*
     �������� ����������.
     @return ����� ��������� �� ����� ������, ���� ������� �������� (��� �����������), ����� nullptr.
    */
    std::unique_ptr<Projectile> fire(Owner owner);

    // ������ ��� ��������
    int getHealth() const { return health; }

    // �������� ���� (����������� ����)
    void upgrade();

    // ������������� ������� �����
    void setPosition(float newX, float newY);

    // ����������, �������� �� ����
    bool getIsMoving() const;

    float getFireCooldown() const { return fireCooldown; }

protected:

    int health = 1;
    int damage = 1;
    float speed = 8.0f;
    bool isMoving = false;

    void draw(float color1[], float color2[], float color3[]);

    // ������ �����������, ����� ���� �� ������� ��������
    float fireCooldown = 0.0f;
    const float fireRate = 0.5f; // 1 ������� ������ 0.5 ��� (�������)
};