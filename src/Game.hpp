/*
    Game.hpp
    Declan Hopkins
    10/15/2015
*/

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "SDL2/SDL.h"
#include "SnakeSegment.hpp"
#include "Food.hpp"

class Game
{
    public:
        Game();

        void Start();
        void Update();
        void Render();
        void Stop();

        void Grow();
        void UpdateSnake();
        void CheckCollision();

        enum Direction {NONE, NORTH, SOUTH, EAST, WEST};

    private:
        bool m_isRunning;
        std::string m_windowTitle;
        int m_windowWidth;
        int m_windowHeight;
        const int m_cellSize;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;

        // Snake
        std::vector< std::unique_ptr<SnakeSegment> > m_snakeSegments;
        Uint32 m_lastMoveTime;
        Direction m_dir;
};
