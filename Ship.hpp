#pragma once
#include <SFML/Graphics.hpp>

class Ship : public sf::Sprite {
public:
    Ship();
    Ship(const Ship& s);
    Ship(sf::IntRect ir);
    virtual ~Ship() = 0;

    virtual void Update(const float& dt);
    virtual void explode();
    bool is_exploded() const;
    bool should_remove() const;

protected:
    sf::IntRect _sprite;
    bool _exploded = false;          // Whether ship has exploded
    float _explosionTimer = 0.f;     // Time before ship is removed after explosion
    float _explosionAnimTimer = 0.f; // Timer for switching explosion frames
    int _currentExplosionFrame = 0;  // Which explosion frame is showing
};

// ------------------ Invader ------------------
class Invader : public Ship {
public:
    static bool direction;
    static float speed;
    static constexpr float acc = 5.f;
    bool IsBottomMost() const;



    Invader();
    Invader(const Invader& inv);
    Invader(sf::IntRect ir, sf::Vector2f pos);

    void Update(const float& dt) override;
    void MoveDown();
};

// ------------------ Player ------------------
class Player : public Ship {
public:
    Player();
    void Update(const float& dt) override;
};
