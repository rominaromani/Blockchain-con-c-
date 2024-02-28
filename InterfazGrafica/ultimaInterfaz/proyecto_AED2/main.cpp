#include "mainwindow.h"
#include <iostream>
#include <ctime> // en block.h
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

#include "Structures/sha256.cpp"
#include "Structures/sha256.h"
#include "Structures/circularList.h"
#include "Structures/fecha.h"
#include "blockchain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Blockchain<Block*> *bc = new Blockchain<Block*>();
    int count=0;

    //Leemos información del block_transferencias
//    ifstream archivo("C:/Users/ASUS/Documents/QT/proyecto_AED2/data-YlawKHeFu8AHyJAkeI0YK.csv");
     ifstream archivo("C:/Users/ASUS/Documents/QT/proyecto_AED2/data-copia.csv");
    string linea;
    char delimitador = ',';
    CircularArray<Transferencia*> *array = new CircularArray<Transferencia*>(); // almacenar transacciones
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

        if (array->get_size() == 4){
            // cout << "Block " << count << " time creation: ";
            // auto start = std::chrono::steady_clock::now();

            Block *block = new Block();
            for(int i=0;i<4;i++){
                block->transferencias.push_back(array->pop_front());
            }
            bc->setPrevAndHash(block);
            block->mine();
            bc->addBlock(block);

            // auto end = std::chrono::steady_clock::now();
            // cout<<chrono::duration_cast<chrono::milliseconds>(end - start).count()<<" ms\n";

            count++;
        }
    }

    //Enviamos el bc a MainWindow
    w.setBC(bc);
    w.show();
    return a.exec();
}
