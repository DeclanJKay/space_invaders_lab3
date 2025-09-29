#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Ship.hpp"

struct GameSystem {
    static sf::Texture spriteSheet;
    static std::vector<std::shared_ptr<Ship>> ships;

    static void init();
    static void clean();
    static void update(const float& dt);
    static void render(sf::RenderWindow& window);
};
