#include "fish.h"

bool Fish::CheckIsLive() const
{
       return isLive;
}

bool Fish::GetCurPoint() const
{
    return true;
}

int Fish::GetX() const
{
    return pos_x;
}

int Fish::GetY() const
{
    return pos_y;
}

void Fish::SetCoord(int new_X, int new_Y)
{
    pos_x = new_X;
    pos_y = new_Y;
}

void Fish::SetX(int new_X)
{
    pos_x = new_X;
}

void Fish::SetY(int new_Y)
{
    pos_y = new_Y;
}

void Fish::SetNewLife(bool new_life)
{
    isLive = new_life;
}

Fish::~Fish()
{

}

