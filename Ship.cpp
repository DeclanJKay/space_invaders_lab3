#include "Ship.hpp"
#include "game_parameters.hpp"
#include "game_systems.hpp"
#include "bullet.hpp"

using param = Parameters;
using gs = GameSystem;

// ------------------ Ship ------------------
Ship::Ship() = default;
Ship::Ship(const Ship& s) : _sprite(s._sprite) {}
Ship::Ship(sf::IntRect ir) : _sprite(ir) {
    setTexture(gs::spriteSheet);
    setTextureRect(_sprite);
}
Ship::~Ship() = default;

void Ship::Update(const float& dt) {
    // Count down explosion timer if exploded
    if (_exploded) {
        _explosionTimer -= dt;
    }
}

void Ship::explode() {
    if (!_exploded) {
        _exploded = true;
        _explosionTimer = 0.6f; // Show explosion for 0.6 seconds
        setTextureRect(sf::IntRect(128, 32, 32, 32)); // Single explosion frame
    }
}

bool Ship::is_exploded() const {
    return _exploded;
}

bool Ship::should_remove() const {
    // Remove ship after explosion timer ends
    return _exploded && _explosionTimer <= 0.f;
}

// ------------------ Invader ------------------
bool Invader::direction = true;
float Invader::speed = 20.f;
constexpr float Invader::acc;

Invader::Invader() : Ship() {}
Invader::Invader(const Invader& inv) : Ship(inv) {}
Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
    setOrigin(param::sprite_size / 2.f, param::sprite_size / 2.f);
    setPosition(pos);
}

void Invader::Update(const float& dt) {
    Ship::Update(dt);

    // Stop movement if exploded
    if (_exploded) return;

    // Movement left-right
    move(dt * (direction ? 1.f : -1.f) * speed, 0.f);

    // Edge detection → reverse direction + drop
    if ((direction && getPosition().x > param::game_width - param::sprite_size / 2.f) ||
        (!direction && getPosition().x < param::sprite_size / 2.f)) {
        direction = !direction;
        speed += acc;
        for (auto& ship : gs::ships) {
            if (auto inv = std::dynamic_pointer_cast<Invader>(ship))
                inv->MoveDown();
        }
    }

    // Animate invaders (toggle frame 0 ↔ 32)
    static float animTime = 0.f;
    animTime += dt;
    if (animTime > 0.5f) {
        int left = (getTextureRect().left == 0) ? 32 : 0;
        setTextureRect(sf::IntRect(left, getTextureRect().top, param::sprite_size, param::sprite_size));
        animTime = 0.f;
    }

    // Shooting → only bottom row invaders fire
    static float firetime = 0.f;
    firetime -= dt;
    if (firetime <= 0.f && rand() % 100 == 0 && IsBottomMost()) {
        Bullet::Fire(getPosition(), true);
        firetime = 4.0f + (rand() % 60);
    }
}

void Invader::MoveDown() { move(0.f, param::sprite_size); }

bool Invader::IsBottomMost() const {
    float x = getPosition().x;
    float y = getPosition().y;

    for (auto& ship : gs::ships) {
        auto inv = std::dynamic_pointer_cast<Invader>(ship);
        if (inv && !inv->is_exploded() && inv->getPosition().x == x && inv->getPosition().y > y) {
            return false;
        }
    }
    return true;
}

// ------------------ Player ------------------
Player::Player() :
    Ship(sf::IntRect(param::sprite_size * 5, param::sprite_size, param::sprite_size, param::sprite_size)) {
    setOrigin(param::sprite_size / 2.f, param::sprite_size / 2.f);
    setPosition(param::game_width / 2.f, param::game_height - static_cast<float>(param::sprite_size));
}

void Player::Update(const float& dt) {
    Ship::Update(dt);

    // Player movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && getPosition().x > param::sprite_size / 2.f)
        move(-200.f * dt, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && getPosition().x < param::game_width - param::sprite_size / 2.f)
        move(200.f * dt, 0.f);

    // Shooting with cooldown
    static float firetime = 0.f;
    firetime -= dt;
    if (firetime <= 0.f && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        Bullet::Fire(getPosition(), false);
        firetime = 0.7f;
    }
}
