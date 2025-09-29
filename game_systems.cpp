#include "game_systems.hpp"
#include "game_parameters.hpp"
#include "Ship.hpp"
#include "bullet.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using param = Parameters;

sf::Texture GameSystem::spriteSheet;
std::vector<std::shared_ptr<Ship>> GameSystem::ships;

void GameSystem::init() {
    if (!spriteSheet.loadFromFile("res/img/invaders_sheet.png")) {
        std::cerr << "Failed to load sprite sheet!" << std::endl;
    }

    // Initialize bullets
    Bullet::Init();

    // Player at index 0
    auto player = std::make_shared<Player>();
    ships.push_back(player);

    // All invaders use the same sprite from the first row
    for (int r = 0; r < param::rows; ++r) {
        for (int c = 0; c < param::columns; ++c) {
            auto inv = std::make_shared<Invader>(
                sf::IntRect(0, 0, param::sprite_size, param::sprite_size),  // always same sprite
                sf::Vector2f(100.f + c * 50.f, 50.f + r * 50.f)
            );
            ships.push_back(inv);
        }
    }
}

void GameSystem::clean() {
    for (auto& ship : ships) ship.reset();
    ships.clear();
}

void GameSystem::update(const float& dt) {
    for (auto& s : ships) {
        s->Update(dt);
    }
    Bullet::Update(dt);

    // Remove ships after explosion animation finishes
    ships.erase(std::remove_if(ships.begin(), ships.end(),
        [](const std::shared_ptr<Ship>& s) { return s->should_remove(); }),
        ships.end());
}

void GameSystem::render(sf::RenderWindow& window) {
    for (const auto& s : ships) {
        window.draw(*s);
    }
    Bullet::Render(window);
}
