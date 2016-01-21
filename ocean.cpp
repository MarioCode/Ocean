#include "ocean.h"
#include "ui_ocean.h"
#include <QDebug>

Ocean::Ocean(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ocean)
{
    ui->setupUi(this);
    ocean = new Drawing_Scene(this);

    connect(ui->startButton, SIGNAL(clicked()), ocean, SLOT(startGame()));
    connect(ui->stopButton, SIGNAL(clicked()), ocean, SLOT(stopGame()));
    connect(ui->clearButton, SIGNAL(clicked()), ocean, SLOT(clear()));

    connect(ui->UpTimeBox, SIGNAL(valueChanged(int)), ocean, SLOT(SetInterval(int)));
    connect(ui->SizeFieldBox, SIGNAL(valueChanged(int)), ocean, SLOT(ChangeFieldSize(int)));
    connect(ocean,SIGNAL(fieldNotChanged(bool)),ui->SizeFieldBox,SLOT(setDisabled(bool)));
    connect(ocean,SIGNAL(Stat(QString, QString)),this, SLOT(SetStat(QString, QString)));

    ui->main_layout->setStretchFactor(ui->OceanScene, 8);
    ui->main_layout->setStretchFactor(ui->ConfigLaout, 2);
    ui->OceanScene->addWidget(ocean);
}

void Ocean::SetStat(QString pr, QString vic)
{
    qDebug() << "Stat";
    ui->countPredators->setText(pr);
    ui->countVictims->setText(vic);
}

Ocean::~Ocean()
{
    delete ui;
}
