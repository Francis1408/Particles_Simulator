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
#include <cmath>

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

    pixel_size = 8;

    gridCols = Width/pixel_size;
    gridRows =  Height/pixel_size;

    // Declaring grid texture
    gridTexture = new Texture2D(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);


    this->grid.resize(gridCols * gridRows, 0);
    this->pixel_buffer.resize(this->Width * this->Height * 4); // 4 Chanels (RGBA) -> 8 bits each

    std::fill(this->pixel_buffer.begin(), this->pixel_buffer.end(), 0);
    // Make alpha visible
    for (int i = 0; i < Width * Height; i++)
    {
        pixel_buffer[i * 4 + 3] = 255; // Alpha
    }

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
        // glm::vec2 drawCoord = glm::vec2(this->MouseX, this->MouseY);

        int x = (int) this->MouseX;
        int y = (int) this->MouseY;

        // Check if it is not out of bounds
        if (x >=  0 && x < Width && y >= 0 && y < Height) {
 
            // Re-scale for the grid cell
            int cellX = x / pixel_size;
            int cellY = y / pixel_size;

            // Check if it is not out of bounds
            if (cellX >= 0 && cellX < gridCols &&
                cellY >= 0 && cellY < gridRows) {
   
                // Exact cell (cube) which the mouse is on
                int cellIndex = cellY * gridCols + cellX;
                this->grid[cellIndex] = 1;

                // Get the top-left pixel_buffer coordinate
                int startX = cellX * pixel_size;
                int startY = cellY * pixel_size;
                
                for (int dy = 0; dy < pixel_size; dy++) {
                    for (int dx = 0; dx < pixel_size; dx++) {

                        int px = startX + dx;
                        int py = startY + dy;
                        
                        // Fills the pixels that belong to the grid
                        // Horizontally(right) to vertically(down)
                        int pixelIndex = (py * Width + px) * 4;

                        pixel_buffer[pixelIndex + 0] = 255;
                        pixel_buffer[pixelIndex + 1] = 255;
                        pixel_buffer[pixelIndex + 2] = 255;
                        pixel_buffer[pixelIndex + 3] = 255;
                    }
                }
            }
        }
        
        gridTexture->Update(this->pixel_buffer.data()); 
    
    }

    GridRenderer->DrawSprite(*gridTexture, glm::vec2(0.0f, 0.0f), glm::vec2(Width, Height));

}

void Game::Simulator() {

    // Iterates through the cell grid and appylies the blocks interactions

    for (int i = 0; i < this->grid.size(); i++) {

        // Checks if the action was not applied to the cell
        bool visited = ((this->grid[i] >> 7) & 0);

        if (!visited) {

            if(!this->Down(i))
                if(!this->DownLeft(i))
                     this->DownRight(i);

        }
        
    }

}

bool Game::Down(int currentCell) {

    int downCell = currentCell + gridRows;
    // Boundary check
    if (downCell >= 0 && downCell < grid.size()) {

        // Check if space is available
        if (this->grid[downCell] != 1) {
            this->grid[downCell] = 1;
            // Mark as visited
            this->grid[downCell] |= (1 << 7);
            return true;
        }
        return false;      
    }
    return false;
}

bool Game::DownLeft(int currentCell) {

    int leftCell =  currentCell + gridRows - 1;
    
    int currentCellRow = (int) currentCell/gridRows;
    int leftCellRow = (int) leftCell/gridRows;
    // Boundary check
    if (leftCell >= 0 && leftCell < grid.size() &&
    leftCellRow > currentCellRow) { // If they are not on the same row
        
        // Check if space is available
        if (this->grid[leftCell] != 1) {
            this->grid[leftCell] = 1;
            // Mark as visited
            this->grid[leftCell] |= (1 << 7);

            return true;    
        }
        return false;
    }

    return false;
}

bool Game::DownRight(int currentCell) {

    int rightCell = currentCell + gridRows + 1;

    int currentCellRow = (int) currentCell/gridRows;
    int rightCellRow = (int) rightCell/gridRows;

    // Boundary check
    if(rightCell >= 0 && rightCell < grid.size() &&
    currentCellRow + 1 == rightCellRow) {

        // Check if space is available
        if (this->grid[rightCell] != 1) {
            this->grid[rightCell] = 1;
            // Mark as visited
            this->grid[rightCell] |= (1 << 7);

            return true;    
        }
        return false;

    }

    return false;
}




