#include "max_min.h"
#include "ui_max_min.h"

Max_Min::Max_Min(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Max_Min)
{
    ui->setupUi(this);
}

Max_Min::~Max_Min()
{
    delete ui;
}

void Max_Min::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}

void Max_Min::on_btn_atras_clicked()
{
    menuBusqueda* anterior = new menuBusqueda();
    anterior->show();
    anterior->setBC(bc);
    close();
}


void Max_Min::on_btn_maximo_clicked()
{
    Transferencia* maxMonto = bc->searchMax("monto");

    QString resultado;
    resultado += "\tResultado con el monto máximo\n";
    resultado += "Monto : " + QString::number(maxMonto->monto) +"\n";
    resultado += "Transferencia: " + QString::fromStdString(maxMonto->emisor) + " -> " +
            QString::fromStdString(maxMonto->receptor) +"\n";
    resultado += "Fecha: " + QString::fromStdString(maxMonto->fecha.getString()) + "\n";

    Transferencia* maxFecha = bc->searchMax("fecha");
    resultado += "------------------------\n";
    resultado += "\tResultado con la fecha máxima\n";
    resultado += "Monto : " + QString::number(maxFecha->monto) +"\n";
    resultado += "Transferencia: " + QString::fromStdString(maxFecha->emisor) + " -> " +
            QString::fromStdString(maxFecha->receptor) +"\n";
    resultado += "Fecha: " + QString::fromStdString(maxFecha->fecha.getString());

    ui->resultado_max->setText(resultado); //envío resultados en la interfaz
}


void Max_Min::on_btn_minimo_clicked()
{
    Transferencia* minMonto = bc->searchMin("monto");

    QString resultado;
    resultado += "\tResultado con el monto mínimo\n";
    resultado += "Monto : " + QString::number(minMonto->monto) +"\n";
    resultado += "Transferencia: " + QString::fromStdString(minMonto->emisor) + " -> " +
            QString::fromStdString(minMonto->receptor) +"\n";
    resultado += "Fecha: " + QString::fromStdString(minMonto->fecha.getString()) + "\n";

    Transferencia* minFecha = bc->searchMin("fecha");
    resultado += "------------------------\n";
    resultado += "\tResultado con la fecha mínima\n";
    resultado += "Monto : " + QString::number(minFecha->monto) +"\n";
    resultado += "Transferencia: " + QString::fromStdString(minFecha->emisor) + " -> " +
            QString::fromStdString(minFecha->receptor) +"\n";
    resultado += "Fecha: " + QString::fromStdString(minFecha->fecha.getString());

    ui->resultado_min->setText(resultado); //envío resultados en la interfaz
}

