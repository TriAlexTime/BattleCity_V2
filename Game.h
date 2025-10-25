#pragma once

#include "Map.h"

#include <vector>
#include <memory>   // ��� std::unique_ptr
#include <string>

// ������ ����������, ����� �� �������� .h �����
class PlayerTank;
class EnemyTank;
class Projectile;

class Game {
public:
    Game();
    ~Game();

    void run();

    /*
     ������������ ������� ����� �� �� (GLUT).
     @param key ��� �������.
     @param isPressed true - ������, false - ��������.
    */
    void handleInput(int key, bool isPressed);

private:
    /*
     �������������� ���� (��������� �����, ������� ������).
    */
    void init();

    /*
     ������� ����� ����������, ���������� ������ ����.
     @param deltaTime ����� � �������� �����.
    */
    void update(float deltaTime);

    void render();

    void checkCollisions();

    void spawnEnemyTank();

    void cleanupObjects();


    Map m_map;


    std::unique_ptr<PlayerTank> m_player;

    // ������� ��� �������� ���� ������������ ��������
    std::vector<std::unique_ptr<EnemyTank>> m_enemies;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;

    int m_lives; // 'hp' ������
    int m_enemiesLeftToSpawn; // 'last_tanks'
    float m_enemySpawnTimer = 0.0f; // ������ ��� ������
    bool m_isRunning = true;
};