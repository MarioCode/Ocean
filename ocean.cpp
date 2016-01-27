#include "ocean.h"
#include "ui_ocean.h"
#include <QMessageBox>
#include <sys/timeb.h>

struct _timeb timebuffer1;

Ocean::Ocean(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ocean)
{
    ui->setupUi(this);
    timer = new QTimer;
    SizeField=ui->SizeFieldBox->value();

    for (int i = 0;i<100;i++)
        massPointX.push_back(i);

    ocean = new Drawing_Scene(this, SizeField);
    area = new AREA * [400];
    for (int i = 0; i < 400; i++)
        area[i] = new AREA [400];

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(Generation()));
    connect(ui->generationButton, SIGNAL(clicked()), this, SLOT(StartInit()));
    connect(ocean, SIGNAL(SendCoordMous(int, int, int)), this, SLOT(GetCoordMouse(int, int, int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(Generation()));
    connect(ui->SizeFieldBox, SIGNAL(valueChanged(int)), ocean, SLOT(ChangeFieldSize(int)));

    ui->main_layout->setStretchFactor(ui->OceanScene, 8);
    ui->main_layout->setStretchFactor(ui->ConfigLaout, 2);
    ui->startButton->setEnabled(false);
    ui->OceanScene->addWidget(ocean);
    ocean->RecieveInit(area);
}

void Ocean::DrawGraph(int Pr, int Vic)
{
    massPointVictims.push_back(Pr);
    massPointPredators.push_back(Vic);

    if (massPointVictims.count() > massPointX.count())
        massPointVictims.removeAt(0);
    if (massPointPredators.count() > massPointX.count())
        massPointPredators.removeAt(0);

    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->setPen(QPen(Qt::red));
    ui->widget->graph(0)->setData(massPointX, massPointVictims);
    ui->widget->xAxis->setRange(0, 100);//Для оси Ox
    ui->widget->yAxis->setRange(0, SizeField*SizeField);//Для оси Oy

    ui->widget->addGraph();
    ui->widget->graph(1)->setData(massPointX, massPointPredators);
    ui->widget->xAxis->setRange(0, 100);//Для оси Ox
    ui->widget->yAxis->setRange(0, SizeField*SizeField);//Для оси Oy

    ui->widget->replot();
}

void Ocean::StartInit()
{
    // Добавление хищников в вектор

    int CountCell=SizeField*SizeField;
    if(CountCell - ui->AmountStones->value() - ui->Amount_Predators->value() - ui->Amount_Victims->value() < 0)
    {
        QMessageBox::information(this, tr("Ошибка!"), tr("Количество существ превышает размеры поля.\nВведите корректные данные."));
        return;
    }

    predators.erase(predators.begin(), predators.end());
    victims.erase(victims.begin(), victims.end());

    for (int i = 0; i < SizeField; i++)
        for (int j = 0; j < SizeField; j++)
            area[i][j].who=0;


    for(int i=0; i<ui->AmountStones->value(); i++)
    {
        int posX = qrand()%SizeField;
        int posY = qrand()%SizeField;

        if(area[posX][posY].who)
        {
            i--;
            continue;
        }
        area[posX][posY].who = 3;
    }

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
    qDebug() <<predators.size() << victims.size();

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

    ui->countPredators->setText(QString::number(predators.size()));
    ui->countVictims->setText(QString::number(victims.size()));
    ui->startButton->setEnabled(true);

    ocean->RecieveInit(area);
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
        else if(predators[i].GetCountAte()>=ui->razmnozh_Predators->value()) //очень сытый
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

        if(isVic==false && cord.x==-1)
            continue;

        area[predators[i].GetX()][predators[i].GetY()].who=0;//там где были не занято
        area[cord.x][cord.y].who=1;//куда идём - хищник

        if(isVic)
        {
            //            int vic;
            //            for(vic=0; vic<victims.size(); vic++)
            //            {
            //                if(victims[vic] == Victims(cord.x, cord.y))
            //                    break;
            //            }
            //            victims.erase(victims.begin() + vic);//удаляем жертву
            victims.erase(std::find(victims.begin(), victims.end(), Victims(cord.x, cord.y)));
            predators[i].GetCountAte()++; //поели
            predators[i].GetCountTime()=0;
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

        if(cord.x==-1)
            continue;

        area[victims[i].GetX()][victims[i].GetY()].who=0;
        area[cord.x][cord.y].who=2;

        //area[cord.x][cord.y].iter=victims.begin()+i;

        victims[i].SetX(cord.x);
        victims[i].SetY(cord.y);
        victims[i].GetCountTime()++;
    }

    ui->countPredators->setText(QString::number(predators.size()));
    ui->countVictims->setText(QString::number(victims.size()));
    DrawGraph(predators.size(), victims.size());
    ocean->RecieveInit(area);
}


Point Ocean::RandomCellAr1(Point p, int who)
{
    QVector<Point> cord;

    for(int k=-1; k<2; k++)
        for(int l=-1; l<2; l++)
        {
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

void Ocean::GetCoordMouse(int butclick, int x, int y)
{

    if(!timer->isActive())
        SettForMouse();

    if (area[x][y].who == 2 || area[x][y].who == 1)
        return;

    // Если нажата ПКМ - добавляем хищника
    if (butclick==0)
    {
        predators.push_back(Predators(x,y));
        ui->countPredators->setText(QString::number(predators.size()));
        area[x][y].who = 1;
    }
    // Если нажата(зажата) ЛКМ - добавляем камни (стены)
    else if (butclick==1)
        area[x][y].who = 3;

    // Если нажато колесико - добавляем хищника
    else if (butclick==2)
    {
        victims.push_back(Victims(x,y));
        ui->countVictims->setText(QString::number(victims.size()));
        area[x][y].who = 2;
    }

    ocean->RecieveInit(area);

    ui->countPredators->setText(QString::number(predators.size()));
    ui->countVictims->setText(QString::number(victims.size()));

}

void Ocean::on_startButton_clicked()
{
    timer->start(ui->UpTimeBox->value());

    ui->AmountStones->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->clearButton->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->generationButton->setEnabled(false);
    ui->SizeFieldBox->setEnabled(false);
    ui->Amount_Predators->setEnabled(false);
    ui->Amount_Victims->setEnabled(false);
}

void Ocean::SettForMouse()
{
    ui->startButton->setEnabled(true);
    ui->clearButton->setEnabled(false);
    ui->stopButton->setEnabled(false);
    ui->generationButton->setEnabled(false);
    ui->SizeFieldBox->setEnabled(false);
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
    ui->comboBox->setCurrentIndex(0);
    ui->AmountStones->setEnabled(true);
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

    massPointVictims.erase(massPointVictims.begin(), massPointVictims.end());
    massPointPredators.erase(massPointPredators.begin(), massPointPredators.end());
    DrawGraph(predators.size(), victims.size());
    ocean->RecieveInit(area);
}

void Ocean::on_SizeFieldBox_valueChanged(int arg1)
{
    SizeField = arg1;
}

void Ocean::on_UpTimeBox_valueChanged(int arg1)
{
    timer->setInterval(arg1);
}

void Ocean::on_comboBox_currentIndexChanged(int index)
{

    if(index && index!=3)
    {
        ui->AmountStones->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->clearButton->setEnabled(false);
        ui->startButton->setEnabled(true);
        ui->generationButton->setEnabled(false);
        ui->SizeFieldBox->setEnabled(false);
        ui->Amount_Predators->setEnabled(false);
        ui->Amount_Victims->setEnabled(false);
        ui->deadStep_Predators->setEnabled(false);
        ui->razmnozh_Predators->setEnabled(false);
        ui->razmnozh_Victims->setEnabled(false);
    }

    switch(index)
    {
    case 1:
    {
        HEADMAP LoadMap;

        QFile file_load("labyrint.MAPAS");

        if(file_load.open(QIODevice::ReadOnly))
        {
            file_load.read((char*)&LoadMap, sizeof(LoadMap));

            SizeField = LoadMap.mSize;
            ocean->ChangeFieldSize(SizeField);
            ui->razmnozh_Predators->setValue(LoadMap.mPredBorn);
            ui->razmnozh_Victims->setValue(LoadMap.mVictBorn);
            ui->deadStep_Predators->setValue(LoadMap.mPredDead);
            ui->Amount_Predators->setValue(LoadMap.mPredCount);
            ui->Amount_Victims->setValue(LoadMap.mVictCount);
            ui->countPredators->setText(QString::number(LoadMap.mPredCount));
            ui->countVictims->setText(QString::number(LoadMap.mVictCount));
            ui->SizeFieldBox->setValue(SizeField);
            ui->AmountStones->setValue(LoadMap.mStoneCount);

            for(int i=0; i<SizeField; i++)
                for(int j=0; j<SizeField; j++){
                    file_load.read((char*)&area[i][j].who, 1);

                    if(area[i][j].who==1)
                        predators.push_back(Predators(i,j));
                    if(area[i][j].who==2)
                        victims.push_back(Victims(i,j));
                }
            file_load.close();
        }

        ocean->RecieveInit(area);
        break;
    }
    case 2:


        break;
    case 3:
        ui->comboBox->setCurrentIndex(0);
        break;

    case 4:
    {
        HEADMAP SaveMap;

        SaveMap.mSize=SizeField;
        SaveMap.mPredBorn=ui->razmnozh_Predators->value();
        SaveMap.mVictBorn=ui->razmnozh_Victims->value();
        SaveMap.mPredDead=ui->deadStep_Predators->value();
        SaveMap.mPredCount=predators.size();
        SaveMap.mVictCount=victims.size();

        SaveMap.mStoneCount=0;
        for (int i = 0; i < SizeField; i++)
            for (int j = 0; j < SizeField; j++)
                if (area[i][j].who==3)
                    SaveMap.mStoneCount++;


        QFile file("labyrint.MAPAS");
        if(file.open(QIODevice::WriteOnly))
        {
            file.write((char*)&SaveMap, sizeof(SaveMap));
            for(int i=0; i<SizeField; i++)
                for(int j=0; j<SizeField; j++)
                    file.write((char*)&area[i][j].who, 1);

            file.close();
        }
        else
            QMessageBox::information(this, tr("Ошибка!"), tr("Ошибка при открытии файла."));

        ui->comboBox->setCurrentIndex(0);
        break;
    }
    case 5:
        QString str = QFileDialog::getOpenFileName(0, "Загрузка карты", "", "*.MAPAS");
        ui->comboBox->setCurrentIndex(0);
        break;
    }
}
