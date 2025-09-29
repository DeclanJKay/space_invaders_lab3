#include "game_systems.hpp"
#include "game_parameters.hpp"
#include "Ship.hpp"
#include "bullet.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> // for std::remove_if

using param = Parameters;

// --------------------------
// Static variable definitions
// --------------------------
sf::Texture GameSystem::spriteSheet;
std::vector<std::shared_ptr<Ship>> GameSystem::ships;
bool GameSystem::gameOver = false; // new game state flag

// --------------------------
// init()
// --------------------------
void GameSystem::init() {
    if (!spriteSheet.loadFromFile("res/img/invaders_sheet.png")) {
        std::cerr << "Failed to load sprite sheet!" << std::endl;
    }

    // Reset state
    ships.clear();
    gameOver = false;

    // Initialize bullets
    Bullet::Init();

    // Player at index 0
    auto player = std::make_shared<Player>();
    ships.push_back(player);

    // Create invader grid (no more test invader)
    for (int r = 0; r < param::rows; ++r) {
        for (int c = 0; c < param::columns; ++c) {
            auto inv = std::make_shared<Invader>(
                sf::IntRect(0, 0, param::sprite_size, param::sprite_size),  // sprite frame
                sf::Vector2f(100.f + c * 50.f, 50.f + r * 50.f)             // position
            );
            ships.push_back(inv);
        }
    }
}

// --------------------------
// clean()
// --------------------------
void GameSystem::clean() {
    for (auto& ship : ships) ship.reset();
    ships.clear();
}

// --------------------------
// update()
// --------------------------
void GameSystem::update(const float& dt) {
    // Stop updating if game over
    if (gameOver) return;

    // Update all ships
    for (auto& s : ships) {
        s->Update(dt);
    }

    // Update bullets
    Bullet::Update(dt);

    // Remove ships after explosion animation finishes
    ships.erase(std::remove_if(ships.begin(), ships.end(),
        [](const std::shared_ptr<Ship>& s) { return s->should_remove(); }),
        ships.end());

    // --------------------------
    // Game Over condition
    // --------------------------
    // If the player (index 0) is exploded, mark game over
    if (!ships.empty() && ships[0]->is_exploded()) {
        gameOver = true;
    }
}

// --------------------------
// render()
// --------------------------
void GameSystem::render(sf::RenderWindow& window) {
    // Draw all ships
    for (const auto& s : ships) {
        window.draw(*s);
    }

    // Draw bullets
    Bullet::Render(window);

    // Draw Game Over screen
    if (gameOver) {
        sf::Font font;
        if (font.loadFromFile("res/fonts/arial.ttf")) { // make sure this font exists
            sf::Text text("GAME OVER - Press R to Restart", font, 32);
            text.setFillColor(sf::Color::Red);
            text.setStyle(sf::Text::Bold);
            text.setPosition(100, param::game_height / 2);
            window.draw(text);
        }
    }
}
