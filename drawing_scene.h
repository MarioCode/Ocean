#ifndef DRAWING_SCENE_H
#define DRAWING_SCENE_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QVector>
#include <QLabel>


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

signals:
    void fieldNotChanged(bool ok);

private slots:

    void DrawScene(QPainter &p);
    void DrawGrid(QPainter &p);
    void DrawUpdate(QPainter &p);
    void InitField(QPainter &p);

public slots:
    void ChangeFieldSize(const int &size);
    void startGame();
    void stopGame();
    void clear();
    void SetInterval(int);

     void RecieveInit(AREA **);

private:
    int SizeField;
    int **Field;
    QTimer *timer;
};

#endif // DRAWING_SCENE_H
