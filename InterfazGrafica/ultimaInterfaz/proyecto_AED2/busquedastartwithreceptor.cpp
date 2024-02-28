#include "busquedastartwithreceptor.h"
#include "ui_busquedastartwithreceptor.h"
#include <QLabel>
#include <QVBoxLayout>

busquedaStartWithReceptor::busquedaStartWithReceptor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaStartWithReceptor)
{
    ui->setupUi(this);
}

busquedaStartWithReceptor::~busquedaStartWithReceptor()
{
    delete ui;
}

void busquedaStartWithReceptor::setBC(Blockchain<Block *> *bc){
    this->bc = bc;
}
void busquedaStartWithReceptor::on_btn_atras_clicked()
{
    busquedaStartWith* anterior = new busquedaStartWith();
    anterior->setBC(bc);
    anterior->show();
    close();
}


void busquedaStartWithReceptor::on_btn_buscar_clicked()
{
    QVBoxLayout* lay = new QVBoxLayout(this);

    QString prefijo=ui->prefijo->text();
//    std::string fechaString = fecha.toLocal8Bit().constData();
    string prefijoStr = prefijo.toLocal8Bit().constData();

    CircularArray<Transferencia*>transferencias = bc->searchStartWith("receptor",prefijoStr); //búsqueda por contenido

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

