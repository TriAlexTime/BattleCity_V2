#pragma once

#include "Tank.h"

/*
 ����������� �� Tank � ��������� ��������� �����.
*/
class PlayerTank : public Tank {
public:
    PlayerTank(float startX, float startY, int startDir);

    /*
     @brief ������������ ������� ������ �� ������.
     @param key ��� ������� �������.
    */
    void handleInput(int key);

    /*
     @brief ��������� ������ ������.
    */
    void update(float deltaTime) override;

    // ����� fire() ����� ������ �� handleInput()
};