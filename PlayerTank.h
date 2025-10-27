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
    void update(float deltaTime, const Map& map) override;

    // ����� fire() ����� ������ �� handleInput()
};