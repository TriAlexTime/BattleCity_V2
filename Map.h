#pragma once

#include <vector>
#include <string>

// ���� ������� int
enum class Material {
    NULL_MAT, // null - ����������������� �����
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
    NONE,      // ������ �� ������
    WALL,      // ��������� � ������� �����
    HQ_HIT     // ��������� � ����
};

// ��������� ��� ������ ������� parts
struct Part {
    bool exist = true;
    int health = 0;   // 0 ��� ����������, 1 ��� �������, 2 ��� ������
    int collision = 0; // 0: ���, 1: ����, 2: �����
};

// ��������� ��� ������ ������ ������� map
struct Cell {
    Material material = Material::NULL_MAT;
    BonusType bonus = BonusType::NONE;

    // ������ ������ (Cell) �������� ����� 6x6 ������
    // ���� map[i][j] � parts[k][l]
    std::vector<std::vector<Part>> parts;

    // ����������� �� ���������, �������������� ����� 6x6
    Cell() : parts(6, std::vector<Part>(6)) {}
};


/*
 ��������� ���������, ���������� � ������� ������������
*/

class Map {
public:
    Map();

    /*
     ��������� ����� �� ����� (������ �� initMap).
     @param filename ��� ����� �����
    */
    void loadFromFile(const std::string& filename);

    /*
     ������������ ��� ������ �����.
    */
    void draw(bool isOverlay);

    // --- ������ ��� �������� ������������ ---

    /*
     ���������, ����� �� ���� ��������� � �������� �����������.
     @return true, ���� ���� ��������.
    */
    bool checkTankCollision(float x, float y, int dir) const;

    /*
     ���������, ����� �� ������ � ����� � ���� �� - ������� ���� �����.
     
     @param projX ���������� X ������� (���������� �� ������).
     @param projY ���������� Y ������� (���������� �� ������).
     @return true, ���� ������ ���������� � ��� ���� ����������.
    */

    ProjectileHitResult checkProjectileHit(float& projX, float& projY, int dir, int damage);

    // --- ������ ������� ---

    void spawnBonus(); // ������ ������ ������ �� `timer()`

    /**
     ���������, �������� �� ���� �����.
     @return ��� ������������ ������ (��� NONE).
    */
    BonusType checkBonusPickup(float x, float y);

    // --- ������� ��� ������ ---
    void getPlayerSpawn(float& x, float& y) const;
    void getEnemySpawn(int index, float& x, float& y) const;

private:
    // ���������, ������� ������ ���� #define
    static const int MAP_WIDTH = 13;
    static const int MAP_HEIGHT = 13;
    static const int CELL_SIZE = 48;
    static const int PARTS_PER_CELL = 6;
    static const int PART_SIZE = 8; // CELL_SIZE / PARTS_PER_CELL

    // ��������� ������, �������� ��� ������ �����.
    // ��� ������ cell map[MAP_HEIGHT][MAP_WIDTH]
    std::vector<std::vector<Cell>> grid;

    // ���������� ������
    float playerSpawnX = 0.0f;
    float playerSpawnY = 0.0f;
    float enemySpawns[3][2] = { {0,0}, {0,0}, {0,0} };
};