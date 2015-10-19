/*
    Texture.cpp
    Declan Hopkins
    10/18/2015
*/

#include "Texture.hpp"
#include "Game.hpp"

Texture::Texture(Game* game) :
    m_game(game),
    m_texture(nullptr),
    m_x(0),
    m_y(0),
    m_width(0),
    m_height(0)
{

}

Texture::~Texture()
{
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
    m_game = nullptr;
}

void Texture::SetX(int x)
{
    m_x = x;
}

void Texture::SetY(int y)
{
    m_y = y;
}

void Texture::SetWidth(int width)
{
    m_width = width;
}

void Texture::SetHeight(int height)
{
    m_height = height;
}

int Texture::GetX()
{
    return m_x;
}

int Texture::GetY()
{
    return m_y;
}

int Texture::GetWidth()
{
    return m_width;
}

int Texture::GetHeight()
{
    return m_height;
}

void Texture::CreateTextureFromString(const std::string& text, const std::string& fontPath, SDL_Color color, int size)
{
    // Open the font
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
    if(font == nullptr)
    {
        std::cout << "Failed to load font! SDL_ttf error: " << TTF_GetError() << std::endl;
    }

    // Render the text onto a surface, then load that surface into a texture.
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if(surface == nullptr)
    {
        std::cout << "Failed to render font to surface! SDL_ttf error: " << TTF_GetError() << std::endl;
    }

    m_texture = SDL_CreateTextureFromSurface(m_game->GetRenderer(), surface);
    SDL_QueryTexture(m_texture, NULL, NULL, &m_width, &m_height);
    if(m_texture == nullptr)
    {
        std::cout << "Failed to create texture from surface! SDL error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
}

void Texture::Render()
{
    SDL_Rect rect = {m_x, m_y, m_width, m_height};
    SDL_RenderCopy(m_game->GetRenderer(), m_texture, NULL, &rect);
}
