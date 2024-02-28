#include "busquedafecha.h"
#include "ui_busquedafecha.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

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
    QVBoxLayout* lay = new QVBoxLayout(this);

    QString fecha=ui->respuesta->text();
    std::string fechaString = fecha.toLocal8Bit().constData();
    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("fecha",fechaString); //búsqueda por igualdad
    QString aviso;
    QString resultados;
    if(transferencias!=nullptr){
        aviso += "Número de transferencias encontradas: " + QString::number(transferencias->get_size()) + "\n";
        aviso += "Transferencias encontradas con fecha " + fecha + ":\n";
        aviso += "------------------\n";

        for(int i=0;i<transferencias->get_size();i++){
            resultados += "\tResultado " + QString::number(i+1) +"\n";
            resultados += "Fecha: " + QString::fromStdString((*transferencias)[i]->fecha.getString()) +"\n";
            resultados += "Emisor: " + QString::fromStdString((*transferencias)[i]->emisor) + "\n";
            resultados += "Receptor: " + QString::fromStdString((*transferencias)[i]->receptor) + "\n";
            resultados += "Monto: " + QString::number((*transferencias)[i]->monto) + "\n";
            resultados += "------------------\n";
            QLabel* label = new QLabel(resultados);
            lay->addWidget(label);
            resultados = "";
        }

    }
    else{
        aviso += "No se encontraron transferencias con la fecha " + fecha + "\n";
    }
    ui->resultados->setText(aviso); //envío resultados en la interfaz
    ui->scrollAreaWidgetContents->setLayout(lay);
}

