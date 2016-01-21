#include "victims.h"

int& Victims::GetCountTime()
{
    return CountTime;
}

bool Victims::operator==(const Victims& v)
{
    if(GetX()==v.GetX() && GetY()==v.GetY())
        return true;
    return false;
}

Victims::~Victims()
{

}

