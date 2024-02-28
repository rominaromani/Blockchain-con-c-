#include "busquedacontiene.h"
#include "ui_busquedacontiene.h"

busquedaContiene::busquedaContiene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaContiene)
{
    ui->setupUi(this);
}

busquedaContiene::~busquedaContiene()
{
    delete ui;
}

void busquedaContiene::setBC(Blockchain<Block *> *bc){
    this->bc = bc;
}

void busquedaContiene::on_btn_atras_clicked()
{
    menuBusqueda* anterior = new menuBusqueda();
    anterior->setBC(bc);
    anterior->show();
    close();
}


void busquedaContiene::on_btn_emisor_clicked()
{
    busquedaContieneEmisor* siguiente = new busquedaContieneEmisor();
    siguiente->setBC(bc);
    siguiente->show();
    close();
}


void busquedaContiene::on_btn_receptor_clicked()
{
    busquedaContieneReceptor* siguiente = new busquedaContieneReceptor();
    siguiente->setBC(bc);
    siguiente->show();
    close();
}

