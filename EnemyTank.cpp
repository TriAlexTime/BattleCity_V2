#include "EnemyTank.h"
#include "Map.h"
#include <cstdlib>

// Конструктор просто вызывает конструктор базового класса
EnemyTank::EnemyTank(float startX, float startY, int startDir)
    : Tank(startX, startY, startDir) {
    // Здесь можно инициализировать специфичные для ИИ переменные, если нужно
}

void EnemyTank::update(float deltaTime, const Map& map) {
    // Вызываем update базового класса Tank.
    // Он отвечает за таймер перезарядки (fireCooldown) и базовую логику движения.
    Tank::update(deltaTime, map);

    aiTimer += deltaTime;

    // --- ЛОГИКА ПРИНЯТИЯ РЕШЕНИЙ ИИ ---

    // Каждые 0.8 секунды танк "думает", что делать дальше
    if (aiTimer > 0.8f) {
        aiTimer = 0.0f; // Сбрасываем таймер

        // Проверяем, не уперлись ли мы в стену
        if (!map.checkTankCollision(this->x, this->y, this->direction)) {
            // Путь заблокирован. Выбираем новое случайное направление.
            int oldDirection = this->direction;
            // Цикл, чтобы гарантированно получить новое направление
            while (this->direction == oldDirection) {
                this->direction = (rand() % 4) + 1; // Случайное направление от 1 до 4
            }
        }
        else {
            // Путь свободен. Есть 20% шанс, что танк все равно сменит направление.
            // Это делает поведение менее предсказуемым.
            if ((rand() % 100) < 20) {
                this->direction = (rand() % 4) + 1;
            }
        }
    }

    // --- ЛОГИКА ДВИЖЕНИЯ ---

    // Пытаемся двигаться в текущем направлении
    if (map.checkTankCollision(this->x, this->y, this->direction)) {
        this->isMoving = true;
    }
    else {
        this->isMoving = false;
    }

    // Логика стрельбы будет управляться из главного игрового цикла (в классе Game),
    // который будет вызывать метод fire(), когда fireCooldown <= 0.
    // Здесь мы просто управляем состоянием танка.
}

void EnemyTank::draw() {
    float enemyColor1[3] = { 0.35f, 0.35f, 0.35f };
    float enemyColor2[3] = { 0.65f, 0.65f, 0.65f };
    float enemyColor3[3] = { 0.85f, 0.85f, 0.85f };
    Tank::draw(enemyColor1, enemyColor2, enemyColor3);
}