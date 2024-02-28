#include "busquedareceptor.h"
#include "ui_busquedareceptor.h"

busquedaReceptor::busquedaReceptor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaReceptor)
{
    ui->setupUi(this);
}

busquedaReceptor::~busquedaReceptor()
{
    delete ui;
}

void busquedaReceptor::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}

void busquedaReceptor::on_btn_atras_clicked()
{
    Busqueda* anterior = new Busqueda();
    anterior->setBC(bc);
    anterior->show();
    close();
}


void busquedaReceptor::on_btn_buscar_clicked()
{
    QString receptor=ui->respuesta->text();
    std::string receptorString = receptor.toLocal8Bit().constData();
    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("receptor",receptorString); //búsqueda por igualdad

    QString resultados;
    if(transferencias!=nullptr){
        resultados += "Número de transferencias encontradas: " + QString::number(transferencias->get_size()) + "\n";
        resultados += "Transferencias encontradas con receptor " + receptor + ":\n";
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
        resultados += "No se encontraron transferencias con el receptor " + receptor + "\n";
    }
    ui->resultados->setText(resultados); //envío resultados en la interfaz
}

