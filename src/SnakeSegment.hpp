/*
    SnakeSegment.hpp
    Declan Hopkins
    10/16/2015
*/

#pragma once
#include "PCH.hpp"

class SnakeSegment
{
    public:
        SnakeSegment();

        void SetX(int x);
        void SetY(int y);

        int GetX();
        int GetY();

    private:
        int m_x;
        int m_y;
};
