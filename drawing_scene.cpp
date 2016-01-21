#include "drawing_scene.h"

Drawing_Scene::Drawing_Scene(QWidget *parent) : QWidget(parent)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    timer = new QTimer;


    SizeField = 600;

    area = new AREA * [SizeField];
    for (int i = 0; i < SizeField; i++)
        area[i] = new AREA [SizeField];

//    for (int i = 0;i<10;i++)
//    {
//        massPointX << ((gr->width()/10) * i)+10;
//    }

    connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
}

void Drawing_Scene::startGame()
{
    emit fieldNotChanged(true);
    timer->start(150);
}

void Drawing_Scene::stopGame()
{
    timer->stop();
}

void Drawing_Scene::clear()
{
    predators.erase(predators.begin(), predators.end());
    victims.erase(victims.begin(), victims.end());

    for(int i=0; i<SizeField; i++)
        for(int j=0; j<SizeField; j++)
            area[i][j].who = 0;
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

    if(init_start)
        InitField(p);
    init_start=false;

   Generation(p);
   //DrawGraph(p);
}

//void Drawing_Scene::DrawGraph()
//{
//   QPainter p(this);
//     massPointY << qrand()%100;

//     if (massPointY.count() > massPointX.count())
//         massPointY.removeAt(0);

//     for (int i = 0;i<massPointX.count();i++)
//     {
//         if (massPointY.count() <= i)
//             break;

//         if (i == 0)
//         {
//             QPen pen;
//             pen.setWidth(3);

//             p.setPen(pen);
//             p.drawPoint(massPointX[i],massPointY[i]);
//         }else
//         {
//             QPen pen;
//             pen.setWidth(3);

//             p.setPen(pen);
//             p.drawPoint(massPointX[i],massPointY[i]);


//             pen.setWidth(1);
//             p.setPen(pen);
//             p.drawLine(massPointX[i-1],massPointY[i-1],massPointX[i],massPointY[i]);
//         }
//     }
//}

void Drawing_Scene::ChangeFieldSize(const int &size)
{
    SizeField = size;
    repaint();
}

void Drawing_Scene::InitField(QPainter &p)
{

    for(int i=0; i<10; i++)
    {
        int posX = qrand()%SizeField;
        int posY = qrand()%SizeField;

        if(area[posX][posY].who)
        {
            i--;
            continue;
        }
        predators.push_back(Predators(posX,posY));
        area[posX][posY].who = 1;
    }

    // Добавление жертв в вектор

    for(int i=0; i<1000; i++)
    {
        int posX = qrand()%SizeField;
        int posY = qrand()%SizeField;
        if(area[posX][posY].who)
        {
            i--;
            continue;
        }
        victims.push_back(Victims(posX, posY));
        area[posX][posY].who = 2;
    }

    // Начальная отрисовка
    // Оставить для кнопки генерации поля

//        for(int Pr=0; Pr<100; Pr++)
//        {
//             double cellWidth = (double)width()/SizeField;
//             double cellHeight = (double)height()/SizeField;

//             p.setBrush(Qt::blue);
//             p.drawEllipse((predators[Pr].GetX())*cellWidth ,(predators[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
//        }
}


Point Drawing_Scene::RandomCellAr1(Point p, int who)
 {
    QVector<Point> cord;

        for(int k=-1; k<2; k++)
            for(int l=-1; l<2; l++)
            {
                //if(k!=0 && l!=0)
                    if(p.x+k < SizeField  && p.y+l < SizeField
                            && p.x+k>=0 && p.y+l>=0)
                    {
                        if(area[p.x+k][p.y+l].who==who)
                        {
                            cord.push_back(Point(p.x+k, p.y+l));
                        }
                    }
            }
        if(cord.size()==0) return Point(-1, -1);
        int rand = qrand()%cord.size();
        return cord[rand];
 }

void Drawing_Scene::Generation(QPainter &p)
{
    for(int i=0; i<predators.size(); i++)
        {
            if(predators[i].GetCountTime()==40) //если хищник голодный
            {
                area[predators[i].GetX()][predators[i].GetY()].who=0;
                predators.erase(predators.begin()+ i);
                i--;
                continue;
            }
            else if(predators[i].GetCountAte()==10) //очень сытый
            {
                //размножся
                Point cord = RandomCellAr1(Point(predators[i].GetX(), predators[i].GetY()), 0);

                if(cord.x != -1)
                {
                    predators[i].GetCountAte()=0;
                    predators.push_back(Predators(cord.x, cord.y));
                    area[cord.x][cord.y].who=1;
                }
            }
            //ищем жертву
            Point cord = RandomCellAr1(Point(predators[i].GetX(), predators[i].GetY()), 2);
            bool isVic=true;
            if(cord.x==-1)  //если ничего не нашли - передвинутся в пустую(дописать)
            {
                cord=RandomCellAr1(Point(predators[i].GetX(), predators[i].GetY()), 0);
                isVic=false;
            }

            area[predators[i].GetX()][predators[i].GetY()].who=0;//там где были не занято
            area[cord.x][cord.y].who=1;//куда идём - хищник

            if(isVic)
            {
                int vic;
                for(vic=0; vic<victims.size(); vic++)
                {
                    if(victims[vic] == Victims(cord.x, cord.y))
                        break;
                }
                victims.erase(victims.begin() + vic);//удаляем жертву
                predators[i].GetCountAte()++; //поели
                predators[i].GetCountTime()-=5;
            }
            else
                predators[i].GetCountTime()++;
            predators[i].SetX(cord.x); //переместились
            predators[i].SetY(cord.y);

        }


        for(int Pr=0; Pr<predators.size(); Pr++)
        {
            double cellWidth = (double)width()/SizeField;
            double cellHeight = (double)height()/SizeField;

            p.setBrush(Qt::red);
            p.drawEllipse((predators[Pr].GetX())*cellWidth ,(predators[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
        }


        //ЖЕРТВЫ//////////////////////////////////////////////////////

        for(int i=0; i<victims.size(); i++)
        {

                    if(victims[i].GetCountTime()==10)
                    {
                        //размножся
                        Point cord = RandomCellAr1(Point(victims[i].GetX(), victims[i].GetY()), 0);

                        if(cord.x != -1)
                        {
                            victims[i].GetCountTime()=0;
                            victims.push_back(Victims(cord.x, cord.y));
                            area[cord.x][cord.y].who=2;
                        }
                    }

            Point cord=RandomCellAr1(Point(victims[i].GetX(), victims[i].GetY()), 0);

            if(cord.x==-1) continue;


            area[victims[i].GetX()][victims[i].GetY()].who=0;
            area[cord.x][cord.y].who=2;

            //area[cord.x][cord.y].iter=victims.begin()+i;

            victims[i].SetX(cord.x);
            victims[i].SetY(cord.y);

            victims[i].GetCountTime()++;
        }

        for(int Pr=0; Pr<victims.size(); Pr++)
        {
            double cellWidth = (double)width()/SizeField;
            double cellHeight = (double)height()/SizeField;

            p.setBrush(Qt::blue);
            p.drawEllipse((victims[Pr].GetX())*cellWidth ,(victims[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
        }

        emit Stat(QString::number(predators.size()), QString::number(victims.size()));
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
