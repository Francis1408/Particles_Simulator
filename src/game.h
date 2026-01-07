#ifndef GAME_H
#define GAME_H

#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

class Game {

    public:
    // game state
    GameState State;
    bool Keys[1024]; // Array of keys
    bool MouseKeys[8];
    double MouseX, MouseY;
    unsigned int Width, Height; // Width and Height of the window
    int action; // Key action type

    // FLAGS
    bool keyChartOn = false;

    // ZBuffer
    std::vector<float> ZBuffer;

    // Constructor
    Game(unsigned int width, unsigned int height);

    // Destructor
    ~Game();

    int pixel_size;
    float gridCols;
    float gridRows;

    // Game Grids
    std::vector<uint8_t> grid;
    std::vector<uint8_t> pixel_buffer;

    // Initialize game state (load all shaders/textures/levels)
    void Init(int argc, char* argv[]);

    // Game Loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void Simulator();
    bool Down(int currentCell);
    bool DownLeft(int currentCell);
    bool DownRight(int currentCell);

};

#endif