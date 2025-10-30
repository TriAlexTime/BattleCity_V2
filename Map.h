#pragma once

#include <vector>
#include <string>

// Были простые int
enum class Material {
    NULL_MAT, // null - зарезервированное слово
    BRICK,
    CONCRETE,
    WATER,
    LEAFS,
    ICE,
    HQ
};

enum class BonusType {
    NONE,
    HEALTH_UP,
    UPGRADE
};

enum class ProjectileHitResult {
    NONE,      // Ничего не задето
    WALL,      // Попадание в обычную стену
    HQ_HIT     // Попадание в штаб
};

// Структура для замена массива parts
struct Part {
    bool exist = true;
    int health = 0;   // 0 для проходимых, 1 для кирпича, 2 для бетона
    int collision = 0; // 0: нет, 1: вода, 2: стена
};

// Структура для замены ячейки массива map
struct Cell {
    Material material = Material::NULL_MAT;
    BonusType bonus = BonusType::NONE;

    // Каждая ячейка (Cell) содержит сетку 6x6 частей
    // Было map[i][j] и parts[k][l]
    std::vector<std::vector<Part>> parts;

    // Конструктор по умолчанию, инициализирует сетку 6x6
    Cell() : parts(6, std::vector<Part>(6)) {}
};


/*
 Управляет загрузкой, отрисовкой и логикой столкновений
*/

class Map {
public:
    Map();

    /*
     Загружает карту из файла (логика из initMap).
     @param filename Имя файла карты
    */
    void loadFromFile(const std::string& filename);

    /*
     Отрисовывает все ячейки карты.
    */
    void draw(bool isOverlay);

    // --- Методы для проверки столкновений ---

    /*
     Проверяет, может ли танк двигаться в заданном направлении.
     @return true, если путь свободен.
    */
    bool checkTankCollision(float x, float y, int dir) const;

    /*
     Проверяет, попал ли снаряд в стену и если да - наносит урон стене.
     
     @param projX Координата X снаряда (передается по ссылке).
     @param projY Координата Y снаряда (передается по ссылке).
     @return true, если снаряд столкнулся и его надо уничтожить.
    */

    ProjectileHitResult checkProjectileHit(float& projX, float& projY, int dir, int damage);

    // --- Логика бонусов ---

    void spawnBonus(); // Логика спавна бонуса из `timer()`

    /**
     Проверяет, подобрал ли танк бонус.
     @return Тип подобранного бонуса (или NONE).
    */
    BonusType checkBonusPickup(float x, float y);

    // --- Геттеры для спавна ---
    void getPlayerSpawn(float& x, float& y) const;
    void getEnemySpawn(int index, float& x, float& y) const;

private:
    // Константы, которые раньше были #define
    static const int MAP_WIDTH = 13;
    static const int MAP_HEIGHT = 13;
    static const int CELL_SIZE = 48;
    static const int PARTS_PER_CELL = 6;
    static const int PART_SIZE = 8; // CELL_SIZE / PARTS_PER_CELL

    // Двумерный вектор, хранящий все ячейки карты.
    // Это замена cell map[MAP_HEIGHT][MAP_WIDTH]
    std::vector<std::vector<Cell>> grid;

    // Координаты спавна
    float playerSpawnX = 0.0f;
    float playerSpawnY = 0.0f;
    float enemySpawns[3][2] = { {0,0}, {0,0}, {0,0} };
};