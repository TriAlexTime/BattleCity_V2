#pragma once

#include "GameObject.h"
#include <memory> // Для std::unique_ptr (умный указатель)
#include "Projectile.h"

class Projectile;

class Tank : public GameObject {
public:
    Tank(float startX, float startY, int startDir);

    // 'override' - это подсказка компилятору, что мы намеренно переопределяем метод базового класса.

    void update(float deltaTime, const Map& map) override;
    bool isAlive() const override;

    void draw() override = 0; // Этот метод будут вызывать снаружи

    void takeDamage(int dmg);

    /*
     Пытается выстрелить.
     @return Умный указатель на новый снаряд, если выстрел возможен (нет перезарядки), иначе nullptr.
    */
    std::unique_ptr<Projectile> fire(Owner owner);

    // Геттер для здоровья
    int getHealth() const { return health; }

    // Улучшает танк (увеличивает урон)
    void upgrade();

    // Устанавливает позицию танка
    void setPosition(float newX, float newY);

    // Возвращает, движется ли танк
    bool getIsMoving() const;

    float getFireCooldown() const { return fireCooldown; }

protected:

    int health = 1;
    int damage = 1;
    float speed = 8.0f;
    bool isMoving = false;

    void draw(float color1[], float color2[], float color3[]);

    // Таймер перезарядки, чтобы танк не стрелял очередью
    float fireCooldown = 0.0f;
    const float fireRate = 0.5f; // 1 выстрел каждые 0.5 сек (условно)
};