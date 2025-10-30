#include "Game.h"
#include "Tank.h"
#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Projectile.h"
#include "Graphics.h"
#include <GL/glut.h>
#include <iostream>  // Для отладочного вывода
#include <algorithm> // Для std::remove_if

Game::Game() {
    init();
}

Game::~Game() {
    // Деструктор пуст, т.к. std::unique_ptr автоматически освободит память
}

void Game::init() {
    m_map.loadFromFile("test.txt");

    float playerX, playerY;
    m_map.getPlayerSpawn(playerX, playerY);

    // Создаем игрока
    m_player = std::make_unique<PlayerTank>(playerX, playerY, 1); // 1 = UP

    m_lives = 3;
    m_enemiesLeftToSpawn = 20;
    m_enemySpawnTimer = 3.0f; // Первый враг появится через 3 секунды
    m_isRunning = true;
}

// run() будет вызываться в main.cpp в бесконечном цикле
void Game::run() {
    // В реальном проекте здесь будет расчет deltaTime
    float deltaTime = 0.016f; // Предполагаем, что у нас примерно 60 FPS

    update(deltaTime);
    render();
}

void Game::handleInput(int key, bool isPressed) {
    if (m_gameState != GameState::PLAYING) return;
    if (!m_player->isAlive()) return;

    // Стрельба (обрабатываем здесь)
    if (key == 32 && isPressed) {
        auto newProjectile = m_player->fire(Owner::PLAYER);
        if (newProjectile) {
            m_projectiles.push_back(std::move(newProjectile));
        }
    }
    // Движение (передаем в танк)
    else {
        m_player->handleInput(key, isPressed);
    }
}

void Game::update(float deltaTime) {
    if (m_gameState != GameState::PLAYING) return;

    // 1. Обновляем все объекты
    // Игрок
    if (m_player->isAlive()) {
        m_player->update(deltaTime, m_map);
        BonusType pickedBonus = m_map.checkBonusPickup(m_player->getX(), m_player->getY());
        if (pickedBonus != BonusType::NONE) {
            if (pickedBonus == BonusType::HEALTH_UP) {
                m_lives++;
            }
            else if (pickedBonus == BonusType::UPGRADE) {
                m_player->upgrade();
            }
        }
    }
    else { // Логика смерти и респавна игрока
        if (m_lives > 0) {
            m_lives--;
            if (m_lives > 0) {
                float px, py;
                m_map.getPlayerSpawn(px, py);
                m_player = std::make_unique<PlayerTank>(px, py, 1);
            }
            else {
                m_gameState = GameState::GAME_OVER;
                std::cout << "GAME OVER! You ran out of lives." << std::endl;
            }
        }
    }

    // Враги
    for (auto& enemy : m_enemies) {
        enemy->update(deltaTime, m_map);
        if (enemy->getFireCooldown() <= 0.0f) {
            auto newProjectile = enemy->fire(Owner::ENEMY);
            if (newProjectile) m_projectiles.push_back(std::move(newProjectile));
        }
    }

    // Снаряды
    for (auto& projectile : m_projectiles) {
        projectile->update(deltaTime, m_map);
    }

    // 2. Спавн врагов и бонусов
    spawnEnemyTank(deltaTime);
    m_map.spawnBonus();

    // 3. Проверка столкновений
    checkCollisions();

    // 4. Очистка "мертвых" объектов
    cleanupObjects();

    // 5. Проверка условия победы
    if (m_enemies.empty() && m_enemiesLeftToSpawn <= 0) {
        m_gameState = GameState::WIN;
        std::cout << "YOU WIN!" << std::endl;
    }
}

// Добавьте эту вспомогательную функцию внутрь класса Game в Game.h как private,
// а ее реализацию в Game.cpp
void Game::drawInfoPanel() {
    // Размеры из battlecity.c
    const int MAP_PIXEL_WIDTH = 13 * 48; // MAP_WIDTH * CELL_SIZE
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 624;

    glColor3f(0.5f, 0.5f, 0.5f);
    drawRectangle(MAP_PIXEL_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawText(MAP_PIXEL_WIDTH + 12, 200, GLUT_BITMAP_TIMES_ROMAN_24, "HP: " + std::to_string(m_lives));
    drawText(MAP_PIXEL_WIDTH + 12, 230, GLUT_BITMAP_TIMES_ROMAN_24, "Tanks: " + std::to_string(m_enemiesLeftToSpawn + m_enemies.size()));
}


void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT); // Очищаем экран

    // 1. Рисуем нижний слой карты (все, кроме листьев)
    m_map.draw(false);

    // 2. Рисуем снаряды
    for (auto& projectile : m_projectiles) {
        projectile->draw();
    }

    // 3. Рисуем бонусы (предполагаем, что это часть отрисовки карты)
    // m_map.drawBonuses(); // Если бы выделили в отдельный метод

    // 4. Рисуем танк игрока
    if (m_player->isAlive()) {
        m_player->draw();
    }

    // 5. Рисуем танки врагов
    for (auto& enemy : m_enemies) {
        enemy->draw();
    }

    // 6. Рисуем верхний слой карты (листья), чтобы они были поверх танков
    m_map.draw(true);

    // 7. Рисуем информационную панель
    drawInfoPanel();

    // 8. Показываем готовый кадр
    glutSwapBuffers();
}

void Game::spawnEnemyTank(float deltaTime) {
    m_enemySpawnTimer -= deltaTime;
    if (m_enemySpawnTimer <= 0.0f && m_enemiesLeftToSpawn > 0 && m_enemies.size() < 4) {
        m_enemySpawnTimer = 5.0f; // Следующий через 5 секунд

        int spawnIndex = rand() % 3;
        float spawnX, spawnY;
        m_map.getEnemySpawn(spawnIndex, spawnX, spawnY);

        m_enemies.push_back(std::make_unique<EnemyTank>(spawnX, spawnY, 2)); // 2 = DOWN
        m_enemiesLeftToSpawn--;
    }
}

void Game::checkCollisions() {
    // Столкновения снарядов с картой
    for (auto& proj : m_projectiles) {
        if (!proj->isAlive()) continue;

        // Используем новую логику с ProjectileHitResult
        float projX = proj->getX(); // Копируем, так как функция может их изменить
        float projY = proj->getY();
        ProjectileHitResult result = m_map.checkProjectileHit(projX, projY, proj->getDirection(), proj->getDamage());

        if (result != ProjectileHitResult::NONE) {
            proj->destroy(); // Уничтожаем снаряд
            if (result == ProjectileHitResult::HQ_HIT) {
                m_gameState = GameState::GAME_OVER;
                std::cout << "GAME OVER! Your HQ was destroyed." << std::endl;
                return; // Сразу выходим, чтобы остановить обработку
            }
        }
    }

    // Столкновения снарядов с танками (остается без изменений)
    for (auto& proj : m_projectiles) {
        if (!proj->isAlive()) continue;

        if (proj->getOwner() == Owner::PLAYER) { // Снаряд игрока
            for (auto& enemy : m_enemies) {
                if (!enemy->isAlive()) continue;
                if (proj->getX() < enemy->getX() + 20 && proj->getX() > enemy->getX() - 20 &&
                    proj->getY() < enemy->getY() + 20 && proj->getY() > enemy->getY() - 20) {
                    enemy->takeDamage(proj->getDamage());
                    proj->destroy();
                    break;
                }
            }
        }
        else { // Снаряд врага
            if (!m_player->isAlive()) continue;
            if (proj->getX() < m_player->getX() + 20 && proj->getX() > m_player->getX() - 20 &&
                proj->getY() < m_player->getY() + 20 && proj->getY() > m_player->getY() - 20) {
                m_player->takeDamage(proj->getDamage());
                proj->destroy();
            }
        }
    }
}


void Game::cleanupObjects() {
    // Удаляем "мертвые" снаряды
    m_projectiles.erase(
        std::remove_if(m_projectiles.begin(), m_projectiles.end(),
            [](const std::unique_ptr<Projectile>& p) {
                return !p->isAlive();
            }),
        m_projectiles.end()
    );

    // Удаляем "мертвых" врагов
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [](const std::unique_ptr<EnemyTank>& e) {
                return !e->isAlive();
            }),
        m_enemies.end()
    );
}