#include "Map.h"
#include "Graphics.h"
#include <GL/glut.h>
#include <fstream>   // Для работы с файлами (ifstream)
#include <iostream>  // Для вывода ошибок
#include <cstdlib>   // Для rand() и srand()
#include <vector>

Map::Map() {
    // Инициализируем сетку grid нужного размера.
    grid.resize(MAP_HEIGHT, std::vector<Cell>(MAP_WIDTH));
}

void Map::loadFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open map file " << filename << std::endl;
        return;
    }

    // Загрузка координат спавна игрока
    inputFile >> playerSpawnX >> playerSpawnY;

    // Загрузка координат спавна врагов
    for (int i = 0; i < 3; ++i) {
        inputFile >> enemySpawns[i][0] >> enemySpawns[i][1];
    }

    // Загрузка сетки карты
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            int materialType;
            inputFile >> materialType;
            grid[i][j].material = static_cast<Material>(materialType);

            // Инициализация "частей" (parts) внутри каждой ячейки
            for (int k = 0; k < PARTS_PER_CELL; ++k) {
                for (int l = 0; l < PARTS_PER_CELL; ++l) {
                    Part& currentPart = grid[i][j].parts[k][l];
                    currentPart.exist = true;

                    switch (grid[i][j].material) {
                    case Material::BRICK:
                    case Material::HQ:
                        currentPart.health = 1;
                        currentPart.collision = 2; // Стена
                        break;
                    case Material::CONCRETE:
                        currentPart.health = 2; // Бетон прочнее
                        currentPart.collision = 2; // Стена
                        break;
                    case Material::WATER:
                        currentPart.health = 0;
                        currentPart.collision = 1; // Вода
                        break;
                    default: // NULL_MAT, LEAFS, ICE
                        currentPart.health = 0;
                        currentPart.collision = 0; // Проходимо
                        break;
                    }
                }
            }
        }
    }
}


Part& Map::getPart(int globalPartY, int globalPartX) {
    // Конвертируем глобальные (0-77) в локальные (0-12) и (0-5)
    int cellY = globalPartY / PARTS_PER_CELL;
    int cellX = globalPartX / PARTS_PER_CELL;
    int localPartY = globalPartY % PARTS_PER_CELL;
    int localPartX = globalPartX % PARTS_PER_CELL;
    return grid[cellY][cellX].parts[localPartY][localPartX];
}

const Part& Map::getPart(int globalPartY, int globalPartX) const {
    // Конвертируем глобальные (0-77) в локальные (0-12) и (0-5)
    int cellY = globalPartY / PARTS_PER_CELL;
    int cellX = globalPartX / PARTS_PER_CELL;
    int localPartY = globalPartY % PARTS_PER_CELL;
    int localPartX = globalPartX % PARTS_PER_CELL;
    return grid[cellY][cellX].parts[localPartY][localPartX];
}


void Map::draw(bool isOverlay) {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {
            bool shouldDraw = isOverlay ? (grid[i][j].material == Material::LEAFS)
                : (grid[i][j].material != Material::LEAFS);

            if (shouldDraw) {
                float worldX = j * CELL_SIZE;
                float worldY = i * CELL_SIZE;
                int destroyedCount = 0;

                switch (grid[i][j].material) {
                case Material::BRICK:
                    glColor3f(1.0f, 0.4f, 0.0f);
                    drawRectangle(worldX, worldY, worldX + CELL_SIZE, worldY + CELL_SIZE);
                    
                    glColor3f(0.4, 0.4, 0.4);
                    drawRectangle(worldX, worldY + CELL_SIZE, worldX + CELL_SIZE, worldY + CELL_SIZE - 4);
                    drawRectangle(worldX, worldY + CELL_SIZE - 12, worldX + CELL_SIZE, worldY + CELL_SIZE - 16);
                    drawRectangle(worldX, worldY + CELL_SIZE - 24, worldX + CELL_SIZE, worldY + CELL_SIZE - 28);
                    drawRectangle(worldX, worldY + CELL_SIZE - 36, worldX + CELL_SIZE, worldY + CELL_SIZE - 40);

                    drawRectangle(worldX + 12, worldY, worldX + 16, worldY + 12);
                    drawRectangle(worldX + CELL_SIZE - 12, worldY, worldX + CELL_SIZE - 8, worldY + 12);
                    drawRectangle(worldX + 12, worldY + 24, worldX + 16, worldY + 36);
                    drawRectangle(worldX + CELL_SIZE - 12, worldY + 24, worldX + CELL_SIZE - 8, worldY + 36);

                    drawRectangle(worldX, worldY + 12, worldX + 4, worldY + 24);
                    drawRectangle(worldX + CELL_SIZE - 24, worldY + 12, worldX + CELL_SIZE - 20, worldY + 24);
                    drawRectangle(worldX, worldY + 36, worldX + 4, worldY + 48);
                    drawRectangle(worldX + CELL_SIZE - 24, worldY + 36, worldX + CELL_SIZE - 20, worldY + 48);

                    destroyedCount = drawDestroyedParts(worldX, worldY, grid[i][j]);
                    break;

                case Material::CONCRETE:
                    for (int k = 0; k < 4; k++) {
                        glColor3f(1.0f, 1.0f, 1.0f);
                        drawRectangle(worldX + 24 * (k % 2), worldY + 24 * (k / 2), worldX + 24 + 24 * (k % 2), worldY + 24 + 24 * (k / 2));
                        glColor3f(0.85f, 0.85f, 0.85f);
                        drawRectangle(worldX + 24 * (k % 2), worldY + 24 * (k / 2), worldX + 24 + 24 * (k % 2), worldY + 6 + 24 * (k / 2));
                        drawRectangle(worldX + 24 * (k % 2), worldY + 24 * (k / 2), worldX + 6 + 24 * (k % 2), worldY + 24 + 24 * (k / 2));
                        glColor3f(0.5f, 0.5f, 0.5f);
                        drawRectangle(worldX + 24 + 24 * (k % 2), worldY + 6 + 24 * (k / 2), worldX + 18 + 24 * (k % 2), worldY + 24 + 24 * (k / 2));
                        drawRectangle(worldX + 24 + 24 * (k % 2), worldY + 24 + 24 * (k / 2), worldX + 6 + 24 * (k % 2), worldY + 18 + 24 * (k / 2));
                    }
                    destroyedCount = drawDestroyedParts(worldX, worldY, grid[i][j]);
                    break;

                case Material::LEAFS:
                    for (int k = 0; k < 4; k++) {
                        glColor3f(0.6f, 1.0f, 0.1f);
                        drawRectangle(worldX + 3 + 24 * (k % 2), worldY + 24 * (k / 2), worldX + 21 + 24 * (k % 2), worldY + 24 + 24 * (k / 2));
                        drawRectangle(worldX + 24 * (k % 2), worldY + 3 + 24 * (k / 2), worldX + 24 + 24 * (k % 2), worldY + 21 + 24 * (k / 2));
                        
                        glColor3f(0.0f, 0.4f, 0.0f);
                        drawRectangle(worldX + 3 + 24 * (k % 2), worldY + 24 * (k / 2), worldX + 15 + 24 * (k % 2), worldY + 6 + 24 * (k / 2));
                        drawRectangle(worldX + 24 * (k % 2), worldY + 3 + 24 * (k / 2), worldX + 6 + 24 * (k % 2), worldY + 12 + 24 * (k / 2));
                        drawRectangle(worldX + 24 + 24 * (k % 2), worldY + 3 + 24 * (k / 2), worldX + 21 + 24 * (k % 2), worldY + 6 + 24 * (k / 2));
                        drawRectangle(worldX + 15 + 24 * (k % 2), worldY + 12 + 24 * (k / 2), worldX + 21 + 24 * (k % 2), worldY + 15 + 24 * (k / 2));
                        drawRectangle(worldX + 6 + 24 * (k % 2), worldY + 15 + 24 * (k / 2), worldX + 9 + 24 * (k % 2), worldY + 18 + 24 * (k / 2));
                        drawRectangle(worldX + 9 + 24 * (k % 2), worldY + 21 + 24 * (k / 2), worldX + 18 + 24 * (k % 2), worldY + 24 + 24 * (k / 2));
                    }
                    break;

                case Material::WATER:
                    glColor3f(0.2f, 0.2f, 0.8f);
                    drawRectangle(worldX, worldY, worldX + CELL_SIZE, worldY + CELL_SIZE);
                    glColor3f(0.85f, 0.85f, 0.85f);
                    drawRectangle(worldX + 4, worldY + 8, worldX + 26, worldY + 10);
                    drawRectangle(worldX + 16, worldY + 18, worldX + 42, worldY + 20);
                    drawRectangle(worldX + 4, worldY + 28, worldX + 26, worldY + 30);
                    drawRectangle(worldX + 16, worldY + 38, worldX + 42, worldY + 40);
                    break;

                case Material::ICE:
                    glColor3f(0.75f, 0.75f, 0.75f);
                    drawRectangle(worldX, worldY, worldX + CELL_SIZE, worldY + CELL_SIZE);
                    glColor3f(0.5f, 0.5f, 0.5f);
                    drawRectangle(worldX, worldY + 44, worldX + CELL_SIZE, worldY + 48);
                    drawRectangle(worldX, worldY + 36, worldX + CELL_SIZE, worldY + 40);
                    drawRectangle(worldX, worldY + 28, worldX + CELL_SIZE, worldY + 32);
                    drawRectangle(worldX, worldY + 20, worldX + CELL_SIZE, worldY + 24);
                    drawRectangle(worldX, worldY + 12, worldX + CELL_SIZE, worldY + 16);
                    drawRectangle(worldX, worldY + 4, worldX + CELL_SIZE, worldY + 8);
                    break;

                case Material::HQ:
                    glColor3f(0.3f, 0.3f, 0.3f);
                    drawRectangle(worldX + 8, worldY + 8, worldX + CELL_SIZE - 8, worldY + CELL_SIZE - 8);
                    glColor3f(0.9f, 0.9f, 0.9f);
                    drawText(worldX + 10, worldY + 31, GLUT_BITMAP_HELVETICA_18, "HQ");

                    destroyedCount = drawDestroyedParts(worldX, worldY, grid[i][j]);
                    break;

                default: // NULL_MAT
                    glColor3f(0.1f, 0.1f, 0.1f);
                    drawRectangle(worldX, worldY, worldX + CELL_SIZE, worldY + CELL_SIZE);
                    break;
                }

                if (destroyedCount == (PARTS_PER_CELL * PARTS_PER_CELL)) {
                    grid[i][j].material = Material::NULL_MAT;
                }
            }
        }
    }
}


bool Map::checkTankCollision(float x, float y, int dir) const {
    // Получаем "part" координату центра танка (0-77)
    int xmap = static_cast<int>(x / PART_SIZE);
    int ymap = static_cast<int>(y / PART_SIZE);
    bool result = true; // true = нет коллизии

    // 1. Проверяем границы карты (с буфером в 4 "части")
    if (dir == 1 && ymap - 3 <= 0) return false;
    if (dir == 2 && ymap + 2 >= MAP_HEIGHT * PARTS_PER_CELL - 1) return false;
    if (dir == 3 && xmap - 3 <= 0) return false;
    if (dir == 4 && xmap + 2 >= MAP_WIDTH * PARTS_PER_CELL - 1) return false;

    // 2. Проверяем коллизии со стенами (полоска из 6 "частей")
    // --- ИСПРАВИТЬ ЗДЕСЬ ---
    // Мы меняем 6-частную проверку (xmap - 3 ... xmap + 2) 
    // на 5-частную (xmap - 2 ... xmap + 2)

    switch (dir) {
    case 1: // UP
        // БЫЛО: for (int i = xmap - 3; i <= xmap + 2; i++) {
        for (int i = xmap - 2; i <= xmap + 2; i++) { // СТАЛО
            if (getPart(ymap - 3, i).collision > 0) result = false; // ymap-3 - это верно
        }
        break;
    case 2: // DOWN
        // БЫЛО: for (int i = xmap - 3; i <= xmap + 2; i++) {
        for (int i = xmap - 2; i <= xmap + 2; i++) { // СТАЛО
            if (getPart(ymap + 3, i).collision > 0) result = false; // ymap+3 - это верно
        }
        break;
    case 3: // LEFT
        // БЫЛО: for (int i = ymap - 3; i <= ymap + 2; i++) {
        for (int i = ymap - 2; i <= ymap + 2; i++) { // СТАЛО
            if (getPart(i, xmap - 3).collision > 0) result = false; // xmap-3 - это верно
        }
        break;
    case 4: // RIGHT
        // БЫЛО: for (int i = ymap - 3; i <= ymap + 2; i++) {
        for (int i = ymap - 2; i <= ymap + 2; i++) { // СТАЛО
            if (getPart(i, xmap + 3).collision > 0) result = false; // xmap+3 - это верно
        }
        break;
    }

    return result;
}


ProjectileHitResult Map::checkProjectileHit(float& projX, float& projY, int dir, int damage) {
    
    // Проверяет КРАЯ снаряда (центр +/– 2 пикселя)
    const float MAP_MAX_X = MAP_WIDTH * CELL_SIZE;
    const float MAP_MAX_Y = MAP_HEIGHT * CELL_SIZE;

    if (projX - 2 < 0.0f || projX + 2 >= MAP_MAX_X ||
        projY - 2 < 0.0f || projY + 2 >= MAP_MAX_Y) {
        return ProjectileHitResult::NONE;
    }

    // 1. Получаем глобальные "part" координаты краев снаряда (0-77)
    int _x1 = static_cast<int>((projX - 2) / PART_SIZE);
    int _y1 = static_cast<int>((projY - 2) / PART_SIZE);
    int _x2 = static_cast<int>((projX + 2) / PART_SIZE);
    int _y2 = static_cast<int>((projY + 2) / PART_SIZE);

    if (dir == 1 || dir == 2) { // UP или DOWN
        int _y = (dir == 1) ? _y1 : _y2; // Y-координата столкновения
        int yf = (dir == 1) ? 1 : -1;  // Смещение для проверки бетона

        int _x = -1; // X-координата первой "части", в которую попали
        if (getPart(_y, _x1).exist && getPart(_y, _x1).health != 0) _x = _x1;
        else if (getPart(_y, _x2).exist && getPart(_y, _x2).health != 0) _x = _x2;

        if (_x != -1) { // Если попали
            int _xf1 = (_x == _x1) ? 2 : 3;
            int _xf2 = (_x == _x1) ? 3 : 2;

            if (grid[_y / PARTS_PER_CELL][_x / PARTS_PER_CELL].material == Material::HQ) {
                getPart(_y, _x).exist = false;
                getPart(_y, _x).health = 0;
                getPart(_y, _x).collision = 0;
                return ProjectileHitResult::HQ_HIT;
            }

            // Ломаем полоску влево
            for (int j = _x; j >= _x - _xf1; j--) {
                if (j < 0) break;
                Part& p = getPart(_y, j);
                if (p.exist && p.health <= damage) {
                    if (p.health == 2 && getPart(_y + yf, j).health != 0) break;
                    p.exist = false; p.health = 0; p.collision = 0;
                }
                else break;
            }
            // Ломаем полоску вправо
            for (int j = _x + 1; j <= _x + _xf2; j++) {
                if (j >= MAP_WIDTH * PARTS_PER_CELL) break;
                Part& p = getPart(_y, j);
                if (p.exist && p.health <= damage) {
                    if (p.health == 2 && getPart(_y + yf, j).health != 0) break;
                    p.exist = false; p.health = 0; p.collision = 0;
                }
                else break;
            }
            return ProjectileHitResult::WALL;
        }
    }
    else if (dir == 3 || dir == 4) { // LEFT или RIGHT
        int _x = (dir == 3) ? _x1 : _x2; // X-координата столкновения
        int xf = (dir == 3) ? 1 : -1;  // Смещение для проверки

        int _y = -1; // Y-координата первой "части"
        if (getPart(_y1, _x).exist && getPart(_y1, _x).health != 0) _y = _y1;
        else if (getPart(_y2, _x).exist && getPart(_y2, _x).health != 0) _y = _y2;

        if (_y != -1) { // Если попали
            int _yf1 = (_y == _y1) ? 2 : 3;
            int _yf2 = (_y == _y1) ? 3 : 2;

            if (grid[_y / PARTS_PER_CELL][_x / PARTS_PER_CELL].material == Material::HQ) {
                getPart(_y, _x).exist = false;
                getPart(_y, _x).health = 0;
                getPart(_y, _x).collision = 0;
                return ProjectileHitResult::HQ_HIT;
            }

            // Ломаем полоску вверх
            for (int j = _y; j >= _y - _yf1; j--) {
                if (j < 0) break;
                Part& p = getPart(j, _x);
                if (p.exist && p.health <= damage) {
                    if (p.health == 2 && getPart(j, _x + xf).health != 0) break;
                    p.exist = false; p.health = 0; p.collision = 0;
                }
                else break;
            }
            // Ломаем полоску вниз
            for (int j = _y + 1; j <= _y + _yf2; j++) {
                if (j >= MAP_HEIGHT * PARTS_PER_CELL) break;
                Part& p = getPart(j, _x);
                if (p.exist && p.health <= damage) {
                    if (p.health == 2 && getPart(j, _x + xf).health != 0) break;
                    p.exist = false; p.health = 0; p.collision = 0;
                }
                else break;
            }
            return ProjectileHitResult::WALL;
        }
    }

    return ProjectileHitResult::NONE;
}

void Map::spawnBonus() {
    // Шанс появления бонуса, как в C-коде
    if (rand() % 1536 == 0) {
        int cellX, cellY;
        // Ищем случайную пустую ячейку, где еще нет бонуса
        do {
            cellX = rand() % MAP_WIDTH;
            cellY = rand() % MAP_HEIGHT;
        } while ((grid[cellY][cellX].material != Material::NULL_MAT &&
            grid[cellY][cellX].material != Material::LEAFS &&
            grid[cellY][cellX].material != Material::ICE) ||
            grid[cellY][cellX].bonus != BonusType::NONE);

        // Выбираем тип бонуса
        grid[cellY][cellX].bonus = (rand() % 2 == 0) ? BonusType::HEALTH_UP : BonusType::UPGRADE;
    }
}

BonusType Map::checkBonusPickup(float x, float y) {
    int cellX = static_cast<int>(x / CELL_SIZE);
    int cellY = static_cast<int>(y / CELL_SIZE);

    // Проверяем, есть ли бонус в ячейке, где находится танк
    if (grid[cellY][cellX].bonus != BonusType::NONE) {
        BonusType pickedBonus = grid[cellY][cellX].bonus;
        grid[cellY][cellX].bonus = BonusType::NONE; // Убираем бонус с карты
        return pickedBonus; // Возвращаем тип подобраного бонуса
    }

    return BonusType::NONE; // Бонус не подобран
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

int Map::drawDestroyedParts(float worldX, float worldY, const Cell& cell) {
    int destroyedCount = 0;
    // Проходим по сетке 6x6
    for (int k = 0; k < PARTS_PER_CELL; ++k) {
        for (int l = 0; l < PARTS_PER_CELL; ++l) {

            // Если часть не существует (уничтожена)
            if (!cell.parts[k][l].exist) {
                destroyedCount++;

                // Вычисляем ее мировые координаты
                float partX = worldX + l * PART_SIZE;
                float partY = worldY + k * PART_SIZE;

                // Рисуем "пустоту" (фон) поверх
                glColor3f(0.1f, 0.1f, 0.1f);
                drawRectangle(partX, partY, partX + PART_SIZE, partY + PART_SIZE);
            }
        }
    }
    return destroyedCount;
}

void Map::drawBonuses() {
    for (int i = 0; i < MAP_HEIGHT; ++i) {
        for (int j = 0; j < MAP_WIDTH; ++j) {

            float worldX = j * CELL_SIZE;
            float worldY = i * CELL_SIZE;

            if (grid[i][j].bonus == BonusType::HEALTH_UP) {
                glColor3f(0.9f, 0.9f, 0.9f); // Фон
                drawRectangle(worldX + 8, worldY + 8, worldX + CELL_SIZE - 8, worldY + CELL_SIZE - 8);
                // Красный крест
                glColor3f(0.9f, 0.0f, 0.0f);
                drawRectangle(worldX + 20, worldY + 12, worldX + CELL_SIZE - 20, worldY + CELL_SIZE - 12);
                drawRectangle(worldX + 12, worldY + 20, worldX + CELL_SIZE - 12, worldY + CELL_SIZE - 20);
            }
            else if (grid[i][j].bonus == BonusType::UPGRADE) {
                glColor3f(1.0f, 0.75f, 0.0f); // Фон
                drawRectangle(worldX + 8, worldY + 8, worldX + CELL_SIZE - 8, worldY + CELL_SIZE - 8);
                // Текст "UP"
                glColor3f(0.75f, 0.45f, 0.0f);
                drawText(worldX + 12, worldY + 31, GLUT_BITMAP_HELVETICA_18, "UP");
            }
        }
    }
}

bool Map::isCellLeafs(float x, float y) const {
    int cellX = static_cast<int>(x / CELL_SIZE);
    int cellY = static_cast<int>(y / CELL_SIZE);

    // Проверка на выход за границы
    if (cellX < 0 || cellX >= MAP_WIDTH || cellY < 0 || cellY >= MAP_HEIGHT) {
        return false;
    }

    return grid[cellY][cellX].material == Material::LEAFS;
}