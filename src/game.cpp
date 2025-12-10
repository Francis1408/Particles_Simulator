#include "game.h"
#include "utils/resourceManager.h"
#include "utils/spriteRenderer.h"

// GLM Mathematics Library headers
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include <iostream>
#include <vector>
#include <algorithm> 
#include <filesystem>

SpriteRenderer *CubeRenderer;


// Constructor
Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

// Destructor
Game::~Game()
{
   delete CubeRenderer;
}

void Game::Init(int argc, char* argv[])
{
    // SIMULATOR PRE-PROCESSING CONFIGS
    ResourceManager::LoadShader("Shaders/basicShader.vs", "Shaders/basicShader.fs", nullptr, "cube");

    // Define the View Matrix - Game is oriented from top to bottom
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    ResourceManager::GetShader("cube").Use().SetMat4("projection", projection);

    Shader Shader = ResourceManager::GetShader("cube");
    CubeRenderer = new SpriteRenderer(Shader);
}

void Game::Update(float dt)
{
    
}

void Game::ProcessInput(float dt)
{
    
}

void Game::Render()
{
   
    if (this->MouseKeys[GLFW_MOUSE_BUTTON_LEFT]) {
        glm::vec2 drawCoord = glm::vec2(this->MouseX, this->MouseY);

        CubeRenderer->DrawSprite(drawCoord, glm::vec2(1.0f, 1.0f));
    }

}


