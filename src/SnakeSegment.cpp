/*
    SnakeSegment.cpp
    Declan Hopkins
    10/16/2015
*/
#include "PCH.hpp"
#include "SnakeSegment.hpp"

SnakeSegment::SnakeSegment() :
    m_x(0),
    m_y(0)
{

}

void SnakeSegment::SetX(int x)
{
    m_x = x;
}

void SnakeSegment::SetY(int y)
{
    m_y = y;
}

int SnakeSegment::GetX()
{
    return m_x;
}

int SnakeSegment::GetY()
{
    return m_y;
}
