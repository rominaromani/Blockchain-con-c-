#include "busquedarangofecha.h"
#include "ui_busquedarangofecha.h"
#include <QLabel>
#include <QVBoxLayout>

busquedaRangoFecha::busquedaRangoFecha(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaRangoFecha)
{
    ui->setupUi(this);
}

busquedaRangoFecha::~busquedaRangoFecha()
{
    delete ui;
}

void busquedaRangoFecha::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}


void busquedaRangoFecha::on_btn_atras_clicked()
{
    busquedaRango* anterior = new busquedaRango();
    anterior->setBC(bc);
    anterior->show();
    close();
}


void busquedaRangoFecha::on_btn_buscar_clicked()
{
    QVBoxLayout* lay = new QVBoxLayout(this);

    QString fechaMin=ui->min->text();
    QString fechaMax=ui->max->text();
//    std::string fechaString = fecha.toLocal8Bit().constData();
    string fechaMinStr = fechaMin.toLocal8Bit().constData();
    string fechaMaxStr = fechaMax.toLocal8Bit().constData();

    CircularArray<Transferencia*>transferencias = bc->searchRange("fecha",fechaMinStr,fechaMaxStr); //búsqueda por igualdad
    QString aviso;
    QString resultados;
    if(transferencias.get_size()>0){
        aviso += "Número de transferencias encontradas: " + QString::number(transferencias.get_size()) + "\n";
        aviso += "Transferencias encontradas:\n";
        aviso += "------------------\n";

        for(int i=0;i<transferencias.get_size();i++){
            resultados += "\tResultado " + QString::number(i+1) +"\n";
            resultados += "Fecha: " + QString::fromStdString(transferencias[i]->fecha.getString()) +"\n";
            resultados += "Emisor: " + QString::fromStdString(transferencias[i]->emisor) + "\n";
            resultados += "Receptor: " + QString::fromStdString(transferencias[i]->receptor) + "\n";
            resultados += "Monto: " + QString::number(transferencias[i]->monto) + "\n";
            resultados += "------------------\n";
            QLabel* label = new QLabel(resultados);
            lay->addWidget(label);
            resultados = "";
        }

    }
    else{
        aviso += "No se encontraron transferencias entre " + fechaMin + " y " + fechaMax + "\n";
    }
    ui->aviso->setText(aviso); //envío resultados en la interfaz

    ui->scrollAreaWidgetContents->setLayout(lay);
}

