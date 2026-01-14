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

enum ElementType : u_int8_t {
    EMPTY,
    SAND,
    WATER,
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
    int gridCols;
    int gridRows;

    // Game Grids
    std::vector<uint8_t> grid;
    std::vector<uint8_t> pixel_buffer;

    // Element Flags
    u_int8_t currentElement;
    std::vector<bool> visited;

    // 

    // Initialize game state (load all shaders/textures/levels)
    void Init(int argc, char* argv[]);

    // Game Loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void Simulator();

    // Elements behaviours
    void Sand_behaviour(int currentCell);
    void Water_behaviour(int currentCell);

    // Movements
    bool Down(int currentCell);
    bool DownLeft(int currentCell);
    bool DownRight(int currentCell);
    bool Left(int currentCell);
    bool Right(int currentCell);
};

#endif