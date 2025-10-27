#pragma once

#include "Tank.h"

/*
 ����������� �� Tank � ��������� ������ ��.
*/
class EnemyTank : public Tank {
public:
    EnemyTank(float startX, float startY, int startDir);

    void update(float deltaTime, const Map& map) override;

private:
    // ������ ��� �������� ������� ��
    float aiTimer = 0.0f;

    // ������ ��� ���������
    float fireTimer = 0.0f;
};