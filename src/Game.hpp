/*
    Game.hpp
    Declan Hopkins
    10/15/2015
*/

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <time.h>
#include <ctime>
#include <stdlib.h>

#if WINDOWS
    #include <SDL.h>
    #include <SDL_ttf.h>
    #include <SDL_mixer.h>
#else
    #include "SDL2/SDL.h"
    #include "SDL2_ttf/SDL_ttf.h"
    #include "SDL2_mixer/SDL_mixer.h"
#endif

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

        void UpdateSnake();
        void CheckCollision();
        void EatFood();
        void ResetFood();
        void Grow(int amount);
        void GameOver();
        void LoadHighScore();
        void SaveHighScore();
        void Restart();

        SDL_Texture* CreateText(const std::string& message, const std::string& path, SDL_Color color, int size);

        enum Direction {NONE, NORTH, SOUTH, EAST, WEST};

    private:
        bool m_isRunning;
        std::string m_windowTitle;
        int m_windowWidth;
        int m_windowHeight;
        const int m_cellSize;
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        SDL_Texture* m_scoreText;
        SDL_Texture* m_highScoreText;
        SDL_Texture* m_gameOverText;
        SDL_Texture* m_highScoreLabelText;
        SDL_Texture* m_scoreLabelText;
        SDL_Texture* m_newHighScoreLabelText;
        SDL_Texture* m_tryAgainLabelText;
        Mix_Chunk* m_eatSound;
        Mix_Chunk* m_dieSound;
        int m_score;
        int m_highScore;
        std::stringstream scoreString;
        bool m_gameOver;

        // Snake
        std::vector< std::unique_ptr<SnakeSegment> > m_snakeSegments;
        int m_snakeSpeed; // The speed modifier
        const int m_snakeUpdateTime; // The frequency, in milliseconds, to update the snake
        Uint32 m_lastMoveTime;
        Direction m_dir;

        // Food
        std::unique_ptr<Food> m_food;
};
