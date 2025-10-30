#pragma once

/*
 Тут будет класс для всех динамических объектов на игровой карте
*/
class Map;

class GameObject {
public:
    // Виртуальный деструктор. Обязателен для базовых классов, чтобы корректно освобождалась память наследников.
    virtual ~GameObject() {}

    /*
     Обновляет логику объекта (движение, таймеры и т.д.).
    @param deltaTime Время, прошедшее с последнего кадра.
    */
    virtual void update(float deltaTime, const Map& map) = 0;

    /*
     Отрисовывает объект на экране.
    */
    virtual void draw() = 0;

    /*
     Проверяет, "жив" ли еще объект.
    */
    virtual bool isAlive() const = 0; // 'const' обещает, что метод не изменит объект


    float getX() const { return x; }
    float getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getDirection() const { return direction; }

protected:
    // 'protected' означает, что эти поля видны
    // самому классу GameObject и всем его наследникам (Tank, Projectile).

    float x = 0.0f;
    float y = 0.0f;
    int width = 40;  // Размеры танка (из твоего `checkCollision` 20+20)
    int height = 40;

    // 1:UP, 2:DOWN, 3:LEFT, 4:RIGHT
    int direction = 1;
};