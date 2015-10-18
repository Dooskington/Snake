/*
    Food.cpp
    Declan Hopkins
    10/16/2015
*/

#include "PCH.hpp"
#include "Game.hpp"
#include "Food.hpp"

Food::Food(Game* game) :
    m_game(game),
    m_x(10),
    m_y(10)
{

}

Food::~Food()
{

}

void Food::SetX(int x)
{
    m_x = x;
}

void Food::SetY(int y)
{
    m_y = y;
}

int Food::GetX()
{
    return m_x;
}

int Food::GetY()
{
    return m_y;
}

void Food::Update()
{

}

void Food::Render()
{
    SDL_Rect foodRect = {m_x * m_game->GetCellSize() + 4, m_y * m_game->GetCellSize() + 4, 8, 8};
    SDL_SetRenderDrawColor(m_game->GetRenderer(), 0, 255, 0, 255);
    SDL_RenderFillRect(m_game->GetRenderer(), &foodRect);
}


void Food::Reset()
{
    // Puts the food at a random position anywhere on the map, with 1 cell padding around the borders
    m_x = std::rand() % ((m_game->GetWindowWidth() / m_game->GetCellSize()) - 2) + 1;
    m_y = std::rand() % ((m_game->GetWindowHeight() / m_game->GetCellSize()) - 2) + 1;
}
