#include "element.h"
#include "../game.h"

void SandUpdate(Game& game, int i)
{
    if (!game.Down(i))
        if (!game.DownLeft(i))
            game.DownRight(i);
}