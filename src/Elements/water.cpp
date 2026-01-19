#include "../game.h"

void WaterUpdate(Game& game, int i)
{
    if (!game.Down(i))
        if (!game.DownLeft(i))
            if (!game.DownRight(i))
                if (!game.Left(i))
                    game.Right(i);
}