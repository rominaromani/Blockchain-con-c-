#ifndef MENUMODIFICAR_H
#define MENUMODIFICAR_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class menuModificar;
}

class menuModificar : public QWidget
{
    Blockchain<Block*>*bc;
    Q_OBJECT

public:
    explicit menuModificar(QWidget *parent = nullptr);
    ~menuModificar();

    void setBC(Blockchain<Block*>*bc);
private:
    Ui::menuModificar *ui;
};

#endif // MENUMODIFICAR_H
