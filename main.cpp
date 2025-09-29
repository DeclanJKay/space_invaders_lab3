#include <SFML/Graphics.hpp>
#include "game_systems.hpp"
#include "game_parameters.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode(Parameters::game_width, Parameters::game_height), "Space Invaders");

    GameSystem::init();
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();
        GameSystem::update(dt);

        window.clear();
        GameSystem::render(window);
        window.display();
    }

    GameSystem::clean();
    return 0;
}
