#include "drawing_scene.h"

Drawing_Scene::Drawing_Scene(QWidget *parent) : QWidget(parent)
{
    qDebug() << "Draw Конструктор";
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    //timer = new QTimer;
    SizeField = 100;

    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void Drawing_Scene::startGame()
{
    //emit fieldNotChanged(true);
    //timer->start(150);
}

void Drawing_Scene::stopGame()
{
    //timer->stop();
}


void Drawing_Scene::clear()
{

}

void Drawing_Scene::RecieveInit(AREA **_area)
{

}

void Drawing_Scene::SetInterval(int msec)
{
    timer->setInterval(msec);
}

void Drawing_Scene::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    static bool init_start = true;
    DrawScene(p);
    DrawGrid(p);
    //InitField(p);
    // DrawUpdate(p);
    // DrawGraph(p);
}

void Drawing_Scene::ChangeFieldSize(const int &size)
{
    SizeField = size;
    update();
}

void Drawing_Scene::InitField(QPainter &p)
{
    qDebug() << "Init";
//        for(int Pr=0; Pr<100; Pr++)
//        {
//             double cellWidth = (double)width()/SizeField;
//             double cellHeight = (double)height()/SizeField;

//             p.setBrush(Qt::blue);
//             p.drawEllipse((predators[Pr].GetX())*cellWidth ,(predators[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
//        }

}

void Drawing_Scene::DrawUpdate(QPainter &p)
{
    qDebug() << "Up";
//        for(int Pr=0; Pr<predators.size(); Pr++)
//        {
//            double cellWidth = (double)width()/SizeField;
//            double cellHeight = (double)height()/SizeField;

//            p.setBrush(Qt::red);
//            p.drawEllipse((predators[Pr].GetX())*cellWidth ,(predators[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
//        }


//        for(int Pr=0; Pr<victims.size(); Pr++)
//        {
//            double cellWidth = (double)width()/SizeField;
//            double cellHeight = (double)height()/SizeField;

//            p.setBrush(Qt::blue);
//            p.drawEllipse((victims[Pr].GetX())*cellWidth ,(victims[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
//        }
}


void Drawing_Scene::DrawGrid(QPainter &p)
{
    qDebug() << "Grid";
    QRect borders(0, 0, width()-1, height()-1);
    QColor gridColor = Qt::blue;
    gridColor.setAlpha(15);
    p.setPen(gridColor);

    double cellWidth = (double)width()/SizeField;
    double cellHeight = (double)height()/SizeField;

    for(double i = cellWidth; i <= width(); i += cellWidth)
        p.drawLine(i, 0, i, height());

    for(double i = cellHeight; i <= height(); i += cellHeight)
        p.drawLine(0, i, width(), i);

    p.drawRect(borders);
}

void Drawing_Scene::DrawScene(QPainter &p)
{
    qDebug() << "Scene";
    QColor gridColor = Qt::green;
    gridColor.setAlpha(9);

    double cellWidth = (double)width()/SizeField;
    double cellHeight = (double)height()/SizeField;

    for(int i=1; i <= SizeField; i++) {
        for(int j=1; j <= SizeField; j++) {
            qreal left = (qreal)(cellWidth*j-cellWidth);
            qreal top  = (qreal)(cellHeight*i-cellHeight);
            QRectF r(left, top, (qreal)cellWidth, (qreal)cellHeight);
            p.fillRect(r, gridColor);
        }
    }
}

Drawing_Scene::~Drawing_Scene()
{
}
