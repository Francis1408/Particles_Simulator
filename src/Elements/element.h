#pragma once
#include <cstdint>

class Game;

struct Element {
    float density;
    bool solid;

    void (*update)(Game&, int cell);
};