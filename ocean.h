#ifndef OCEAN_H
#define OCEAN_H

#include <QMainWindow>
#include "drawing_scene.h"

namespace Ui {
class Ocean;
}

class Ocean : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ocean(QWidget *parent = 0);
    ~Ocean();

private:
    Ui::Ocean *ui;
    Drawing_Scene *ocean;
};

#endif // OCEAN_H
