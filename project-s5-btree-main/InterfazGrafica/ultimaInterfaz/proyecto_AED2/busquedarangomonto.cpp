#include "busquedarangomonto.h"
#include "ui_busquedarangomonto.h"
#include <QLabel>
#include <QVBoxLayout>
//#include <QPushButton>

busquedaRangoMonto::busquedaRangoMonto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::busquedaRangoMonto)
{
    ui->setupUi(this);
}

busquedaRangoMonto::~busquedaRangoMonto()
{
    delete ui;
}

void busquedaRangoMonto::setBC(Blockchain<Block*>*bc){
    this->bc = bc;
}

void busquedaRangoMonto::on_btn_atras_clicked()
{
    busquedaRango* anterior = new busquedaRango();
    anterior->setBC(bc);
    anterior->show();
    close();
}


void busquedaRangoMonto::on_btn_buscar_clicked(){
//    string montoMinimo;
//    string montoMaximo;
//    cout << "Ingrese el monto mínimo:" << endl;
//    cin>>montoMinimo;
//    cout << "Ingrese el monto máximo:" << endl;
//    cin>>montoMaximo;
//    cin.ignore();
//    CircularArray<Transferencia*>transferencias = bc->searchRange("monto",montoMinimo,montoMaximo); //búsqueda por igualdad

//    if(transferencias.get_size()==0){
//        cout << "No se encontraron resultados entre el monto mínimo " << montoMinimo << " y el monto máximo " << montoMaximo << endl;
//    }
//    else{
//        cout << "Número de transferencias encontradas: " << transferencias.get_size() << endl;
//        cout << "Transferencias encontradas con monto entre " << montoMinimo << " y " << montoMaximo << ":" << endl;
//        cout << "------------------" << endl;

//        for(int i=0;i<transferencias.get_size();i++){
//            cout  << "\tResultado " << i+1 << endl;
//            cout << "Fecha: " << transferencias[i]->fecha.getString() << endl;
//            cout << "Emisor: " << transferencias[i]->emisor << endl;
//            cout << "Receptor: " << transferencias[i]->receptor << endl;
//            cout << "Monto: " << transferencias[i]->monto << endl;
//            cout << "------------------" << endl;
//        }
//    }


    QVBoxLayout* lay = new QVBoxLayout(this);

    QString montoMin=ui->min->text();
    QString montoMax=ui->max->text();
//    std::string fechaString = fecha.toLocal8Bit().constData();
    string montoMinStr = montoMin.toLocal8Bit().constData();
    string montoMaxStr = montoMax.toLocal8Bit().constData();

    CircularArray<Transferencia*>transferencias = bc->searchRange("monto",montoMinStr,montoMaxStr); //búsqueda por igualdad
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
        aviso += "No se encontraron transferencias entre " + montoMin + " y " + montoMax + "\n";
    }
    ui->resultados->setText(aviso); //envío resultados en la interfaz

    ui->scrollAreaWidgetContents->setLayout(lay);

}

