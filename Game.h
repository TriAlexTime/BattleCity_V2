#pragma once

#include "Map.h"

#include <vector>
#include <memory>   // Для std::unique_ptr
#include <string>

// Прямые объявления, чтобы не включать .h файлы
class PlayerTank;
class EnemyTank;
class Projectile;

class Game {
public:
    Game();
    ~Game();

    void run();

    /*
     Обрабатывает события ввода от ОС (GLUT).
     @param key Код клавиши.
     @param isPressed true - нажата, false - отпущена.
    */
    void handleInput(int key, bool isPressed);

private:
    /*
     Инициализирует игру (загружает карту, создает игрока).
    */
    void init();

    /*
     Главный метод обновления, вызывается каждый кадр.
     @param deltaTime Время с прошлого кадра.
    */
    void update(float deltaTime);

    void render();

    void checkCollisions();

    void spawnEnemyTank();

    void cleanupObjects();


    Map m_map;


    std::unique_ptr<PlayerTank> m_player;

    // Векторы для хранения всех динамических объектов
    std::vector<std::unique_ptr<EnemyTank>> m_enemies;
    std::vector<std::unique_ptr<Projectile>> m_projectiles;

    int m_lives; // 'hp' игрока
    int m_enemiesLeftToSpawn; // 'last_tanks'
    float m_enemySpawnTimer = 0.0f; // Таймер для спавна
    bool m_isRunning = true;
};