#include "busquedacontienereceptor.h"
#include "ui_busquedacontienereceptor.h"
#include <QLabel>
#include <QVBoxLayout>

busquedaContieneReceptor::busquedaContieneReceptor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaContieneReceptor)
{
    ui->setupUi(this);
}

busquedaContieneReceptor::~busquedaContieneReceptor()
{
    delete ui;
}

void busquedaContieneReceptor::setBC(Blockchain<Block *> *bc){
    this->bc = bc;
}
void busquedaContieneReceptor::on_btn_atras_clicked()
{
    busquedaContiene* anterior = new busquedaContiene();
    anterior->setBC(bc);
    anterior->show();
    close();
}


void busquedaContieneReceptor::on_btn_buscar_clicked()
{
    QVBoxLayout* lay = new QVBoxLayout(this);

    QString patron=ui->patron->text();
//    std::string fechaString = fecha.toLocal8Bit().constData();
    string patronStr = patron.toLocal8Bit().constData();

    CircularArray<Transferencia*>transferencias = bc->searchContains(patronStr,"receptor"); //búsqueda por contenido

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
        aviso += "No se encontraron transferencias con el patrón: " + patron + "\n";
    }
    ui->aviso->setText(aviso); //envío resultados en la interfaz

    ui->scrollAreaWidgetContents->setLayout(lay);
}

