#include "bullet.hpp"
#include "game_systems.hpp"
#include "game_parameters.hpp"

using param = Parameters;
using gs = GameSystem;

unsigned char Bullet::_bulletPointer = 0;
Bullet Bullet::_bullets[256];

Bullet::Bullet() {}

void Bullet::Init() {
    for (auto& b : _bullets) {
        b.setTexture(gs::spriteSheet);
        b.setOrigin(param::sprite_size / 2.f, param::sprite_size / 2.f);
        b.setPosition(-100, -100); // off-screen by default
    }
}

void Bullet::Fire(const sf::Vector2f& pos, bool mode) {
    Bullet& b = _bullets[++_bulletPointer];
    b._mode = mode;

    // Select correct bullet sprite
    b.setTextureRect(mode ?
        sf::IntRect(96, 32, 32, 32) :  // Enemy bullet
        sf::IntRect(64, 32, 32, 32));  // Player bullet

    // Always spawn centered directly above/below shooter
    b.setPosition(pos.x, pos.y + (mode ? param::sprite_size / 2.f : -param::sprite_size / 2.f));
}


void Bullet::Update(const float& dt) {
    for (auto& b : _bullets)
        b._Update(dt);
}

void Bullet::Render(sf::RenderWindow& window) {
    for (auto& b : _bullets)
        window.draw(b);
}

void Bullet::_Update(const float& dt) {
    // Move bullet
    move(0, dt * param::bullet_speed * (_mode ? 1.f : -1.f));

    // Off-screen check
    if (getPosition().y < -param::sprite_size || getPosition().y > param::game_height + param::sprite_size) {
        return;
    }

    // Collision detection
    auto box = getGlobalBounds();
    auto player = gs::ships[0]; // First ship is player

    for (auto& s : gs::ships) {
        if (_mode) {
            // Enemy bullets hit only player
            if (s != player) continue;
        }
        else {
            // Player bullets hit only invaders
            if (s == player) continue;
        }

        // Hit check
        if (!s->is_exploded() && s->getGlobalBounds().intersects(box)) {
            s->explode();
            setPosition(-100, -100); // remove bullet
            return;
        }
    }
}
