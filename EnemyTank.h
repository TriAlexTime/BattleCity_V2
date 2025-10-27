#pragma once

#include "Tank.h"

/*
 Наследуется от Tank и реализует логику ИИ.
*/
class EnemyTank : public Tank {
public:
    EnemyTank(float startX, float startY, int startDir);

    void update(float deltaTime, const Map& map) override;

private:
    // Таймер для принятия решений ИИ
    float aiTimer = 0.0f;

    // Таймер для выстрелов
    float fireTimer = 0.0f;
};