#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite {
public:
    static void Init();
    static void Update(const float& dt);
    static void Render(sf::RenderWindow& window);
    static void Fire(const sf::Vector2f& pos, bool mode);

    Bullet();
    ~Bullet() = default;

protected:
    void _Update(const float& dt);
    bool _mode = false;
    static unsigned char _bulletPointer;
    static Bullet _bullets[256];
};
