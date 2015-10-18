/*
    Game.cpp
    Declan Hopkins
    10/15/2015
*/

#include "PCH.hpp"
#include "Game.hpp"

Game::Game() :
    m_isRunning(true),
    m_windowTitle("Snake"),
    m_windowWidth(640),
    m_windowHeight(480),
    m_cellSize(16),
    m_window(nullptr),
    m_renderer(nullptr),
    m_scoreText(nullptr),
    m_highScoreText(nullptr),
    m_gameOverText(nullptr),
    m_highScoreLabelText(nullptr),
    m_scoreLabelText(nullptr),
    m_newHighScoreLabelText(nullptr),
    m_tryAgainLabelText(nullptr),
    m_eatSound(nullptr),
    m_dieSound(nullptr),
    m_score(0),
    m_highScore(0),
    m_state(PLAY),
    m_snake(nullptr),
    m_food(nullptr)
{

}

SDL_Renderer* Game::GetRenderer()
{
    return m_renderer;
}

Mix_Chunk* Game::GetEatSound()
{
    return m_eatSound;
}

Mix_Chunk* Game::GetDieSound()
{
    return m_dieSound;
}

int Game::GetScore()
{
    return m_score;
}

int Game::GetWindowWidth()
{
    return m_windowWidth;
}

int Game::GetWindowHeight()
{
    return m_windowHeight;
}

int Game::GetCellSize()
{
    return m_cellSize;
}

Food* Game::GetFood()
{
    return m_food.get();
}

void Game::SetScore(int score)
{
    m_score = score;
}

void Game::Start()
{
    //TODO Error checking

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL error: " << SDL_GetError() << std::endl;
    }

    // Initialize SDL_ttf
    if(TTF_Init() < 0)
    {
        std::cout << "SDL_ttf could not be initialized! SDL_ttf error: " << TTF_GetError() << std::endl;
    }

    // Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "SDL_mixer could not be initialized! SDL_mixer error: " << Mix_GetError() << std::endl;
    }
    // Create window
    m_window = SDL_CreateWindow(m_windowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN);
    if(m_window == nullptr)
    {
        std::cerr << "Window could not be created! SDL error: " << SDL_GetError() << std::endl;
    }

    // Create renderer
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_renderer == nullptr)
    {
        std::cerr << "Renderer could not be created! SDL error: " << SDL_GetError() << std::endl;
    }

    // Load sound effects
    m_eatSound = Mix_LoadWAV("res/eat.wav");
    if(m_eatSound == nullptr)
    {
        std::cout << "Failed to load eat sound effect! SDL_mixer error: " << Mix_GetError() << std::endl;
    }

    m_dieSound = Mix_LoadWAV("res/die.wav");
    if(m_eatSound == nullptr)
    {
        std::cout << "Failed to load die sound effect! SDL_mixer error: " << Mix_GetError() << std::endl;
    }

    //ResetHighScore();
    NewGame();

    // Game loop
    SDL_Event event;
    while(m_isRunning)
    {
        // Poll for window input
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
            {
                m_isRunning = false;
            }
        }

        Update();
        Render();
    }

    Stop();
}

void Game::Update()
{
    // Get the key states
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    m_snake->Update();

    if(m_state == GAME_OVER)
    {
        if(currentKeyStates[SDL_SCANCODE_R])
        {
            NewGame();
        }
    }
}

void Game::Render()
{
    Uint32 currentTime = SDL_GetTicks();

    // Clear window
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    if(m_state == PLAY)
    {
        m_snake->Render();
        m_food->Render();

        // Render score text
        // Set rendering space and render to screen
        scoreString.str("");
        scoreString << m_score;
        SDL_Color color = {0, 255, 0, 255};
        m_scoreText = CreateText(scoreString.str().c_str(), "res/dos.ttf", color, 22);

        int w, h;
        SDL_QueryTexture(m_scoreText, NULL, NULL, &w, &h);
        SDL_Rect textRect = {(m_windowWidth / 2) - (w / 2), 8, w, h};
        SDL_RenderCopy(m_renderer, m_scoreText, NULL, &textRect);
    }
    else if (m_state == GAME_OVER)
    {
        // TODO Fix this awfulness.
        // Currently this just loads the font every frame and generates
        // textures every frame. This is extremely dumb and inefficient

        // Render game over window
        SDL_Color color = {0, 255, 0, 255};

        m_gameOverText = CreateText("Game Over", "res/dos.ttf", color, 32);
        m_scoreLabelText = CreateText("Score", "res/dos.ttf", color, 22);
        m_highScoreLabelText = CreateText("High Score", "res/dos.ttf", color, 22);
        m_newHighScoreLabelText = CreateText("New High Score!", "res/dos.ttf", color, 40);
        m_tryAgainLabelText = CreateText("Press 'R' to play again!", "res/dos.ttf", color, 18);

        scoreString.str("");
        scoreString << m_score;
        m_scoreText = CreateText(scoreString.str().c_str(), "res/dos.ttf", color, 40);

        scoreString.str("");
        scoreString << m_highScore;
        m_highScoreText = CreateText(scoreString.str().c_str(), "res/dos.ttf", color, 40);

        int w, h;

        // Game over text
        SDL_QueryTexture(m_gameOverText, NULL, NULL, &w, &h);
        SDL_Rect textRect = {(m_windowWidth / 2) - (w / 2), 8, w, h};
        SDL_RenderCopy(m_renderer, m_gameOverText, NULL, &textRect);

        // New high score label text
        // TODO Fix this awful hacky flashing code
        static Uint32 lastFlash = 0;
        static bool flash = false;
        if(m_score > m_highScore)
        {
            if((currentTime - lastFlash) > 500)
            {
                flash = !flash;
                lastFlash = SDL_GetTicks();
            }

            if(flash)
            {
                SDL_QueryTexture(m_newHighScoreLabelText, NULL, NULL, &w, &h);
                textRect = {(m_windowWidth / 2) - (w / 2), (m_windowHeight / 2), w, h};
                SDL_RenderCopy(m_renderer, m_newHighScoreLabelText, NULL, &textRect);
            }
        }
        else
        {
            // High score label text
            SDL_QueryTexture(m_highScoreLabelText, NULL, NULL, &w, &h);
            textRect = {(m_windowWidth / 2) - (w / 2), (m_windowHeight / 2), w, h};
            SDL_RenderCopy(m_renderer, m_highScoreLabelText, NULL, &textRect);

            // High Score text
            SDL_QueryTexture(m_highScoreText, NULL, NULL, &w, &h);
            textRect = {(m_windowWidth / 2) - (w / 2), (m_windowHeight / 2) + (h), w, h};
            SDL_RenderCopy(m_renderer, m_highScoreText, NULL, &textRect);
        }

        // Score label text
        SDL_QueryTexture(m_scoreLabelText, NULL, NULL, &w, &h);
        textRect = {(m_windowWidth / 2) - (w / 2), (m_windowHeight / 2) - (h * 5), w, h};
        SDL_RenderCopy(m_renderer, m_scoreLabelText, NULL, &textRect);

        // Score text
        SDL_QueryTexture(m_scoreText, NULL, NULL, &w, &h);
        textRect = {(m_windowWidth / 2) - (w / 2), (m_windowHeight / 2) - (h * 2), w, h};
        SDL_RenderCopy(m_renderer, m_scoreText, NULL, &textRect);

        // Try again label
        SDL_QueryTexture(m_tryAgainLabelText, NULL, NULL, &w, &h);
        textRect = {(m_windowWidth / 2) - (w / 2), (m_windowHeight - h) - (h/2), w, h};
        SDL_RenderCopy(m_renderer, m_tryAgainLabelText, NULL, &textRect);
    }

    // Temp fix for memory leak
    SDL_DestroyTexture(m_scoreText);
    m_scoreText = nullptr;
    SDL_DestroyTexture(m_highScoreText);
    m_highScoreText = nullptr;
    SDL_DestroyTexture(m_scoreLabelText);
    m_scoreLabelText = nullptr;
    SDL_DestroyTexture(m_highScoreLabelText);
    m_highScoreLabelText = nullptr;
    SDL_DestroyTexture(m_gameOverText);
    m_gameOverText = nullptr;
    SDL_DestroyTexture(m_newHighScoreLabelText);
    m_newHighScoreLabelText = nullptr;
    SDL_DestroyTexture(m_tryAgainLabelText);
    m_tryAgainLabelText = nullptr;

    // Update window
    SDL_RenderPresent(m_renderer);
}

void Game::Stop()
{
    // Free textures
    SDL_DestroyTexture(m_scoreText);
    m_scoreText = nullptr;
    SDL_DestroyTexture(m_highScoreText);
    m_highScoreText = nullptr;
    SDL_DestroyTexture(m_scoreLabelText);
    m_scoreLabelText = nullptr;
    SDL_DestroyTexture(m_highScoreLabelText);
    m_highScoreLabelText = nullptr;
    SDL_DestroyTexture(m_gameOverText);
    m_gameOverText = nullptr;
    SDL_DestroyTexture(m_newHighScoreLabelText);
    m_newHighScoreLabelText = nullptr;
    SDL_DestroyTexture(m_tryAgainLabelText);
    m_tryAgainLabelText = nullptr;

    // Free sounds
    Mix_FreeChunk(m_eatSound);
    m_eatSound = nullptr;
    Mix_FreeChunk(m_dieSound);
    m_dieSound = nullptr;

    // Free SDL resources
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    m_renderer = nullptr;

    SDL_Quit();
    TTF_Quit();
    Mix_Quit();
}

void Game::GameOver()
{
    // If we have a new high score, save it.
    if(m_score > m_highScore)
    {
        SaveHighScore();
    }

    m_state = GAME_OVER;
}

SDL_Texture* Game::CreateText(const std::string& message, const std::string& path, SDL_Color color, int size)
{
    SDL_Texture* texture;

    // Open the font
    TTF_Font* font = TTF_OpenFont(path.c_str(), size);
    if(font == nullptr)
    {
        std::cout << "Failed to load font! SDL_ttf error: " << TTF_GetError() << std::endl;
    }

    // Render the text onto a surface, then load that surface into a texture.
    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    if(surface == nullptr)
    {
        TTF_CloseFont(font);
        std::cout << "Failed to render font to surface! SDL_ttf error: " << TTF_GetError() << std::endl;
    }

    texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if(texture == nullptr)
    {
        std::cout << "Failed to create texture from surface! SDL error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

    return texture;
}

void Game::LoadHighScore()
{
    // TODO Make safe. Currently it will just accept ANY value in that file as a high score
    std::ifstream scoreFile("res/highscore.txt");
    if(scoreFile.is_open())
    {
        scoreFile >> m_highScore;
        scoreFile.close();
    }
    else
    {
        std::cout << "Unable to open highscores.txt!" << std::endl;
    }
}

void Game::SaveHighScore()
{
    std::ofstream scoreFile("res/highscore.txt");
    if(scoreFile.is_open())
    {
        scoreFile << m_score << "\n";
        scoreFile.close();
    }
    else
    {
        std::cout << "Unable to open highscores.txt!" << std::endl;
    }
}

void Game::ResetHighScore()
{
    m_score = 0;
    SaveHighScore();
}

void Game::NewGame()
{
    m_snake = std::make_unique<Snake>(this);
    m_food = std::make_unique<Food>(this);
    m_score = 0;

    // Load the high score
    LoadHighScore();

    m_state = PLAY;
}
