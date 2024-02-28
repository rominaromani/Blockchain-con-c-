#include "busquedarango.h"
#include "ui_busquedarango.h"

busquedaRango::busquedaRango(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaRango)
{
    ui->setupUi(this);
}

busquedaRango::~busquedaRango()
{
    delete ui;
}

void busquedaRango::on_pushButton_clicked()
{
    menuBusqueda* anterior = new menuBusqueda();
    anterior->setBC(bc);
    anterior->show();
    close();
}

void busquedaRango::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}


void busquedaRango::on_btn_monto_clicked()
{
    busquedaRangoMonto* siguiente = new busquedaRangoMonto();
    siguiente->setBC(bc);
    siguiente->show();
    close();
}


void busquedaRango::on_btn_fecha_clicked()
{
    busquedaRangoFecha* siguiente = new busquedaRangoFecha();
    siguiente->setBC(bc);
    siguiente->show();
    close();
}

