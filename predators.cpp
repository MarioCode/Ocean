#include "predators.h"

void Predators::SetCountAte(int _addAte)
{
    CountAte +=_addAte;
}

int Predators::GetCountAte() const
{
    return CountAte;
}

int& Predators::GetCountTime()
{
    return CountTime;
}


Predators::~Predators()
{

}

