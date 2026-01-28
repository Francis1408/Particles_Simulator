#include "game.h"
#include "utils/resourceManager.h"
#include "utils/spriteRenderer.h"
#include "Elements/elements_behaviours.h"

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
SpriteRenderer *BrushRenderer;

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
   delete BrushRenderer;

   delete gridTexture;
   gridTexture = nullptr;
}

void Game::Init(int argc, char* argv[])
{
    // SIMULATOR PRE-PROCESSING CONFIGS
    // ResourceManager::LoadShader("Shaders/basicShader.vs", "Shaders/basicShader.fs", nullptr, "cube");
    ResourceManager::LoadShader("Shaders/quadShader.vs", "Shaders/quadShader.fs", nullptr, "grid");
    ResourceManager::LoadShader("Shaders/shaderText.vs", "Shaders/shaderText.fs", nullptr, "text");
    ResourceManager::LoadShader("Shaders/brushShader.vs", "Shaders/brushShader.fs", nullptr, "brush");

    // Define the View Matrix - Game is oriented from top to bottom
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    // The text Coordinate is defined from bottom to top
    glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(this->Width), 0.0f, static_cast<float>(this->Height));
    
    // ResourceManager::GetShader("cube").Use().SetMat4("projection", projection);
    ResourceManager::GetShader("grid").Use().SetInt("image", 0);
    ResourceManager::GetShader("grid").SetMat4("projection", projection);
    ResourceManager::GetShader("brush").Use().SetMat4("projection", projection);
    ResourceManager::GetShader("text").Use().SetMat4("text", 0);
    ResourceManager::GetShader("text").SetMat4("projection", textProjection);

    // Shader Shader = ResourceManager::GetShader("cube");
    // CubeRenderer = new SpriteRenderer(Shader);

    Shader Shader = ResourceManager::GetShader("grid");
    GridRenderer = new SpriteRenderer(Shader);

    Shader = ResourceManager::GetShader("brush");
    BrushRenderer = new SpriteRenderer(Shader);

    pixel_size = 4;
    brushSize = 1;
    brushOriginX = 0.0f;
    brushOriginY = 0.0f;

    gridCols = Width/pixel_size;
    gridRows =  Height/pixel_size;

    // Declaring grid texture
    gridTexture = new Texture2D(GL_R8, GL_RED, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);

    // Resize grid based on the pixel size
    this->grid.resize(gridCols * gridRows, EMPTY);
    // Resize flags
    this->visited.resize(gridCols * gridRows, false);

    // Resize Velocity
    this->velocityY.resize(gridCols * gridRows, 0.0f);
    this->velocityX.resize(gridCols * gridRows, 0.0f);

   
    gridTexture->Generate(gridCols, gridRows, nullptr);


    // Starts with no element selected
    this->currentElement = EMPTY;


    // Define the elements properties

    elements[EMPTY] = {
    .density = 0.0f,
    .solid = false,
    .update = nullptr
    };

    elements[SAND] = {
        .density = 2.0f,
        .solid = true,
        .update = SandUpdate
    };

    elements[WATER] = {
        .density = 1.0f,
        .solid = false,
        .update = WaterUpdate
    };

    elements[WALL] = {
        .density = 100.0f,
        .solid = true,
        .update = nullptr
    };


    glm::vec4 palette[256] = {};
    palette[EMPTY] = glm::vec4{0,0,0,1};
    palette[SAND]  = glm::vec4{1,1,0,1};
    palette[WATER] = glm::vec4{0,0,1,1};
    palette[WALL] = glm::vec4{0.5,0.5,0.5,1};



    ResourceManager::GetShader("grid").Use().SetVec4("palette", palette, 256);

}

void Game::StepSimulation(float dt) {

    accumulator += dt * simulationSpeed;

    // When it reaches the cap time
    while (accumulator >= simulationStep) {

        Simulator(); // Updates the grid once
        accumulator -= simulationStep;
    }
}

void Game::ProcessInput(float dt)
{

    if (this->MouseKeys[GLFW_MOUSE_BUTTON_LEFT]) {
        // glm::vec2 drawCoord = glm::vec2(this->MouseX, this->MouseY);

        int x = (int) this->brushOriginX;
        int y = (int) this->brushOriginY;

        // Re-scale for the grid cell

        int startingCellX = x / pixel_size;
        int startingCellY = y / pixel_size;

        for(int i = 0; i < brushSize; i++) {
            for(int j = 0; j < brushSize; j++) {

                // Generates randomly
                if (this->currentElement != WALL) {

                    double random_val = static_cast<double>(std::rand()) / RAND_MAX;
                    std::cout << random_val << std::endl;
                    if (random_val > 0.25) continue;
                }

                int cellX = startingCellX + j;
                int cellY = startingCellY + i;

                // Check if it is not out of bounds
                if (cellX >= 0 && cellX < gridCols &&
                    cellY >= 0 && cellY < gridRows) {
        
                    // Exact cell (cube) which the mouse is on
                    int cellIndex = cellY * gridCols + cellX;
                    this->grid[cellIndex] = this->currentElement;
                }

            }
        }
      
    }
    
    if (this->Keys[GLFW_KEY_1]) {
        this->currentElement = SAND;
        std::cout << "SAND SELECTED" << std::endl;
    }

    if (this->Keys[GLFW_KEY_2]) {
    this->currentElement = WATER;
    std::cout << "WATER SELECTED" << std::endl;
    }

    if (this->Keys[GLFW_KEY_3]) {
    this->currentElement = WALL;
    std::cout << "WALL SELECTED" << std::endl;
    }

    if (Keys[GLFW_KEY_UP]) {
        simulationSpeed *= 1.02f;
        simulationSpeed = glm::clamp(simulationSpeed, 0.0f, 10.0f);
        std::cout << "AUMENTOU" << std::endl;

    }

    if (Keys[GLFW_KEY_DOWN]) {
        simulationSpeed *= 0.98f;
        simulationSpeed = glm::clamp(simulationSpeed, 0.0f, 10.0f);
        std::cout << "DIMINUIU" << std::endl; 
        
    }

    if (this->Keys[GLFW_KEY_MINUS]) {
        
        brushSize--;
        brushSize = glm::clamp(brushSize, 1, 10);
    }

    if (this->Keys[GLFW_KEY_EQUAL]) {
        
        brushSize++;
        brushSize = glm::clamp(brushSize, 1, 10);
    }

}

void Game::Render()
{   
    
    gridTexture->Update(this->grid.data()); 
    GridRenderer->DrawSprite(*gridTexture, glm::vec2(0.0f, 0.0f), glm::vec2(gridCols * pixel_size, gridRows * pixel_size));
    BrushRenderer->DrawSprite(glm::vec2(brushOriginX, brushOriginY), glm::vec2(static_cast<float>(pixel_size * brushSize) , static_cast<float>(pixel_size * brushSize)));

}

void Game::UpdateDrawingArea() {

    brushOriginX = MouseX - (static_cast<float>(pixel_size) * brushSize * 0.5f);
    brushOriginY = MouseY - (static_cast<float>(pixel_size) * brushSize * 0.5f);

}



void Game::Simulator() {

    // Iterates through the cell grid and appylies the blocks interactions

    for (int y = gridRows - 2; y >= 0; y--) {
        for (int x = 0; x < gridCols; x++) {

            int i = y * gridCols + x;
            
            // Checks if the action was not applied to the cells
            if (!this->visited[i]) {
                
                uint8_t type = grid[i];
                Element& e = elements[type];

                if(e.update) {
                    e.update(*this, i);
                }

            }   
        }
    }
    // Clear visited cells
     for (int i = 0; i < this->visited.size(); i++) {
        this->visited[i] = false;
    }


}

bool Game::Down(int currentCell) {

    int downCell = currentCell + gridCols;
    // Boundary check
    if (downCell >= 0 && downCell < grid.size()) {

        // Check if grid is not solid
        if (grid[downCell] == EMPTY) {
                
            // Switch positions
            uint8_t aux = grid[downCell];
            grid[downCell] = grid[currentCell];
            grid[currentCell] = aux;
            // Mark as visited
            visited[downCell] = true;
            visited[currentCell] = true;
            
            return true;
            
        }
        return false;      
    }
    return false;
}

bool Game::DownLeft(int currentCell) {

    int leftCell =  currentCell + gridCols- 1;
    
    int currentCellRow = (int) currentCell/gridCols;
    int leftCellRow = (int) leftCell/gridCols;
    // Boundary check
    if (leftCell >= 0 && leftCell < grid.size() &&
    leftCellRow > currentCellRow) { // If they are not on the same row
        
        // Check if space is available
        if (grid[leftCell] == EMPTY) {
           
            // Switch positions
            uint8_t aux = grid[leftCell];
            grid[leftCell] = grid[currentCell];
            grid[currentCell] = aux;
            // Mark as visited
            visited[leftCell] = true;
            visited[currentCell] = true;
            return true;
              
        }
        return false;
    }
    return false;
}

bool Game::DownRight(int currentCell) {

    int rightCell = currentCell + gridCols + 1;

    int currentCellRow = (int) currentCell/gridCols;
    int rightCellRow = (int) rightCell/gridCols;

    // Boundary check
    if(rightCell >= 0 && rightCell < grid.size() &&
    currentCellRow + 1 == rightCellRow) {

        // Check if space is available
        if (grid[rightCell] == EMPTY) {

            // Switch positions
            uint8_t aux = this->grid[rightCell];
            this->grid[rightCell] = this->grid[currentCell];
            this->grid[currentCell] = aux;
            // Mark as visited
            visited[rightCell] = true;
            
            return true;
            
    }
        return false;
    }
    return false;
}

bool Game::Right(int currentCell) {

    int rightCell = currentCell + 1;

    int currentCellRow = (int) currentCell/gridCols;
    int rightCellRow = (int) rightCell/gridCols;

    // Boundary check
    if(rightCell >= 0 && rightCell < grid.size() &&
    currentCellRow == rightCellRow) {

        // Check if space is available
        if (grid[rightCell] == EMPTY) {
                
            // Switch positions
            uint8_t aux = grid[rightCell];
            grid[rightCell] = grid[currentCell];
            grid[currentCell] = aux;
            // Mark as visited
            visited[rightCell] = true;
            visited[currentCell] = true;
            
            return true;
             
        }
        return false;
    }

    return false;
}

bool Game::Left(int currentCell) {

    int leftCell =  currentCell - 1;
    
    int currentCellRow = (int) currentCell/gridCols;
    int leftCellRow = (int) leftCell/gridCols;
    // Boundary check
    if (leftCell >= 0 && leftCell < grid.size() &&
    leftCellRow == currentCellRow) { // If they are not on the same row
        
        // Check if space is available
        if (grid[leftCell] == EMPTY) {
       
            // Switch positions
            uint8_t aux = grid[leftCell];
            grid[leftCell] = grid[currentCell];
            grid[currentCell] = aux;
            // Mark as visited
            visited[leftCell] = true;
            visited[currentCell] = true;
            
            return true;
        
        }
        return false;
    }

    return false;

}

void Game::MoveCell(int from, int to) {

    std::swap(grid[from], grid[to]);
    std::swap(velocityX[from], velocityX[to]);
    std::swap(velocityY[from], velocityY[to]);

}


