#include "busquedaemisor.h"
#include "ui_busquedaemisor.h"

using namespace std;

busquedaEmisor::busquedaEmisor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaEmisor)
{
    ui->setupUi(this);
}

busquedaEmisor::~busquedaEmisor()
{
    delete ui;
}

void busquedaEmisor::setBC(Blockchain<Block *> *bc){
    this->bc = bc;
}

void busquedaEmisor::on_btn_atras_clicked()
{
    //Voy a la anterior página
    Busqueda* anterior = new Busqueda();
    anterior->show();
    //Le paso a la página anterior el bc
    anterior->setBC(bc);
    //Cierro la página actual
    close();
}


void busquedaEmisor::on_btn_buscar_clicked()
{
    QString emisor=ui->dato_emisor->text();
    std::string emisorString = emisor.toLocal8Bit().constData();
    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("emisor",emisorString); //búsqueda por igualdad

    QString resultados;
    if(transferencias!=nullptr){
        resultados += "Número de transferencias encontradas: " + QString::number(transferencias->get_size()) + "\n";
        resultados += "Transferencias encontradas con emisor " + emisor + ":\n";
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
        resultados += "No se encontraron transferencias con el emisor " + emisor + "\n";
    }
    ui->resultados->setText(resultados); //envío resultados en la interfaz
}

