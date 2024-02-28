#include "menubusqueda.h"
#include "ui_menubusqueda.h"

menuBusqueda::menuBusqueda(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menuBusqueda)
{
    ui->setupUi(this);
}

menuBusqueda::~menuBusqueda()
{
    delete ui;
}

void menuBusqueda::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}

void menuBusqueda::on_btn_atras_clicked()
{
    MainWindow* anterior = new MainWindow();
    anterior->show();
    anterior->setBC(bc);
    close();
}


void menuBusqueda::on_btn_igualdad_clicked()
{
    Busqueda* siguiente = new Busqueda();
    siguiente->show();
    siguiente->setBC(bc);
    close();
}


void menuBusqueda::on_btn_max_min_clicked()
{
    Max_Min* siguiente = new Max_Min();
    siguiente->show();
    siguiente->setBC(bc);
    close();
}

