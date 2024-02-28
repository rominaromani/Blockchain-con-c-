#include "busquedafecha.h"
#include "ui_busquedafecha.h"

busquedaFecha::busquedaFecha(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaFecha)
{
    ui->setupUi(this);
}

busquedaFecha::~busquedaFecha()
{
    delete ui;
}

void busquedaFecha::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}
void busquedaFecha::on_btn_atras_clicked()
{
    Busqueda* anterior = new Busqueda();
    anterior->show();
    anterior->setBC(bc);
    close();
}


void busquedaFecha::on_btn_buscar_clicked()
{
    QString fecha=ui->respuesta->text();
    std::string fechaString = fecha.toLocal8Bit().constData();
    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("fecha",fechaString); //búsqueda por igualdad

    QString resultados;
    if(transferencias!=nullptr){
        resultados += "Número de transferencias encontradas: " + QString::number(transferencias->get_size()) + "\n";
        resultados += "Transferencias encontradas con fecha " + fecha + ":\n";
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
        resultados += "No se encontraron transferencias con la fecha " + fecha + "\n";
    }
    ui->resultados->setText(resultados); //envío resultados en la interfaz
}

