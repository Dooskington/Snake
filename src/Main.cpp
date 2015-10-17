/*
    Main.cpp
    Declan Hopkins
    10/15/2015
*/

#include "Game.hpp"

int main(int argc, char** argv)
{
    std::srand(std::time(NULL));

    Game game;
    game.Start();

    return 0;
}
