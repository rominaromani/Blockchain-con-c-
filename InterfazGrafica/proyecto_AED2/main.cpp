#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Blockchain<Block*>* bc = new Blockchain<Block*>();

    //Leemos información del block_transferencias
    ifstream archivo("C:/Users/ASUS/Documents/QT/proyecto_AED2/data-YlawKHeFu8AHyJAkeI0YK.csv");
    string linea;
    char delimitador = ',';
    CircularArray<Transferencia*> *array = new CircularArray<Transferencia*>();
    getline(archivo,linea);
    while(getline(archivo,linea)){
        stringstream stream(linea);
        string emisor, receptor, monto, fecha;
        getline(stream,emisor,delimitador);
        getline(stream,receptor,delimitador);
        getline(stream,monto,delimitador);
        getline(stream,fecha,delimitador);

        Transferencia *transferencia = new Transferencia(fecha,emisor,receptor,stoi(monto));
        array->push_back(transferencia);

        if (array->get_size()==4){
            Block *block = new Block();
            for(int i=0;i<4;i++){
                block->transferencias.push_back(array->pop_front());
            }
            block->setHash();
            bc->addBlock(block);
        }
    }
    //Enviamos el bc a MainWindow
    w.setBC(bc);
    w.show();
    return a.exec();
}
