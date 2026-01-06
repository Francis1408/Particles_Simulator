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
SpriteRenderer *GridRenderer;

Texture2D *gridTexture;


// Constructor
Game::Game(unsigned int width, unsigned int height) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{ 

}

// Destructor
Game::~Game()
{
   delete CubeRenderer;
   delete GridRenderer;

   delete gridTexture;
   gridTexture = nullptr;
}

void Game::Init(int argc, char* argv[])
{
    // SIMULATOR PRE-PROCESSING CONFIGS
    // ResourceManager::LoadShader("Shaders/basicShader.vs", "Shaders/basicShader.fs", nullptr, "cube");
    ResourceManager::LoadShader("Shaders/shaderCoordinate.vs", "Shaders/shaderFloor.fs", nullptr, "grid");
    

    // Define the View Matrix - Game is oriented from top to bottom
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

    // ResourceManager::GetShader("cube").Use().SetMat4("projection", projection);
    ResourceManager::GetShader("grid").Use().SetInt("image", 0);
    ResourceManager::GetShader("grid").SetMat4("projection", projection);

    // Shader Shader = ResourceManager::GetShader("cube");
    // CubeRenderer = new SpriteRenderer(Shader);

    Shader Shader = ResourceManager::GetShader("grid");
    GridRenderer = new SpriteRenderer(Shader);

    // Declaring grid texture
    gridTexture = new Texture2D(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);


    this->grid.resize(this->Width * this->Height);
    this->pixel_buffer.resize(this->Width * this->Height);

    std::fill(this->pixel_buffer.begin(), this->pixel_buffer.end(), 0x00000000);
    gridTexture->Generate(Width, Height, this->pixel_buffer.data());


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

        this->grid[static_cast<int>(this->MouseY * this->MouseX) + static_cast<int>(this->MouseX)] = 1;
        

        for(int i = 0; i < grid.size(); i++) {
            switch (grid[i]) {
                case 1: this->pixel_buffer[i] = 0xFFFFFFFF;break;
            }
        }

        gridTexture->Update(this->pixel_buffer.data()); 
    }


}


