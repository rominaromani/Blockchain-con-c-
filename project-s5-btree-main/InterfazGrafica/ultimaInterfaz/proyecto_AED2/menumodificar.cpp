#include "menumodificar.h"
#include "ui_menumodificar.h"

menuModificar::menuModificar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menuModificar)
{
    ui->setupUi(this);
}

menuModificar::~menuModificar()
{
    delete ui;
}

void menuModificar::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}
