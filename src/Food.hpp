/*
    Food.hpp
    Declan Hopkins
    10/16/2015
*/

#pragma once
#include "PCH.hpp"

class Game;

class Food
{
    public:
        Food(Game* game);
        ~Food();

        void SetX(int x);
        void SetY(int y);

        int GetX();
        int GetY();

        void Update();
        void Render();
        void Reset();

    private:
        Game* m_game;
        int m_x;
        int m_y;
};
