#include "Map.h"
#include "Graphics.h"
#include <GL/glut.h>
#include <fstream>   // ��� ������ � ������� (ifstream)
#include <iostream>  // ��� ������ ������
#include <cstdlib>   // ��� rand() � srand()
#include <vector>

Map::Map() {
    // �������������� ����� grid ������� �������.
    grid.resize(MAP_HEIGHT, std::vector<Cell>(MAP_WIDTH));
}

void Map::loadFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open map file " << filename << std::endl;
        return;
    }

    // �������� ��������� ������ ������
    inputFile >> playerSpawnX >> playerSpawnY;

    // �������� ��������� ������ ������
    for (int i = 0; i < 3; ++i) {
        inputFile >> enemySpawns[i][0] >> enemySpawns[i][1];
    }

    // �������� ����� �����
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            int materialType;
            inputFile >> materialType;
            grid[i][j].material = static_cast<Material>(materialType);

            // ������������� "������" (parts) ������ ������ ������
            for (int k = 0; k < PARTS_PER_CELL; ++k) {
                for (int l = 0; l < PARTS_PER_CELL; ++l) {
                    Part& currentPart = grid[i][j].parts[k][l];
                    currentPart.exist = true;

                    switch (grid[i][j].material) {
                    case Material::BRICK:
                    case Material::HQ:
                        currentPart.health = 1;
                        currentPart.collision = 2; // �����
                        break;
                    case Material::CONCRETE:
                        currentPart.health = 2; // ����� �������
                        currentPart.collision = 2; // �����
                        break;
                    case Material::WATER:
                        currentPart.health = 0;
                        currentPart.collision = 1; // ����
                        break;
                    default: // NULL_MAT, LEAFS, ICE
                        currentPart.health = 0;
                        currentPart.collision = 0; // ���������
                        break;
                    }
                }
            }
        }
    }
}

void Map::draw(bool isOverlay) {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            bool shouldDraw = isOverlay ? (grid[i][j].material == Material::LEAFS)
                : (grid[i][j].material != Material::LEAFS);

            if (shouldDraw) {
                float worldX = j * CELL_SIZE;
                float worldY = i * CELL_SIZE;
                switch (grid[i][j].material) {
                case Material::BRICK: glColor3f(0.7f, 0.25f, 0.0f); break;
                case Material::CONCRETE: glColor3f(0.5f, 0.5f, 0.5f); break;
                case Material::WATER: glColor3f(0.2f, 0.2f, 0.8f); break;
                case Material::LEAFS: glColor3f(0.0f, 0.6f, 0.0f); break;
                case Material::HQ: glColor3f(0.9f, 0.9f, 0.9f); break;
                default: glColor3f(0.1f, 0.1f, 0.1f); break;
                }
                drawRectangle(worldX, worldY, worldX + CELL_SIZE, worldY + CELL_SIZE);
            }
        }
    }
}

bool Map::checkTankCollision(float x, float y, int dir) const {
    // ������ ����� 40x40, ������ ��� ���� ��������� �� ���������� 20 �� ������ (x, y)
    float tankEdgeOffset = 20.0f;

    // 1. �������� ������������ � ��������� �����
    if ((dir == 1 && y - tankEdgeOffset <= 0) || // UP
        (dir == 2 && y + tankEdgeOffset >= MAP_HEIGHT * CELL_SIZE) || // DOWN
        (dir == 3 && x - tankEdgeOffset <= 0) || // LEFT
        (dir == 4 && x + tankEdgeOffset >= MAP_WIDTH * CELL_SIZE)) { // RIGHT
        return false;
    }

    // 2. �������� ������������ �� ������� � �����
    // �� ��������� ����� "������" (parts) ����� ����� ������
    int startPart, endPart;
    int fixedPart;

    switch (dir) {
    case 1: // UP
        fixedPart = static_cast<int>((y - tankEdgeOffset - 1) / PART_SIZE); // Y-���������� ����� ��������
        startPart = static_cast<int>((x - tankEdgeOffset) / PART_SIZE);
        endPart = static_cast<int>((x + tankEdgeOffset - 1) / PART_SIZE);
        for (int i = startPart; i <= endPart; ++i) {
            if (grid[fixedPart / PARTS_PER_CELL][i / PARTS_PER_CELL].parts[fixedPart % PARTS_PER_CELL][i % PARTS_PER_CELL].collision > 0) return false;
        }
        break;
    case 2: // DOWN
        fixedPart = static_cast<int>((y + tankEdgeOffset) / PART_SIZE);
        startPart = static_cast<int>((x - tankEdgeOffset) / PART_SIZE);
        endPart = static_cast<int>((x + tankEdgeOffset - 1) / PART_SIZE);
        for (int i = startPart; i <= endPart; ++i) {
            if (grid[fixedPart / PARTS_PER_CELL][i / PARTS_PER_CELL].parts[fixedPart % PARTS_PER_CELL][i % PARTS_PER_CELL].collision > 0) return false;
        }
        break;
    case 3: // LEFT
        fixedPart = static_cast<int>((x - tankEdgeOffset - 1) / PART_SIZE); // X-���������� ����� ��������
        startPart = static_cast<int>((y - tankEdgeOffset) / PART_SIZE);
        endPart = static_cast<int>((y + tankEdgeOffset - 1) / PART_SIZE);
        for (int i = startPart; i <= endPart; ++i) {
            if (grid[i / PARTS_PER_CELL][fixedPart / PARTS_PER_CELL].parts[i % PARTS_PER_CELL][fixedPart % PARTS_PER_CELL].collision > 0) return false;
        }
        break;
    case 4: // RIGHT
        fixedPart = static_cast<int>((x + tankEdgeOffset) / PART_SIZE);
        startPart = static_cast<int>((y - tankEdgeOffset) / PART_SIZE);
        endPart = static_cast<int>((y + tankEdgeOffset - 1) / PART_SIZE);
        for (int i = startPart; i <= endPart; ++i) {
            if (grid[i / PARTS_PER_CELL][fixedPart / PARTS_PER_CELL].parts[i % PARTS_PER_CELL][fixedPart % PARTS_PER_CELL].collision > 0) return false;
        }
        break;
    }

    // ���� �� � ��� �� �����������
    return true;
}

ProjectileHitResult Map::checkProjectileHit(float& projX, float& projY, int dir, int damage) {
    int partX = static_cast<int>(projX / PART_SIZE);
    int partY = static_cast<int>(projY / PART_SIZE);

    int cellX = partX / PARTS_PER_CELL;
    int cellY = partY / PARTS_PER_CELL;

    // �������� ������ �� ������� �����
    if (cellX < 0 || cellX >= MAP_WIDTH || cellY < 0 || cellY >= MAP_HEIGHT) {
        return ProjectileHitResult::NONE;
    }

    Part& hitPart = grid[cellY][cellX].parts[partY % PARTS_PER_CELL][partX % PARTS_PER_CELL];
    Material hitMaterial = grid[cellY][cellX].material;

    if (hitPart.collision == 2) { // ������������ �� ������ (������/�����/����)
        // ���� ������ � ����, ���� ��������
        if (hitMaterial == Material::HQ) {
            hitPart.exist = false;
            hitPart.collision = 0;
            return ProjectileHitResult::HQ_HIT;
        }

        // ������� ���� �����
        if (hitPart.health > 0) {
            hitPart.health -= damage;
            if (hitPart.health <= 0) {
                hitPart.exist = false;
                hitPart.collision = 0;
            }
        }
        return ProjectileHitResult::WALL; // ������ � ������� �����
    }

    return ProjectileHitResult::NONE; // ��� ������������
}

void Map::spawnBonus() {
    // ���� ��������� ������, ��� � C-����
    if (rand() % 1536 == 0) {
        int cellX, cellY;
        // ���� ��������� ������ ������, ��� ��� ��� ������
        do {
            cellX = rand() % MAP_WIDTH;
            cellY = rand() % MAP_HEIGHT;
        } while ((grid[cellY][cellX].material != Material::NULL_MAT &&
            grid[cellY][cellX].material != Material::LEAFS &&
            grid[cellY][cellX].material != Material::ICE) ||
            grid[cellY][cellX].bonus != BonusType::NONE);

        // �������� ��� ������
        grid[cellY][cellX].bonus = (rand() % 2 == 0) ? BonusType::HEALTH_UP : BonusType::UPGRADE;
    }
}

BonusType Map::checkBonusPickup(float x, float y) {
    int cellX = static_cast<int>(x / CELL_SIZE);
    int cellY = static_cast<int>(y / CELL_SIZE);

    // ���������, ���� �� ����� � ������, ��� ��������� ����
    if (grid[cellY][cellX].bonus != BonusType::NONE) {
        BonusType pickedBonus = grid[cellY][cellX].bonus;
        grid[cellY][cellX].bonus = BonusType::NONE; // ������� ����� � �����
        return pickedBonus; // ���������� ��� ����������� ������
    }

    return BonusType::NONE; // ����� �� ��������
}

void Map::getPlayerSpawn(float& x, float& y) const {
    x = playerSpawnX;
    y = playerSpawnY;
}

void Map::getEnemySpawn(int index, float& x, float& y) const {
    if (index >= 0 && index < 3) {
        x = enemySpawns[index][0];
        y = enemySpawns[index][1];
    }
}