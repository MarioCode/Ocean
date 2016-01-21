#ifndef DRAWING_SCENE_H
#define DRAWING_SCENE_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QVector>
#include <QLabel>
#include <QMouseEvent>

#include "predators.h"
#include "victims.h"

struct AREA{
    unsigned char who;
    AREA():who(0){}
};

class Drawing_Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Drawing_Scene(QWidget *parent = 0);
    ~Drawing_Scene();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

signals:
    void fieldNotChanged(bool ok);
    void SendCoordMous(int, int, int);

private slots:

    void DrawGrid(QPainter &p);
    void InitField(QPainter &p);

public slots:
    void ChangeFieldSize(const int &size);
    void SetInterval(int);

    void RecieveInit(AREA **);

private:
    int SizeField;
    AREA **Field;
    QTimer *timer;
};

#endif // DRAWING_SCENE_H
