#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Ship.hpp"

// --------------------------------------------------
// GameSystem: central manager for game state
// --------------------------------------------------
struct GameSystem {
    // Shared resources
    static sf::Texture spriteSheet;                       // Main sprite sheet
    static std::vector<std::shared_ptr<Ship>> ships;      // All ships (player + invaders)

    // Game state
    static bool gameOver;                                 // Tracks if game has ended

    // Functions
    static void init();                                   // Setup ships/resources
    static void clean();                                  // Free/reset memory
    static void update(const float& dt);                  // Update all objects
    static void render(sf::RenderWindow& window);         // Draw everything
};
