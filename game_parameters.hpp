//game_parameters.hpp
#pragma once //insure that this header file is included only once and there will no multiple definition of the same thing

struct Parameters {
    static constexpr int game_width = 800;
    static constexpr int game_height = 600;
    static constexpr int sprite_size = 32;
    static constexpr int rows = 4;      // number of invader rows
    static constexpr int columns = 8;   // number of invader columns
    static constexpr float bullet_speed = 200.f;
};