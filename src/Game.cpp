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
    m_scoreLabelText(nullptr),
    m_highScoreText(nullptr),
    m_highScoreLabelText(nullptr),
    m_newHighScoreLabelText(nullptr),
    m_gameOverText(nullptr),
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

    // Create GUI objects
    SDL_Color colorGreen = {0, 255, 0, 255};
    m_scoreLabelText = std::make_unique<Texture>(this);
    m_scoreLabelText->CreateTextureFromString("Score", "res/dos.ttf", colorGreen, 22);
    m_scoreLabelText->SetX((m_windowWidth / 2) - (m_scoreLabelText->GetWidth() / 2));
    m_scoreLabelText->SetY(128);
    m_highScoreLabelText = std::make_unique<Texture>(this);
    m_highScoreLabelText->CreateTextureFromString("High score", "res/dos.ttf", colorGreen, 22);
    m_highScoreLabelText->SetX((m_windowWidth / 2) - (m_highScoreLabelText->GetWidth() / 2));
    m_highScoreLabelText->SetY(200);
    m_newHighScoreLabelText = std::make_unique<Texture>(this);
    m_newHighScoreLabelText->CreateTextureFromString("New high score!", "res/dos.ttf", colorGreen, 40);
    m_newHighScoreLabelText->SetX((m_windowWidth / 2) - (m_newHighScoreLabelText->GetWidth() / 2));
    m_newHighScoreLabelText->SetY(228);
    m_gameOverText = std::make_unique<Texture>(this);
    m_gameOverText->CreateTextureFromString("Game over", "res/dos.ttf", colorGreen, 32);
    m_gameOverText->SetX((m_windowWidth / 2) - (m_gameOverText->GetWidth() / 2));
    m_gameOverText->SetY(8);
    m_tryAgainLabelText = std::make_unique<Texture>(this);
    m_tryAgainLabelText->CreateTextureFromString("Press 'R' to play again!", "res/dos.ttf", colorGreen, 18);
    m_tryAgainLabelText->SetX((m_windowWidth / 2) - (m_tryAgainLabelText->GetWidth() / 2));
    m_tryAgainLabelText->SetY((m_windowHeight - m_tryAgainLabelText->GetHeight()) - (m_tryAgainLabelText->GetHeight() / 2));

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

    SDL_Color colorGreen = {0, 255, 0, 255};

    if(m_state == PLAY)
    {
        // Update game elements
        m_snake->Update();

        // Update GUI
        scoreString.str("");
        scoreString << m_score;
        m_scoreText->CreateTextureFromString(scoreString.str().c_str(), "res/dos.ttf", colorGreen, 22);
        m_scoreText->SetX((m_windowWidth / 2) - (m_scoreText->GetWidth() / 2));
        m_scoreText->SetY(8);
    }
    else if(m_state == GAME_OVER)
    {
        // Update GUI
        m_scoreText->SetX((m_windowWidth / 2) - (m_scoreText->GetWidth() / 2));
        m_scoreText->SetY(156);

        scoreString.str("");
        scoreString << m_highScore;
        m_highScoreText->CreateTextureFromString(scoreString.str().c_str(), "res/dos.ttf", colorGreen, 22);
        m_highScoreText->SetX((m_windowWidth / 2) - (m_highScoreText->GetWidth() / 2));
        m_highScoreText->SetY(228);

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
        // Render game elements
        m_snake->Render();
        m_food->Render();

        // Render GUI
        m_scoreText->Render();
    }
    else if (m_state == GAME_OVER)
    {
        // TODO Fix this awfulness.
        // Currently this just loads the font every frame and generates
        // textures every frame. This is extremely dumb and inefficient
        m_scoreText->Render();
        m_scoreLabelText->Render();
        m_gameOverText->Render();
        m_tryAgainLabelText->Render();

        // TODO refactor this hacky code
        // Flash the "New high score!" text
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
                m_newHighScoreLabelText->Render();
            }
        }
        else
        {
            m_highScoreText->Render();
            m_highScoreLabelText->Render();
        }
    }

    // Update window
    SDL_RenderPresent(m_renderer);
}

void Game::Stop()
{
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
    m_scoreText = std::make_unique<Texture>(this);
    m_highScoreText = std::make_unique<Texture>(this);
    m_score = 0;

    LoadHighScore();

    m_state = PLAY;
}
