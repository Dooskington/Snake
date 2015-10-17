/*
    Game.hpp
    Declan Hopkins
    10/15/2015
*/

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
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
        void ResetFood();

        SDL_Texture* RenderText(const std::string& message, const std::string& path, SDL_Color color, int size);

        enum Direction {NONE, NORTH, SOUTH, EAST, WEST};

    private:
        bool m_isRunning;
        std::string m_windowTitle;
        int m_windowWidth;
        int m_windowHeight;
        const int m_cellSize;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        SDL_Texture* m_score;

        // Snake
        std::vector< std::unique_ptr<SnakeSegment> > m_snakeSegments;
        int m_snakeSpeed; // The speed modifier
        const int m_snakeUpdateTime; // The frequency, in milliseconds, to update the snake
        Uint32 m_lastMoveTime;
        Direction m_dir;

        // Food
        std::unique_ptr<Food> m_food;
};
