#include "element.h"
#include "../game.h"
#include "../../glm/glm.hpp"

bool SandDown(Game& game, int cell);
bool SandDownRight(Game& game, int cell);
bool SandDownLeft(Game& game, int cell);


void SandUpdate(Game& game, int i)
{   

    // Gets the current speed
    game.velocityY[i] += game.gravity;
    game.velocityY[i] = glm::clamp(game.velocityY[i], 0.0f, game.maxFallSpeed);

    // Gets the final distance to that speed
    float remainingCells = game.velocityY[i];


    // Check every downcell uintil hits on something
    while(remainingCells > 0.0f) {
        if (SandDown(game, i)) {
            i += game.gridCols;
            remainingCells -= 1.0f;
        }
        else if (SandDownLeft(game, i)) {
            i += game.gridCols - 1;
            remainingCells -= 1.0f;
        }
        else if (SandDownRight(game, i)) {
            i += game.gridCols + 1;
            remainingCells -= 1.0f;
        } else {
            game.velocityY[i] = 0.0f;
            break;
        }

    }
 
}

bool SandDown(Game& game, int i) {

    int downCell = i + game.gridCols;
    // Boundary check
    if (downCell >= 0 && downCell < game.grid.size()) {

        // Check if grid is not solid
        if (game.grid[downCell] == EMPTY) {
            
            // Transfer attributes
            game.MoveCell(i, downCell);

            // Mark as visited
            game.visited[downCell] = true;
            game.visited[i] = true;
            
            return true;
            
        }
        return false;      
    }
    return false;
}

bool SandDownLeft(Game& game, int i) {

    int leftCell = i + game.gridCols- 1;
    
    int iRow = (int) i/game.gridCols;
    int leftCellRow = (int) leftCell/game.gridCols;
    // Boundary check
    if (leftCell >= 0 && leftCell < game.grid.size() &&
    leftCellRow > iRow) { // If they are not on the same row
        
        // Check if space is available
        if (game.grid[leftCell] == EMPTY) {
           
            // Transfer attributes
            game.MoveCell(i, leftCell);

            // Mark as visited
            game.visited[leftCell] = true;
            game.visited[i] = true;
            return true;
              
        }
        return false;
    }
    return false;
}

bool SandDownRight(Game& game, int i) {

    int rightCell = i + game.gridCols + 1;

    int currentCellRow = (int) i/game.gridCols;
    int rightCellRow = (int) rightCell/game.gridCols;

    // Boundary check
    if(rightCell >= 0 && rightCell < game.grid.size() &&
    currentCellRow + 1 == rightCellRow) {

        // Check if space is available
        if (game.grid[rightCell] == EMPTY) {

            // Transfer attributes
            game.MoveCell(i, rightCell);
            
            // Mark as visited
            game.visited[rightCell] = true;
            game.visited[i] = true;
            
            return true;
            
    }
        return false;
    }
    return false;
}