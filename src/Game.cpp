/*
    Game.cpp
    Declan Hopkins
    10/15/2015
*/

// TODO PCH
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
    m_score(0),
    m_snakeSpeed(1),
    m_snakeUpdateTime(500)
{

}

void Game::Start()
{
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL error: " << SDL_GetError() << std::endl;
    }

    // Initialize SDL_ttf
    if(TTF_Init() < 0)
    {
        std::cout << "SDL_ttf could not be initialized! SDL_ttf error: " << TTF_GetError() << std::endl;
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

    // Create the snake
    int startLength = 3;
    for(int i = 0; i < startLength; i++)
    {
        Grow();
    }

    // Create the food
    m_food = std::make_unique<Food>();
    ResetFood();

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
    // Snake input
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if(currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP])
    {
        if(m_dir != SOUTH)
        {
            m_dir = NORTH;
        }
    }
    else if(currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT])
    {
        if(m_dir != EAST)
        {
            m_dir = WEST;
        }
    }
    else if(currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN])
    {
        if(m_dir != NORTH)
        {
            m_dir = SOUTH;
        }
    }
    else if(currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT])
    {
        if(m_dir != WEST)
        {
            m_dir = EAST;
        }
    }

    // Move snake at an interval
    Uint32 currentTime = SDL_GetTicks();
    if((currentTime - m_lastMoveTime) > m_snakeUpdateTime / m_snakeSpeed)
    {
        UpdateSnake();

        switch (m_dir)
        {
            case NORTH:
                m_snakeSegments[0]->m_y -= 1;
                break;

            case SOUTH:
                m_snakeSegments[0]->m_y += 1;
                break;

            case EAST:
                m_snakeSegments[0]->m_x += 1;
                break;

            case WEST:
                m_snakeSegments[0]->m_x -= 1;
                break;

            case NONE:
                break;

            default:
                break;
        }

        // If we are moving
        if(m_dir != NONE)
        {
            CheckCollision();
        }

        m_lastMoveTime = SDL_GetTicks();
    }

}

void Game::Render()
{
    // Clear window
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // Render food
    SDL_Rect foodRect = {m_food->m_x * m_cellSize + 4, m_food->m_y * m_cellSize + 4, 8, 8};
    SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(m_renderer, &foodRect);

    // Render all snake segments
    for(std::vector< std::unique_ptr<SnakeSegment> >::const_iterator it = m_snakeSegments.begin(); it != m_snakeSegments.end(); it++)
    {
        SDL_Rect snakeRect = {(*it)->m_x * m_cellSize, (*it)->m_y * m_cellSize, 16, 16};
        SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(m_renderer, &snakeRect);
    }

    // Render score text
    // Set rendering space and render to screen
    scoreString.str("");
    scoreString << std::right << m_score;
    SDL_Color color = { 0, 255, 0, 255 };
    m_scoreText = CreateText(scoreString.str().c_str(), "res/dos.ttf", color, 22);

    int w, h;
    SDL_QueryTexture(m_scoreText, NULL, NULL, &w, &h);
    SDL_Rect textRect = {(m_windowWidth / 2) - (w / 2), 8, w, h};
    SDL_RenderCopy(m_renderer, m_scoreText, NULL, &textRect);

    // Update window
    SDL_RenderPresent(m_renderer);
}

void Game::Stop()
{
    // Free snake
    m_snakeSegments.clear();

    // Free textures
    SDL_DestroyTexture(m_scoreText);
    m_scoreText = nullptr;

    // Free SDL resources
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_window = nullptr;
    m_renderer = nullptr;

    SDL_Quit();
}

void Game::UpdateSnake()
{
    // Make each snake segment take the position of the one in front of it
    for(size_t i = m_snakeSegments.size() - 1; i > 0; i--)
    {
        m_snakeSegments[i]->m_x = m_snakeSegments[i - 1]->m_x;
        m_snakeSegments[i]->m_y = m_snakeSegments[i - 1]->m_y;
    }
}

void Game::CheckCollision()
{
    // Check and see if the head ran into any body parts
    std::vector< std::unique_ptr<SnakeSegment> >::const_iterator head = m_snakeSegments.begin();
    for(std::vector< std::unique_ptr<SnakeSegment> >::const_iterator it = head + 1; it != m_snakeSegments.end(); it++)
    {
        // If the body part is inside the head
        if((*it)->m_x == (*head)->m_x && (*it)->m_y == (*head)->m_y)
        {
            // The snake ran into itself. Game over.
            m_isRunning = false;
        }
    }

    // If the head runs into the the edges
    if((*head)->m_x <= 0 || (*head)->m_x >= (m_windowWidth / m_cellSize))
    {
        // The snake ran into the wall. Game over.
        m_isRunning = false;
    }
    else if ((*head)->m_y <= 0 || (*head)->m_y >= (m_windowHeight / m_cellSize))
    {
        // The snake ran into the wall. Game over.
        m_isRunning = false;
    }

    // If the head runs into any food
    if((*head)->m_x == m_food->m_x && (*head)->m_y == m_food->m_y)
    {
        EatFood();
    }
}

void Game::EatFood()
{
    m_score += 100;
    ResetFood();
    Grow();
}

void Game::ResetFood()
{
    // Puts the food at a random position anywhere on the map, with 1 cell padding around the borders
    m_food->m_x = std::rand() % ((m_windowWidth / m_cellSize) - 2) + 1;
    m_food->m_y = std::rand() % ((m_windowHeight / m_cellSize) - 2) + 1;
}

void Game::Grow()
{
    // Allocate a new snake segment and add it to the end of the snake
    std::unique_ptr<SnakeSegment> seg(new SnakeSegment());
    if(m_snakeSegments.size() > 0)
    {
        // Start it at the position of the head
        seg->m_x = m_snakeSegments[0]->m_x;
        seg->m_y = m_snakeSegments[0]->m_y;
    }
    else
    {
        // This is the head; start it at the center.
        seg->m_x = (m_windowWidth / m_cellSize) / 2;
        seg->m_y = (m_windowHeight / m_cellSize) / 2;
    }
    m_snakeSegments.push_back(std::move(seg));

    // Speed up the snake
    m_snakeSpeed++;
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
