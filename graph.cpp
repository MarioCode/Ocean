#include "graph.h"
#include <QDebug>

Graph::Graph(QWidget *parent) : QWidget(parent)
{

    for (int i = 0;i<100;i++)
    {
        massPointX << 15+((width()/30) * i);
    }
}

void Graph::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QRect background(0, 0, width()-1, height()-1);
    painter.setBrush(QColor("#fcffe0"));
    painter.drawRect(background);
    painter.setBrush(Qt::NoBrush);

    QPen pen;
    QColor color;

    pen.setWidth(1);
    color = Qt::black;
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawLine(15,height()/2,width(),height()/2);
    painter.drawLine(15,0,15,height());
    painter.drawLine(11,6,15,0);
    painter.drawLine(19,6,15,0);

    painter.drawLine(width(),height()/2,width()-8,height()/2-8);
    painter.drawLine(width(),height()/2,width()-8,height()/2+8);
    painter.drawText(3,13,"N");
    painter.drawText(width()-8,height()/2+20,"T");

    for (int i = 0;i<massPointX.count();i++)
    {
        if (massPointVictims.count() <= i || massPointPredators.count() <= i)
            break;

        if (i != 0)
        {
            pen.setWidth(1);

            color = Qt::blue;
            pen.setColor(color);
            painter.setPen(pen);
            painter.drawLine(massPointX[i-1],massPointVictims[i-1]+height()/2,massPointX[i],massPointVictims[i]+height()/2);

            color = Qt::red;
            pen.setColor(color);
            painter.setPen(pen);
            painter.drawLine(massPointX[i-1],massPointPredators[i-1]+height()/2,massPointX[i],massPointPredators[i]+height()/2);

        }
    }
}

void Graph::Up(float Victims, float Predators)
{
    massPointVictims << -1*Victims;
    massPointPredators << -1*Predators;

    if (massPointVictims.count() > massPointX.count())
        massPointVictims.removeAt(0);
    if (massPointPredators.count() > massPointX.count())
        massPointPredators.removeAt(0);
    update();
}

Graph::~Graph()
{

}

