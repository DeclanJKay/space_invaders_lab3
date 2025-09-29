#include <SFML/Graphics.hpp>
#include "game_systems.hpp"
#include "game_parameters.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode(Parameters::game_width, Parameters::game_height), "Space Invaders");

    GameSystem::init();
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Restart on R key press if game over
            if (GameSystem::gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                GameSystem::clean();   // clear ships & bullets
                GameSystem::init();    // re-create player and invaders
                GameSystem::gameOver = false; // reset flag
            }
        }

        float dt = clock.restart().asSeconds();
        GameSystem::update(dt);

        window.clear();
        GameSystem::render(window);
        window.display();
    }

    GameSystem::clean();
    return 0;

    if (GameSystem::gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        GameSystem::clean();
        GameSystem::init();
    }

}
