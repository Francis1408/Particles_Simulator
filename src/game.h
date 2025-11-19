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

    // Array of sprite distances
    std::vector<float> spriteDistance;
    // Array to store the order of the sprites from fartest to the nearst
    std::vector<int> spriteOrder;

    // Number of sprites avaiable
    unsigned int numSprites;

    unsigned int Level;

    // Constructor
    Game(unsigned int width, unsigned int height);

    // Destructor
    ~Game();

    // Initialize game state (load all shaders/textures/levels)
    void Init(int argc, char* argv[]);

    // Game Loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

};

#endif