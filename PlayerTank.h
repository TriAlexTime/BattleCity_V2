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
    void handleInput(int key);

    /*
     @brief Обновляет логику игрока.
    */
    void update(float deltaTime) override;

    // Метод fire() будет вызван из handleInput()
};