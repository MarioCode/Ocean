#ifndef FISH_H
#define FISH_H
#include <QDebug>

class Fish
{
protected:
    int pos_x;
    int pos_y;
    bool isLive;

public:
    Fish(): pos_x(0), pos_y(0), isLive(false) {}
    Fish(int _x, int _y, bool _isLive): pos_x(_x), pos_y(_y), isLive(_isLive) {}

    bool CheckIsLive() const;
    bool GetCurPoint() const;
    int GetX() const;
    int GetY() const;
    void SetX(int new_X);
    void SetY(int new_Y);
    void SetCoord(int new_X, int new_Y);
    void SetNewLife(bool new_life);

    ~Fish();
};

#endif // FISH_H
