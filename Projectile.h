#pragma once

#include "GameObject.h"

// Enum ��� �����������, ��� "��������" �������
enum class Owner {
    PLAYER,
    ENEMY
};

/*
 ������, ���������� ������.
 ��������, ���� �� ���������� ��� �� ������� �� �������.
*/

class Projectile : public GameObject {
public:
    Projectile(float startX, float startY, int dir, int dmg, Owner owner);

    void update(float deltaTime, const Map& map) override;
    void draw() override;
    bool isAlive() const override;


    void destroy();

    Owner getOwner() const { return m_owner; }
    int getDamage() const { return m_damage; }

private:
    int m_damage;
    Owner m_owner;
    float m_speed;
    bool m_isAlive = true;
};