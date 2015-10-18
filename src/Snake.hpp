/*
    Snake.hpp
    Declan Hopkins
    10/17/2015
*/

#pragma once

#include "PCH.hpp"
#include "SnakeSegment.hpp"

class Game;

class Snake
{
    public:
        Snake(Game* game);
        ~Snake();

        void SetX(int x);
        void SetY(int y);

        int GetX();
        int GetY();

        void Update();
        void Render();
        void Grow(int amount);
        void CheckCollision();
        void EatFood();
        void Kill();

        enum Direction {NONE, NORTH, SOUTH, EAST, WEST};

    private:
        Game* m_game;
        int m_x;
        int m_y;
        int m_speed;
        bool m_isAlive;
        Uint32 m_lastMoveTime;
        const int m_snakeUpdateTime; // The frequency, in milliseconds, to update the snake
        Direction m_dir;
        std::vector< std::unique_ptr<SnakeSegment> > m_snakeSegments;
};
