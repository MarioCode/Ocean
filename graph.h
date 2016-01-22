#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include <time.h>

class Graph : public QWidget
{
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = 0);
    ~Graph();

public slots:
    void Up(float, float);

protected:
    void paintEvent(QPaintEvent *);

private:
    QList<int> massPointX;
    QList<float> massPointVictims;
    QList<float> massPointPredators;
};

#endif // GRAPH_H
