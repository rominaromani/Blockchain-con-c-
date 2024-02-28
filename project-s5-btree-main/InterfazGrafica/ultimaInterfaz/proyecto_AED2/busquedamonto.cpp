#include "busquedamonto.h"
#include "ui_busquedamonto.h"

busquedaMonto::busquedaMonto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaMonto)
{
    ui->setupUi(this);
}

busquedaMonto::~busquedaMonto()
{
    delete ui;
}

void busquedaMonto::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}

void busquedaMonto::on_bton_atras_clicked()
{
    Busqueda* anterior = new Busqueda();
    anterior->show();
    anterior->setBC(bc);
    close();
}


void busquedaMonto::on_btn_buscar_clicked()
{
    QString monto=ui->respuesta->text();
    std::string montoString = monto.toLocal8Bit().constData();
    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("monto",montoString); //búsqueda por igualdad

    QString resultados;
    if(transferencias!=nullptr){
        resultados += "Número de transferencias encontradas: " + QString::number(transferencias->get_size()) + "\n";
        resultados += "Transferencias encontradas con monto " + monto + ":\n";
        resultados += "------------------\n";

        for(int i=0;i<transferencias->get_size();i++){
            resultados += "\tResultados " + QString::number(i+1) +"\n";
            resultados += "Fecha: " + QString::fromStdString((*transferencias)[i]->fecha.getString()) +"\n";
            resultados += "Emisor: " + QString::fromStdString((*transferencias)[i]->emisor) + "\n";
            resultados += "Receptor: " + QString::fromStdString((*transferencias)[i]->receptor) + "\n";
            resultados += "Monto: " + QString::number((*transferencias)[i]->monto) + "\n";
            resultados += "------------------\n";
        }
    }
    else{
        resultados += "No se encontraron transferencias con el monto " + monto + "\n";
    }
    ui->resultados->setText(resultados); //envío resultados en la interfaz
}

