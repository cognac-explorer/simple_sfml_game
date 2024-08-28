#include "Vec2.h"
#include <iostream>
#include "GameEngine.h"


int main()
{
    
    GameEngine game = GameEngine("../bin/assets.txt");
    game.run();

    return 0;
}