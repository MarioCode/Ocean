#include "ocean.h"
#include "ui_ocean.h"

Ocean::Ocean(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ocean)
{
    ui->setupUi(this);
    ocean = new Drawing_Scene(this);

    connect(ui->startButton, SIGNAL(clicked()), ocean, SLOT(startGame()));
    connect(ui->stopButton, SIGNAL(clicked()), ocean, SLOT(stopGame()));
    connect(ui->clearButton, SIGNAL(clicked()), ocean, SLOT(clear()));

    connect(ui->SizeFieldBox, SIGNAL(valueChanged(int)), ocean, SLOT(ChangeFieldSize(int)));

    ui->main_layout->setStretchFactor(ui->OceanScene, 8);
    ui->main_layout->setStretchFactor(ui->ConfigLaout, 2);
    ui->OceanScene->addWidget(ocean);
}

Ocean::~Ocean()
{
    delete ui;
}
