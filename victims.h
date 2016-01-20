#ifndef VICTIMS_H
#define VICTIMS_H
#include "fish.h"

class Victims: public Fish
{
private:
    int CountTime;
public:
    Victims():Fish(), CountTime() {}
    Victims(int _posX, int _posY):Fish( _posX, _posY, true), CountTime(0) {}

    int& GetCountTime();

    ~Victims();
};

#endif // VICTIMS_H
