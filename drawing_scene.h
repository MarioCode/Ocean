#ifndef DRAWING_SCENE_H
#define DRAWING_SCENE_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QVector>

#include "predators.h"
#include "victims.h"

class Drawing_Scene : public QWidget
{
    Q_OBJECT
public:
    explicit Drawing_Scene(QWidget *parent = 0);
    ~Drawing_Scene();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void DrawGrid(QPainter &p);
    void DrawScene(QPainter &p);
    void Generation(QPainter &p);
    void InitField(QPainter &p);
public slots:
    void ChangeFieldSize(const int &size);
    void startGame();
    void stopGame();
    void clear();


private:
    int SizeField;
    int **Field;
    QTimer *timer;
    Predators *predators;
    Victims *victims;
    bool **area;
};

#endif // DRAWING_SCENE_H
