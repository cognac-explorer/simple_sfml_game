#include "Vec2.h"
#include <iostream>
#include "GameEngine.h"


int main()
{
    
    GameEngine game = GameEngine("assets.txt");
    game.run();

    return 0;
}