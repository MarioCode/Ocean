#ifndef PREDATORS_H
#define PREDATORS_H
#include "fish.h"

class Predators: public Fish
{
private:
    int CountAte;
    int CountTime;
public:
    Predators():Fish(), CountAte(0), CountTime(0) {}
    Predators(int _posX, int _posY):Fish( _posX, _posY, true), CountAte(0), CountTime(0) {}

    void SetCountAte(int _addAte);
    int& GetCountAte();
    int& GetCountTime();

    ~Predators();
};

#endif // PREDATORS_H
