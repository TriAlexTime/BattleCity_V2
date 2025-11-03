#pragma once

#include "Tank.h"

/*
 Наследуется от Tank и добавляет обработку ввода.
*/
class PlayerTank : public Tank {
public:
    PlayerTank(float startX, float startY, int startDir);

    /*
     @brief Обрабатывает нажатия клавиш от игрока.
     @param key Код нажатой клавиши.
    */
    void handleInput(int key, bool isPressed);

    /*
     @brief Обновляет логику игрока.
    */
    void update(float deltaTime, const Map& map, float playerX = 0, float playerY = 0) override;

    // Метод fire() будет вызван из handleInput()

    void draw() override;
};