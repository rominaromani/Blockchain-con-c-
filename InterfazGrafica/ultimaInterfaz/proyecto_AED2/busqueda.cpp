#include "busqueda.h"
#include "ui_busqueda.h"
#include "mainwindow.h"
#include "busquedaemisor.h"

Busqueda::Busqueda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Busqueda)
{
    ui->setupUi(this);
}

Busqueda::~Busqueda()
{
    delete ui;
}

void Busqueda::on_btn_atras_clicked()
{
    menuBusqueda* anterior = new menuBusqueda();
    anterior->show();
    //Le paso el bc a la página anterior
    anterior->setBC(bc);
    close();
}

void Busqueda::on_btn_emisor_clicked()
{
    busquedaEmisor* siguiente = new busquedaEmisor();
    siguiente->show();
    //Le paso el bc a la página siguiente
    siguiente->setBC(bc);
    close();
}

void Busqueda::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}

void Busqueda::on_btn_receptor_clicked()
{
    busquedaReceptor* siguiente = new busquedaReceptor();
    siguiente->show();
    //Le paso el bc a la página siguiente
    siguiente->setBC(bc);
    close();
}


void Busqueda::on_btn_monto_clicked()
{
    busquedaMonto* siguiente = new busquedaMonto();
    siguiente->show();
    siguiente->setBC(bc);
    close();
}


void Busqueda::on_btn_fecha_clicked()
{
    busquedaFecha* siguiente = new busquedaFecha();
    siguiente->show();
    siguiente->setBC(bc);
    close();
}

