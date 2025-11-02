#pragma once

#include "Tank.h"

class PlayerTank;

/*
 Наследуется от Tank и реализует логику ИИ.
*/
class EnemyTank : public Tank {
public:
    EnemyTank(float startX, float startY, int startDir);

    void update(float deltaTime, const Map& map, float playerX = 0, float playerY = 0) override;

    void draw() override;

    bool isBlocked() const { return m_isBlockedByWall; }

private:
    // Таймер для принятия решений ИИ
    float aiTimer = 0.0f;

    // Таймер для выстрелов
    float fireTimer = 0.0f;

    bool m_isBlockedByWall = false;

    bool hasLineOfSight(const Map& map, float playerX, float playerY);
};