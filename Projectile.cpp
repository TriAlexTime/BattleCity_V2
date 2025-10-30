#include "Projectile.h"
#include "Map.h"
#include "Graphics.h"
#include <GL/glut.h>

Projectile::Projectile(float startX, float startY, int dir, int dmg, Owner owner) {
    this->x = startX;
    this->y = startY;
    this->direction = dir;
    this->m_damage = dmg;
    this->m_owner = owner;

    // Снаряды движутся быстрее танков
    this->m_speed = 250.0f; // Скорость в пикселях в секунду
    this->m_isAlive = true;

    this->width = 4;
    this->height = 4;
}

void Projectile::update(float deltaTime, const Map& map) {
    if (!m_isAlive) {
        return;
    }

    // Двигаем снаряд в зависимости от его направления
    switch (direction) {
    case 1: y -= m_speed * deltaTime; break; // UP
    case 2: y += m_speed * deltaTime; break; // DOWN
    case 3: x -= m_speed * deltaTime; break; // LEFT
    case 4: x += m_speed * deltaTime; break; // RIGHT
    }

    // Проверяем, не вылетел ли снаряд за пределы карты.
    // Если да, то помечаем его на удаление.
    // Размеры карты 13*48 = 624
    if (x < 0 || x > 13 * 48 || y < 0 || y > 13 * 48) {
        m_isAlive = false;
    }
}

bool Projectile::isAlive() const {
    return m_isAlive;
}

void Projectile::destroy() {
    m_isAlive = false;
}

void Projectile::draw() {
    if (!m_isAlive) return;
    glColor3f(1.0f, 0.9f, 0.0f); // Желтый цвет
    drawRectangle(x - 2, y - 2, x + 2, y + 2);
}


