#include "ocean.h"
#include "ui_ocean.h"

Ocean::Ocean(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ocean)
{
    ui->setupUi(this);

    ocean = new Drawing_Scene(this);
    timer = new QTimer;

    SizeField=ui->SizeFieldBox->value();
    area = new AREA * [SizeField];
    for (int i = 0; i < SizeField; i++)
        area[i] = new AREA [SizeField];

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(Generation()));
    connect(ui->generationButton, SIGNAL(clicked()), this, SLOT(StartInit()));
    // connect(ui->stopButton, SIGNAL(clicked()), ocean, SLOT(stopGame()));
    // connect(ui->clearButton, SIGNAL(clicked()), ocean, SLOT(clear()));

    connect(timer, SIGNAL(timeout()), this, SLOT(Generation()));

    // connect(ui->UpTimeBox, SIGNAL(valueChanged(int)), ocean, SLOT(SetInterval(int)));
    connect(ui->SizeFieldBox, SIGNAL(valueChanged(int)), ocean, SLOT(ChangeFieldSize(int)));

    ui->main_layout->setStretchFactor(ui->OceanScene, 8);
    ui->main_layout->setStretchFactor(ui->ConfigLaout, 2);
    ui->startButton->setEnabled(false);
    ui->OceanScene->addWidget(ocean);
}

void Ocean::StartInit()
{
    // Добавление хищников в вектор

    predators.erase(predators.begin(), predators.end());
    victims.erase(victims.begin(), victims.end());

    for (int i = 0; i < SizeField; i++)
        for (int j = 0; j < SizeField; j++)
            area[i][j].who=0;

    for(int i=0; i<ui->Amount_Predators->value(); i++)
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

    for(int i=0; i<ui->Amount_Victims->value(); i++)
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

    ocean->RecieveInit(area);
    ui->countPredators->setText(QString::number(predators.size()));
    ui->countVictims->setText(QString::number(victims.size()));
    ui->startButton->setEnabled(true);

}

void Ocean::Generation()
{
    for(int i=0; i<predators.size(); i++)
    {
        if(predators[i].GetCountTime()==ui->deadStep_Predators->value()) //если хищник голодный, умереть
        {
            area[predators[i].GetX()][predators[i].GetY()].who=0;
            predators.erase(predators.begin()+ i);
            i--;
            continue;
        }
        else if(predators[i].GetCountAte()==ui->razmnozh_Predators->value()) //очень сытый
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

    //ЖЕРТВЫ//////////////////////////////////////////////////////

    for(int i=0; i<victims.size(); i++)
    {
        if(victims[i].GetCountTime()==ui->razmnozh_Victims->value())
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

    ui->countPredators->setText(QString::number(predators.size()));
    ui->countVictims->setText(QString::number(victims.size()));
}


Point Ocean::RandomCellAr1(Point p, int who)
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

Ocean::~Ocean()
{
    delete ui;
}

void Ocean::on_startButton_clicked()
{
    timer->start(ui->UpTimeBox->value());

    ui->clearButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->generationButton->setEnabled(false);
    ui->SizeFieldBox->setEnabled(false);
    ui->Amount_Predators->setEnabled(false);
    ui->Amount_Victims->setEnabled(false);
    ui->deadStep_Predators->setEnabled(false);
    ui->razmnozh_Predators->setEnabled(false);
    ui->razmnozh_Victims->setEnabled(false);
}

void Ocean::on_stopButton_clicked()
{
    ui->clearButton->setEnabled(true);
    ui->startButton->setEnabled(true);
    timer->stop();
}

void Ocean::on_clearButton_clicked()
{
    ui->SizeFieldBox->setEnabled(true);
    ui->generationButton->setEnabled(true);

    ui->Amount_Predators->setEnabled(true);
    ui->Amount_Victims->setEnabled(true);
    ui->deadStep_Predators->setEnabled(true);
    ui->razmnozh_Predators->setEnabled(true);
    ui->razmnozh_Victims->setEnabled(true);
    ui->countPredators->setText(QString::number(ui->Amount_Predators->value()));
    ui->countVictims->setText(QString::number(ui->Amount_Victims->value()));

    predators.erase(predators.begin(), predators.end());
    victims.erase(victims.begin(), victims.end());

    for (int i = 0; i < SizeField; i++)
        for (int j = 0; j < SizeField; j++)
            area[i][j].who=0;
}
