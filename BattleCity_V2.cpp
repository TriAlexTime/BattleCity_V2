
#include <GL/glut.h>
#include "Game.h"
#include <memory> // для std::unique_ptr

// Глобальный указатель на наш игровой объект.
// Он нужен, чтобы "статичные" функции GLUT могли до него добраться.
std::unique_ptr<Game> game;

// --- Функции-обертки для GLUT Callbacks ---

void display() {
    if (game) {
        game->render();
    }
}

void timer(int value) {
    if (game) {
        game->run(); // run() содержит в себе update() и render()
    }
    glutTimerFunc(16, timer, 0); // ~60 FPS
    glutPostRedisplay(); // Запрос на перерисовку кадра
}

void handleKeyPress(unsigned char key, int x, int y) {
    if (game) {
        game->handleInput(key, true); // true - клавиша нажата
    }
}

void handleKeyRelease(unsigned char key, int x, int y) {
    if (game) {
        game->handleInput(key, false); // false - клавиша отпущена
    }
}

// Обработка стрелок
void handleSpecialKeyPress(int key, int x, int y) {
    if (game) {
        // GLUT_KEY_UP/DOWN и т.д. имеют другие коды, чем ' '
        // Мы передаем их как есть
        game->handleInput(key, true);
    }
}

void handleSpecialKeyRelease(int key, int x, int y) {
    if (game) {
        game->handleInput(key, false);
    }
}

// --- Точка входа в программу ---

int main(int argc, char** argv) {
    // Инициализация GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 624); // Размеры из твоего проекта
    glutCreateWindow("Battle City V2 - OOP Edition");

    // Настройка ортографической проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 624, 0);
    glMatrixMode(GL_MODELVIEW);

    // Создаем главный объект игры
    game = std::make_unique<Game>();

    // Регистрируем наши функции-обертки в GLUT
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyPress);
    glutKeyboardUpFunc(handleKeyRelease);
    glutSpecialFunc(handleSpecialKeyPress);
    glutSpecialUpFunc(handleSpecialKeyRelease);
    glutTimerFunc(16, timer, 0); // Запускаем игровой цикл

    // Запускаем главный цикл GLUT, который обрабатывает все события
    glutMainLoop();

    return 0;
}