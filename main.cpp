#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>  

// Global variables
sf::RenderWindow window;       // The game window
sf::Texture spritesheet;       // The sprite sheet texture
sf::Sprite invader;            // The invader sprite

// Initialization
void init() {
    std::cout << "Current working directory: "
        << std::filesystem::current_path() << std::endl;

    window.create(sf::VideoMode(800, 600), "Space Invaders");

    if (!spritesheet.loadFromFile("res/img/invaders_sheet.png")) {
        std::cerr << "Failed to load spritesheet!" << std::endl;
    }

    invader.setTexture(spritesheet);
    invader.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)));
}

// Rendering
void render() {
    window.clear();
    window.draw(invader);
    window.display();
}

int main() {
    init();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        render();
    }

    return 0;
}
