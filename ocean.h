#ifndef OCEAN_H
#define OCEAN_H

#include <QMainWindow>
#include <QDebug>
#include <QVectorIterator>
#include <algorithm>

#include "drawing_scene.h"
#include "graph.h"

namespace Ui {
class Ocean;
}

struct Point
{
    int x,y;
    Point(int _x=0, int _y=0):x(_x), y(_y){}
};


class Ocean : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ocean(QWidget *parent = 0);
    ~Ocean();

private slots:
    void Generation();
    void StartInit();
    Point RandomCellAr1(Point p, int who);
    void GetCoordMouse(int, int, int);
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void SettForMouse();
    void on_clearButton_clicked();

private:
    Ui::Ocean *ui;
    Drawing_Scene *ocean;
    Graph *graph;

    int SizeField;
    QTimer *timer;
    QVector<Predators> predators;
    QVector<Victims> victims;
    AREA **area; //0 - не занято, 1 - занято хищником, 2 - занято жертвой, 3 - камушек бля
};

#endif // OCEAN_H
