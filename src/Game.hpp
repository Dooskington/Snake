/*
    Game.hpp
    Declan Hopkins
    10/15/2015
*/

#pragma once

#include "PCH.hpp"
#include "Snake.hpp"
#include "SnakeSegment.hpp"
#include "Food.hpp"

class Game
{
    public:
        Game();

        SDL_Renderer* GetRenderer();
        Mix_Chunk* GetEatSound();
        Mix_Chunk* GetDieSound();
        int GetScore();
        int GetWindowWidth();
        int GetWindowHeight();
        int GetCellSize();
        Food* GetFood();

        void SetScore(int score);

        void Start();
        void Update();
        void Render();
        void Stop();
        void GameOver();
        void LoadHighScore();
        void SaveHighScore();
        void ResetHighScore();
        void NewGame();

        SDL_Texture* CreateText(const std::string& message, const std::string& path, SDL_Color color, int size);

        enum GameState {PLAY, GAME_OVER};

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

        // Sounds
        Mix_Chunk* m_eatSound;
        Mix_Chunk* m_dieSound;

        int m_score;
        int m_highScore;
        std::stringstream scoreString;
        GameState m_state;

        // Snake
        std::unique_ptr<Snake> m_snake;

        // Food
        std::unique_ptr<Food> m_food;
};
