/*
    Texture.hpp
    Declan Hopkins
    10/18/2015
*/

#pragma once

#include "PCH.hpp"

class Game;

class Texture {
    public:
        Texture(Game* game);
        ~Texture();

        void SetX(int x);
        void SetY(int y);
        void SetWidth(int width);
        void SetHeight(int height);

        int GetX();
        int GetY();
        int GetWidth();
        int GetHeight();

        void CreateTextureFromString(const std::string& text, const std::string& fontPath, SDL_Color color, int size);
        void Render();

    private:
        Game* m_game;
        SDL_Texture* m_texture;
        int m_x;
        int m_y;
        int m_width;
        int m_height;
};
