#include "drawing_scene.h"

struct Point
{
    int x,y;
    Point(int _x=0, int _y=0):x(_x), y(_y){}
};

Drawing_Scene::Drawing_Scene(QWidget *parent) : QWidget(parent)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    timer = new QTimer;

    SizeField = 100;

    area = new unsigned char * [SizeField];
    for (int i = 0; i < SizeField; i++)
        area[i] = new unsigned char [SizeField];

    for(int i=0; i<SizeField; i++)
        for(int j=0; j<SizeField; j++)
            area[i][j] = 0;

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
    // Добавление хищников в вектор
    // Начальное значение брать из ползунка в формы

    for(int i=0; i<2; i++)
    {
        int posX = qrand()%SizeField;
        int posY = qrand()%SizeField;
        if(area[posX][posY])
        {
            i--;
            continue;
        }
        predators.push_back(Predators(posX,posY));
        area[posX][posY] = true;
    }

     // Добавление жертв в вектор

    for(int i=0; i<2; i++)
    {
        int posX = qrand()%SizeField;
        int posY = qrand()%SizeField;
        if(area[posX][posY])
        {
            i--;
            continue;
        }
        victims.push_back(Victims(posX, posY));
        area[posX][posY] = true;
    }

    // Начальная отрисовка
    // Оставить для кнопки генерации поля

    //    for(int Pr=0; Pr<100; Pr++)
    //    {
    //         double cellWidth = (double)width()/SizeField;
    //         double cellHeight = (double)height()/SizeField;

    //         p.setBrush(Qt::blue);
    //         p.drawEllipse((predators[Pr].GetX())*cellWidth ,(predators[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
    //    }
}

void Drawing_Scene::Generation(QPainter &p)
{
    for(int i=0; i<predators.size(); i++)
        {
            if(predators[i].GetCountTime()==5)
            {
                predators.erase(predators.begin()+ i);
                i--;
                continue;
            }

            //заполнить вектор свободных клетов вокруг
            //из количества свободных клеток выбрать рандомное число
            //перейти в клетку под рандомным числом
            //пометить предыдущую клетку как свободную
            //в перейдённую клетку пометить как занятую)))

            QVector<Point> cord;
            for(int k=-1; k<2; k++)
                for(int l=-1; l<2; l++)
                {
                    if(k!=0 && l!=0)
                        if(predators[i].GetX()+k < SizeField  && predators[i].GetY()+l < SizeField
                                && predators[i].GetX()+k>=0 && predators[i].GetY()+l>=0)
                        {
                            if(area[predators[i].GetX()+k][predators[i].GetY()+l]==0)
                            {
                                cord.push_back(Point(predators[i].GetX()+k, predators[i].GetY()+l));
                            }
                        }
                }

            if(cord.size()==0) continue;

            int rand = qrand()%cord.size();


            area[predators[i].GetX()][predators[i].GetY()]=0;
            area[cord[rand].x][cord[rand].y]=1;
            predators[i].SetX(cord[rand].x);
            predators[i].SetY(cord[rand].y);
            predators[i].GetCountTime()++;
        }


        for(int Pr=0; Pr<predators.size(); Pr++)
        {
            double cellWidth = (double)width()/SizeField;
            double cellHeight = (double)height()/SizeField;

            p.setBrush(Qt::red);
            p.drawEllipse((predators[Pr].GetX())*cellWidth ,(predators[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
        }


        //ЖЕРТВЫ

        for(int i=0; i<victims.size(); i++)
        {


            if(victims[i].GetCountTime()==5)
            {
                //размножся
                QVector<Point> cord;

                for(int k=-1; k<2; k++)
                    for(int l=-1; l<2; l++)
                    {
                        if(k!=0 && l!=0)
                            if(victims[i].GetX()+k < SizeField  && victims[i].GetY()+l < SizeField
                                    && victims[i].GetX()+k>=0 && victims[i].GetY()+l>=0)
                            {
                                if(!area[victims[i].GetX()+k][victims[i].GetY()+l])
                                {
                                    cord.push_back(Point(victims[i].GetX()+k, victims[i].GetY()+l));
                                }
                            }
                    }

                if(cord.size()!=0)
                {
                    int rand = qrand()%cord.size();
                    //victims[i].GetCountTime()=0;
                    victims.push_back(Victims(cord[rand].x, cord[rand].y));
                }
            }
            //заполнить вектор свободных клетов вокруг
            //из количества свободных клеток выбрать рандомное число
            //перейти в клетку под рандомным числом
            //пометить предыдущую клетку как свободную
            //в перейдённую клетку пометить как занятую)))


            QVector<Point> cord;

            for(int k=-1; k<2; k++)
                for(int l=-1; l<2; l++)
                {
                    if(k!=0 && l!=0)
                        if(victims[i].GetX()+k < SizeField  && victims[i].GetY()+l < SizeField
                                && victims[i].GetX()+k>=0 && victims[i].GetY()+l>=0)
                        {
                            if(!area[victims[i].GetX()+k][victims[i].GetY()+l])
                            {
                                cord.push_back(Point(victims[i].GetX()+k, victims[i].GetY()+l));
                            }
                        }
                }

            if(cord.size()==0) continue;

            int rand = qrand()%cord.size();

            area[victims[i].GetX()][victims[i].GetY()]=0;
            area[cord[rand].x][cord[rand].y]=2;
            victims[i].SetX(cord[rand].x);
            victims[i].SetY(cord[rand].y);
            victims[i].GetCountTime()++;
        }
        for(int Pr=0; Pr<victims.size(); Pr++)
        {
            double cellWidth = (double)width()/SizeField;
            double cellHeight = (double)height()/SizeField;

            p.setBrush(Qt::blue);
            p.drawEllipse((victims[Pr].GetX())*cellWidth ,(victims[Pr].GetY())*cellHeight, (qreal)cellWidth,(qreal)cellHeight);
        }
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
