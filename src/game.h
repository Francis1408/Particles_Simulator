#ifndef GAME_H
#define GAME_H


#include "Elements/element.h"
#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <array>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum ElementType : u_int8_t {
    EMPTY,
    SAND,
    WATER,
    WALL,
};


class Game {

    public:

    // Simulation timing
    float simulationStep = 1.0f / 60.0f;
    float simulationSpeed = 1.0f;
    float accumulator = 0.0f;

    // game state
    GameState State;
    bool Keys[1024]; // Array of keys
    bool MouseKeys[8];
    double MouseX, MouseY;
    unsigned int Width, Height; // Width and Height of the window
    int action; // Key action type

    // FLAGS
    bool keyChartOn = false;

    // Constructor
    Game(unsigned int width, unsigned int height);

    // Destructor
    ~Game();

    int pixel_size;
    int gridCols;
    int gridRows;

    int brushSize;

    // Game Grids
    std::vector<uint8_t> grid;
    std::vector<uint8_t> pixel_buffer;

    // Element Flags
    u_int8_t currentElement;
    std::vector<bool> visited;

    // Physiscs
    std::array<Element, 256> elements;
    float gravity = 10.0f;

    // Initialize game state (load all shaders/textures/levels)
    void Init(int argc, char* argv[]);

    // Game Loop
    void ProcessInput(float dt);
    void Update(float dt);
    void StepSimulation(float dt);
    void Render();
    void Simulator();
    


    // Movements
    bool Down(int currentCell);
    bool DownLeft(int currentCell);
    bool DownRight(int currentCell);
    bool Left(int currentCell);
    bool Right(int currentCell);


};

#endif