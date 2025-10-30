#pragma once

/*
 ��� ����� ����� ��� ���� ������������ �������� �� ������� �����
*/
class Map;

class GameObject {
public:
    // ����������� ����������. ���������� ��� ������� �������, ����� ��������� ������������� ������ �����������.
    virtual ~GameObject() {}

    /*
     ��������� ������ ������� (��������, ������� � �.�.).
    @param deltaTime �����, ��������� � ���������� �����.
    */
    virtual void update(float deltaTime, const Map& map) = 0;

    /*
     ������������ ������ �� ������.
    */
    virtual void draw() = 0;

    /*
     ���������, "���" �� ��� ������.
    */
    virtual bool isAlive() const = 0; // 'const' �������, ��� ����� �� ������� ������


    float getX() const { return x; }
    float getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getDirection() const { return direction; }

protected:
    // 'protected' ��������, ��� ��� ���� �����
    // ������ ������ GameObject � ���� ��� ����������� (Tank, Projectile).

    float x = 0.0f;
    float y = 0.0f;
    int width = 40;  // ������� ����� (�� ������ `checkCollision` 20+20)
    int height = 40;

    // 1:UP, 2:DOWN, 3:LEFT, 4:RIGHT
    int direction = 1;
};