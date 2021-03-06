#include "drawing_scene.h"
#include <QPainter>
#include <qmath.h>

Drawing_Scene::Drawing_Scene(QWidget *parent, int Size) : QWidget(parent)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    SizeField = Size;
}

void Drawing_Scene::RecieveInit(AREA **_area)
{
    Field = _area;
    update();
}

void Drawing_Scene::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    DrawGrid(p);
    InitField(p);
}

void Drawing_Scene::ChangeFieldSize(const int &size)
{
    SizeField = size;
    update();
}

void Drawing_Scene::InitField(QPainter &p)
{
    for(int i=0; i<SizeField; i++)
        for(int j=0; j<SizeField; j++)
        {
            double cellWidth = (double)width()/SizeField;
            double cellHeight = (double)height()/SizeField;

            if (Field[i][j].who == 0)
                continue;
            if (Field[i][j].who == 2)
                p.setBrush(Qt::blue);
            else if (Field[i][j].who == 1)
                p.setBrush(Qt::red);
            else
                p.setBrush(Qt::black);

            p.drawEllipse (i*cellWidth ,j*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
        }
}

void Drawing_Scene::DrawGrid(QPainter &p)
{
    QRect background(0, 0, width()-1, height()-1);
    QRect borders(0, 0, width()-1, height()-1);

    p.setBrush(QColor("#faf9fe"));
    p.drawRect(background);
    p.setBrush(Qt::NoBrush);

    QColor gridColor = Qt::blue;
    gridColor.setAlpha(20);
    p.setPen(gridColor);

    double cellWidth = (double)width()/SizeField;
    double cellHeight = (double)height()/SizeField;

    for(double i = cellWidth; i <= width(); i += cellWidth)
        p.drawLine(i, 0, i, height());

    for(double i = cellHeight; i <= height(); i += cellHeight)
        p.drawLine(0, i, width(), i);

    p.drawRect(borders);
}

void Drawing_Scene::mousePressEvent(QMouseEvent *e)
{
    double cellWidth = (double)width()/SizeField;
    double cellHeight = (double)height()/SizeField;

    int k = floor(e->y()/cellHeight);
    int j = floor(e->x()/cellWidth);

    if (k >= 0 && k<SizeField && j >= 0 && j<SizeField)
    {
        if (e->button() == Qt::RightButton)
            emit SendCoordMous(0, j, k);
        else  if (e->button() == Qt::LeftButton)
            emit SendCoordMous(1, j, k);
        else emit SendCoordMous(2, j, k);
    }
}

void Drawing_Scene::mouseMoveEvent(QMouseEvent *e)
{
    double cellWidth = (double)width()/SizeField;
    double cellHeight = (double)height()/SizeField;

    int k = floor(e->y()/cellHeight);
    int j = floor(e->x()/cellWidth);

    if (k >= 0 && k<SizeField && j >= 0 && j<SizeField)
    {
        if (e->button() == Qt::RightButton)
            emit SendCoordMous(0, j, k);
        else
            emit SendCoordMous(1, j, k);
    }
}

Drawing_Scene::~Drawing_Scene()
{
}
