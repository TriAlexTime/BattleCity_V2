#include "EnemyTank.h"
#include "Map.h"
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>


// Конструктор просто вызывает конструктор базового класса
EnemyTank::EnemyTank(float startX, float startY, int startDir)
    : Tank(startX, startY, startDir) {
    // Здесь можно инициализировать специфичные для ИИ переменные, если нужно
}

/*
 Проверяет, есть ли прямая линия огня до игрока.
 Это упрощенная "проверка прямой видимости".
*/
bool EnemyTank::hasLineOfSight(const Map& map, float playerX, float playerY) {
    // Проверяем, на одной ли мы оси (с погрешностью)
    bool alignedX = std::abs(this->x - playerX) < 20.0f;
    bool alignedY = std::abs(this->y - playerY) < 20.0f;

    if (alignedX) {
        // Выровнены по X, проверяем вертикальную линию
        int dir = (playerY > this->y) ? 2 : 1; // 2=DOWN, 1=UP
        return map.checkTankCollision(this->x, this->y, dir); // Используем checkTankCollision как упрощенный "рейкаст"
    }
    else if (alignedY) {
        // Выровнены по Y, проверяем горизонтальную линию
        int dir = (playerX > this->x) ? 4 : 3; // 4=RIGHT, 3=LEFT
        return map.checkTankCollision(this->x, this->y, dir);
    }
    return false; // Не на одной линии
}

void EnemyTank::update(float deltaTime, const Map& map, float playerX, float playerY) {
    // Вызываем update базового класса (таймер перезарядки)
    Tank::update(deltaTime, map, playerX, playerY);

    aiTimer += deltaTime;

    // --- ЛОГИКА ПРИНЯТИЯ РЕШЕНИЙ ---
    if (aiTimer > 0.8f) { // Решаем что делать каждые 0.8 сек
        aiTimer = 0.0f; // Сбрасываем таймер

        // 1. Проверяем, видим ли мы игрока
        bool playerObscured = map.isCellLeafs(playerX, playerY);
        bool seesPlayer = false;

        if (!playerObscured) { // Если игрок НЕ в листьях
            seesPlayer = hasLineOfSight(map, playerX, playerY);
        }

        // 2. РЕЖИМ "ОХОТЫ"
        if (seesPlayer) {
            m_wantsToFire = true; // Мы хотим стрелять!

            // Пытаемся повернуться к игроку
            if (std::abs(this->x - playerX) < 20.0f) {
                this->direction = (playerY > this->y) ? 2 : 1;
            }
            else if (std::abs(this->y - playerY) < 20.0f) {
                this->direction = (playerX > this->x) ? 4 : 3;
            }
        }
        // 3. РЕЖИМ "БЛУЖДАНИЯ" (игрок в листьях или за стеной)
        else {
            m_wantsToFire = false; // Мы не видим игрока, стрелять не хотим

            // Проверяем, не уперлись ли мы в стену
            if (!map.checkTankCollision(this->x, this->y, this->direction)) {
                // Путь заблокирован. Ищем новый.
                std::vector<int> directions = { 1, 2, 3, 4 };
                std::random_device rd;
                std::mt19937 g(rd());
                std::shuffle(directions.begin(), directions.end(), g);

                for (int newDir : directions) {
                    if (map.checkTankCollision(this->x, this->y, newDir)) {
                        this->direction = newDir;
                        break;
                    }
                }
            }
            else {
                // Путь свободен. 20% шанс, что танк все равно сменит направление.
                if ((rand() % 100) < 20) {
                    this->direction = (rand() % 4) + 1;
                }
            }
        }
    }

    // --- ЛОГИКА ДВИЖЕНИЯ И БЛОКИРОВКИ ---
    // (Эта логика выполняется каждый кадр)
    if (map.checkTankCollision(this->x, this->y, this->direction)) {
        this->isMoving = true;
        this->m_isBlockedByWall = false; // Путь свободен
    }
    else {
        this->isMoving = false;
        this->m_isBlockedByWall = true; // Уперлись в стену
    }
}

void EnemyTank::draw() {
    float enemyColor1[3] = { 0.35f, 0.35f, 0.35f };
    float enemyColor2[3] = { 0.65f, 0.65f, 0.65f };
    float enemyColor3[3] = { 0.85f, 0.85f, 0.85f };
    Tank::draw(enemyColor1, enemyColor2, enemyColor3);
}