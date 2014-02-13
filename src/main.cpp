#include <iostream>
#include <SFML/Graphics.hpp>
#include "fe/FGame.h"

using namespace std;
using namespace sf;
using namespace fe;

int main(int argc, char** argv)
{
    GameConstants::Init();
    FGame game(GameConstants::WindowSize.x, GameConstants::WindowSize.y);
    game.Run();

    return 0;
}