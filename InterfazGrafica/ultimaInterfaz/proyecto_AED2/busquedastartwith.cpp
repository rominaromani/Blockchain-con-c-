#include "busquedastartwith.h"
#include "ui_busquedastartwith.h"

busquedaStartWith::busquedaStartWith(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaStartWith)
{
    ui->setupUi(this);
}

busquedaStartWith::~busquedaStartWith()
{
    delete ui;
}

void busquedaStartWith::setBC(Blockchain<Block *> *bc){
    this->bc = bc;
}

void busquedaStartWith::on_btn_atras_clicked()
{
    menuBusqueda* anterior = new menuBusqueda();
    anterior->setBC(bc);
    anterior->show();
    close();
}


void busquedaStartWith::on_btn_emisor_clicked()
{
    busquedaStartWithEmisor* siguiente = new busquedaStartWithEmisor();
    siguiente->setBC(bc);
    siguiente->show();
    close();
}


void busquedaStartWith::on_btn_receptor_clicked()
{
    busquedaStartWithReceptor* siguiente = new busquedaStartWithReceptor();
    siguiente->setBC(bc);
    siguiente->show();
    close();
}

