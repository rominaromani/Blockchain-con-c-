#include "busquedastartwithemisor.h"
#include "ui_busquedastartwithemisor.h"
#include <QLabel>
#include <QVBoxLayout>

busquedaStartWithEmisor::busquedaStartWithEmisor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaStartWithEmisor)
{
    ui->setupUi(this);
}

busquedaStartWithEmisor::~busquedaStartWithEmisor()
{
    delete ui;
}

void busquedaStartWithEmisor::setBC(Blockchain<Block *> *bc){
    this->bc = bc;
}
void busquedaStartWithEmisor::on_btn_atras_clicked()
{
    busquedaStartWith* anterior = new busquedaStartWith();
    anterior->setBC(bc);
    anterior->show();
    close();
}


void busquedaStartWithEmisor::on_btn_buscar_clicked()
{
    QVBoxLayout* lay = new QVBoxLayout(this);

    QString prefijo=ui->prefijo->text();
//    std::string fechaString = fecha.toLocal8Bit().constData();
    string prefijoStr = prefijo.toLocal8Bit().constData();

    CircularArray<Transferencia*>transferencias = bc->searchStartWith("emisor",prefijoStr); //búsqueda por contenido

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
        aviso += "No se encontraron transferencias con el prefijo: " + prefijo + "\n";
    }
    ui->aviso->setText(aviso); //envío resultados en la interfaz

    ui->scrollAreaWidgetContents->setLayout(lay);
}

