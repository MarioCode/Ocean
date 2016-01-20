#include "drawing_scene.h"

Drawing_Scene::Drawing_Scene(QWidget *parent) : QWidget(parent)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    timer = new QTimer;

    SizeField = 100;
    predators=new Predators[100];
    victims = new Victims[15000];

    area = new bool * [SizeField];
    for (int i = 0; i < SizeField; i++)
       area[i] = new bool [SizeField];

    for(int i=0; i<SizeField; i++)
        for(int j=0; j<SizeField; j++)
            area[i][j] = false;

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void Drawing_Scene::startGame()
{
    timer->start(150);
}

void Drawing_Scene::stopGame()
{
    timer->stop();
}

void Drawing_Scene::clear()
{

}

void Drawing_Scene::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    static bool init_start = true;
    DrawScene(p);
    DrawGrid(p);

    if(init_start)
    InitField(p);
    init_start=false;

    Generation(p);
}

void Drawing_Scene::ChangeFieldSize(const int &size)
{
    SizeField = size;
    update();
}

void Drawing_Scene::InitField(QPainter &p)
{
    qDebug() << "INIT";

    for(int i=0; i<100; i++)
    {
        int posX = qrand()%SizeField;
        int posY = qrand()%SizeField;
        predators[i].SetX(posX);
        predators[i].SetY(posY);
        area[posX][posY] = true;
    }


    for(int Pr=0; Pr<100; Pr++)
    {
         double cellWidth = (double)width()/SizeField;
         double cellHeight = (double)height()/SizeField;

         p.setBrush(Qt::blue);
         p.drawEllipse((predators[Pr].GetX())*cellWidth ,(predators[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
    }
}

void Drawing_Scene::Generation(QPainter &p)
{
    for(int i=0; i<100; i++)
    {
//        int CurPointX=predators[i].GetX();
//        int CurPointY=predators[i].GetY();

//        //Левый верхний угол
//       if (CurPointX==0 && CurPointY == 0) {
//        if(!area[CurPointX+1][CurPointY])
//        {
//            predators[i].SetCoord(CurPointX+1, CurPointY);
//            area[CurPointX+1][CurPointY] = true;
//            area[CurPointX][CurPointY] = false;
//        }
//            else if (!area[CurPointX+1][CurPointY-1])
//        {
//                 predators[i].SetCoord(CurPointX+1, CurPointY-1);
//                 area[CurPointX+1][CurPointY-1] = true;
//                 area[CurPointX][CurPointY] = false;
//        }
//                    else if (!area[CurPointX][CurPointY-1])
//        {
//                       predators[i].SetCoord(CurPointX, CurPointY-1);
//                       area[CurPointX][CurPointY-1] = true;
//                       area[CurPointX][CurPointY] = false;
//        }

//       // Левый нижний угол
//       if (CurPointX==0 && CurPointY == SizeField)
//        if(!area[CurPointX+1][CurPointY])
//        {
//            predators[i].SetCoord(CurPointX+1, CurPointY);
//            area[CurPointX+1][CurPointY] = true;
//            area[CurPointX][CurPointY] = false;
//        }
//            else if (!area[CurPointX+1][CurPointY+1])
//        {
//                 predators[i].SetCoord(CurPointX+1, CurPointY+1);
//                 area[CurPointX+1][CurPointY+1] = true;
//                 area[CurPointX][CurPointY] = false;
//        }
//                    else if (!area[CurPointX][CurPointY+1])
//        {
//                       predators[i].SetCoord(CurPointX, CurPointY+1);
//                       area[CurPointX][CurPointY+1] = true;
//                       area[CurPointX][CurPointY] = false;
//        }

//       // Правый верхний угол
//       if (CurPointX==SizeField && CurPointY == 0)
//        if(!area[CurPointX-1][CurPointY])
//        {
//            predators[i].SetCoord(CurPointX-1, CurPointY);
//            area[CurPointX-1][CurPointY] = true;
//            area[CurPointX][CurPointY] = false;
//        }
//            else if (!area[CurPointX-1][CurPointY-1])
//        {
//                 predators[i].SetCoord(CurPointX-1, CurPointY-1);
//                 area[CurPointX-1][CurPointY-1] = true;
//                 area[CurPointX][CurPointY] = false;
//        }
//                    else if (!area[CurPointX][CurPointY-1])
//        {
//                       predators[i].SetCoord(CurPointX, CurPointY-1);
//                       area[CurPointX][CurPointY-1] = true;
//                       area[CurPointX][CurPointY] = false;
//        }

//       // Правый нижний угол
//       if (CurPointX==SizeField && CurPointY == SizeField)
//        if(!area[CurPointX-1][CurPointY])
//        {
//            predators[i].SetCoord(CurPointX-1, CurPointY);
//            area[CurPointX-1][CurPointY] = true;
//            area[CurPointX][CurPointY] = false;
//        }
//            else if (!area[CurPointX-1][CurPointY+1])
//        {
//                 predators[i].SetCoord(CurPointX-1, CurPointY+1);
//                 area[CurPointX-1][CurPointY+1] = true;
//                 area[CurPointX][CurPointY] = false;
//        }
//                    else if (!area[CurPointX][CurPointY+1])
//        {
//                       predators[i].SetCoord(CurPointX, CurPointY+1);
//                       area[CurPointX][CurPointY+1] = true;
//                       area[CurPointX][CurPointY] = false;
//        }


//       if ((CurPointX > 0 && CurPointX < SizeField-1) && CurPointY == 0)
//       {

//       }

                   predators[i].SetX(predators[i].GetX()-1);
                    predators[i].SetY(predators[i].GetY()-1);

   }


    for(int Pr=0; Pr<100; Pr++)
    {
         double cellWidth = (double)width()/SizeField;
         double cellHeight = (double)height()/SizeField;

         p.setBrush(Qt::blue);
         p.drawEllipse((predators[Pr].GetX())*cellWidth ,(predators[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
    }



    //ЖЕРТВЫ
//    for(int i=0; i<15000; i++)
//    {
//        int posX = qrand()%SizeField;
//        int posY = qrand()%SizeField;
//        if(area[posX][posY]==true)
//        {
//            i--;
//            continue;
//        }
//        victims[i].SetX(posX);
//        victims[i].SetY(posY);
//    }


//    for(int Pr=0; Pr<15000; Pr++)
//    {
//         double cellWidth = (double)width()/SizeField;
//         double cellHeight = (double)height()/SizeField;

//         p.setBrush(Qt::red);
//         p.drawEllipse((victims[Pr].GetX())*cellWidth ,(victims[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
//    }

//    for(int i=0; i<SizeField; i++)
//        for(int j=0; j<SizeField; j++)
//            area[i][j] = false;
}


void Drawing_Scene::DrawGrid(QPainter &p)
{
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
