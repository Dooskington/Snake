/*
    Snake.cpp
    Declan Hopkins
    10/17/2015
*/

#include "PCH.hpp"
#include "Game.hpp"
#include "Snake.hpp"

// TODO The isAlive bool is probably redundant

Snake::Snake(Game* game) :
    m_game(game),
    m_x(0),
    m_y(0),
    m_speed(3),
    m_isAlive(true),
    m_lastMoveTime(0),
    m_snakeUpdateTime(300),
    m_dir(NONE)
{
    // Grow the initial snake
    Grow(3);
}

Snake::~Snake()
{
    m_snakeSegments.clear();
}

void Snake::SetX(int x)
{
    m_x = x;
}

void Snake::SetY(int y)
{
    m_y = y;
}

int Snake::GetX()
{
    return m_x;
}

int Snake::GetY()
{
    return m_y;
}

void Snake::Update()
{
    if(m_isAlive)
    {
        // Get the key states
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        // bool to only allow one change in direction per "turn"
        static bool canMove = true;
        if(canMove)
        {
            if(currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_UP])
            {
                if(m_dir != SOUTH)
                {
                    m_dir = NORTH;
                    canMove = false;
                }
            }
            else if(currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_LEFT])
            {
                if(m_dir != EAST)
                {
                    m_dir = WEST;
                    canMove = false;
                }
            }
            else if(currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_DOWN])
            {
                if(m_dir != NORTH)
                {
                    m_dir = SOUTH;
                    canMove = false;
                }
            }
            else if(currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_RIGHT])
            {
                if(m_dir != WEST)
                {
                    m_dir = EAST;
                    canMove = false;
                }
            }
        }

        // Move snake at an interval
        Uint32 currentTime = SDL_GetTicks();
        if((currentTime - m_lastMoveTime) > m_snakeUpdateTime / m_speed)
        {
            // Make each snake segment take the position of the one in front of it
            for(size_t i = m_snakeSegments.size() - 1; i > 0; i--)
            {
                m_snakeSegments[i]->SetX(m_snakeSegments[i - 1]->GetX());
                m_snakeSegments[i]->SetY(m_snakeSegments[i - 1]->GetY());
            }

            switch (m_dir)
            {
                case NORTH:
                    m_snakeSegments[0]->SetY(m_snakeSegments[0]->GetY() - 1);
                    break;

                case SOUTH:
                    m_snakeSegments[0]->SetY(m_snakeSegments[0]->GetY() + 1);
                    break;

                case EAST:
                    m_snakeSegments[0]->SetX(m_snakeSegments[0]->GetX() + 1);
                    break;

                case WEST:
                    m_snakeSegments[0]->SetX(m_snakeSegments[0]->GetX() - 1);
                    break;

                case NONE:
                    break;

                default:
                    break;
            }

            // If we are moving, check for collision
            if(m_dir != NONE)
            {
                CheckCollision();
            }

            m_lastMoveTime = SDL_GetTicks();
            canMove = true;
        }
    }
}

void Snake::Render()
{
    if(m_isAlive)
    {
        for(std::vector< std::unique_ptr<SnakeSegment> >::const_iterator it = m_snakeSegments.begin(); it != m_snakeSegments.end(); it++)
        {
            SDL_Rect snakeRect = {(*it)->GetX() * m_game->GetCellSize(), (*it)->GetY() * m_game->GetCellSize(), m_game->GetCellSize(), m_game->GetCellSize()};
            SDL_SetRenderDrawColor(m_game->GetRenderer(), 0, 255, 0, 255);
            SDL_RenderFillRect(m_game->GetRenderer(), &snakeRect);
        }
    }
}

void Snake::Grow(int amount)
{
    for(int i = 0; i < amount; i++)
    {
        // Allocate a new snake segment and add it to the end of the snake
        std::unique_ptr<SnakeSegment> seg = std::make_unique<SnakeSegment>();
        if(m_snakeSegments.size() > 0)
        {
            // Start it at the position of the head
            seg->SetX(m_snakeSegments[0]->GetX());
            seg->SetY(m_snakeSegments[0]->GetY());
        }
        else
        {
            // This is the head; start it at the center.
            seg->SetX((m_game->GetWindowWidth() / m_game->GetCellSize()) / 2);
            seg->SetY((m_game->GetWindowHeight() / m_game->GetCellSize()) / 2);
        }
        m_snakeSegments.push_back(std::move(seg));
    }
}

void Snake::CheckCollision()
{
    // Check and see if the head ran into any body parts
    std::vector< std::unique_ptr<SnakeSegment> >::const_iterator head = m_snakeSegments.begin();
    for(std::vector< std::unique_ptr<SnakeSegment> >::const_iterator it = head + 1; it != m_snakeSegments.end(); it++)
    {
        // If the body part is inside the head
        if((*it)->GetX() == (*head)->GetX() && (*it)->GetY() == (*head)->GetY())
        {
            // The snake ran into itself. Game over.
            Kill();
        }
    }

    // If the head runs into the the edges
    if((*head)->GetX() < 0 || (*head)->GetX() >= (m_game->GetWindowWidth() / m_game->GetCellSize()))
    {
        // The snake ran into the wall. Game over.
        Kill();
    }
    else if ((*head)->GetY() < 0 || (*head)->GetY() >= (m_game->GetWindowHeight() / m_game->GetCellSize()))
    {
        // The snake ran into the wall. Game over.
        Kill();
    }

    //If the head runs into any food
    if((*head)->GetX() == m_game->GetFood()->GetX() && (*head)->GetY() == m_game->GetFood()->GetY())
    {
        EatFood();
    }
}

void Snake::EatFood()
{
    Mix_PlayChannel( -1, m_game->GetEatSound(), 0 );

    m_game->SetScore(m_game->GetScore() + 100);
    m_game->GetFood()->Reset();
    Grow(2);
    m_speed++;
}

void Snake::Kill()
{
    Mix_PlayChannel( -1, m_game->GetDieSound(), 0 );
    m_isAlive = false;
    m_game->GameOver();
}
