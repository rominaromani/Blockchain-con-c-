#include <iostream>
#include <ctime> // en block.h
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>

using namespace std;

#include "Structures/sha256.cpp"
#include "Structures/sha256.h"
#include "Structures/circularList.h"
#include "Structures/fecha.h"
#include "blockchain.h"

// Cambiar cantidad de datos en main()

// Criterios de busqueda
    //Criterios para primera entrega
    // 1. Igual a X
    // TV search(TK key)                                ---> HASH 
    // 2. Entre X y Y 
    // vector<TV> range_search(TK begin, TK end)        ---> AVL - B+ - B


    // 3. Inicia con
    // vector<TV> start_with(string prefix)             ---> arbol - tries O(m)
    // 4. Está contenido en
    // vector<TV> contains(string pattern)              ---> 
    // 5. Máximo valor de
    // TV max_value( )                                  ---> HEAP
    // 6. Mínimo valor de
    // TV min_value( )                                  ---> HEAP


int main(){
    Blockchain<Block*> *bc = new Blockchain<Block*>();
    int count=0;

    //Leemos información del block_transferencias
    // ifstream archivo("data-YlawKHeFu8AHyJAkeI0YK.csv"); // 10 mil datos
    ifstream archivo("data-copia.csv"); // 50 datos
    string linea;
    char delimitador = ',';
    CircularArray<Transferencia*> *array = new CircularArray<Transferencia*>(); // almacenar transacciones
    getline(archivo,linea);

    auto insert = std::chrono::steady_clock::now();
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
    auto endInsert = std::chrono::steady_clock::now();
    cout << "Time to create BlockChain: "<<chrono::duration_cast<chrono::seconds>(endInsert - insert).count()<<" s\n";

    // for(int i=0;i<bc->chain.get_size();i++){
    //     cout << "Bloque " << i << endl;
    //     cout << "Cantidad de transferencias: " << bc->chain[i]->transferencias.get_size() << endl;
    //     cout << "Nonce: " << bc->chain[i]->nonce << endl;
    //     cout << "Hash: " << *bc->chain[i]->get_Hash() << endl;
    //     cout << "PrevBlockHash: " << *bc->chain[i]->PrevBlockHash << endl;
    //     cout << "------------------" << endl;
    //     for(int j=0;j<bc->chain[i]->transferencias.get_size();j++){
    //         cout << "Bloque " << i << endl;
    //         bc->chain[i]->transferencias[j]->display();
    //         cout << "------------------" << endl;
    //     }
    //     cout << "Block valid: " << (bc->chain[i]->validatedBlock() ? "valido" : "invalido") << endl;
    //     cout << "--------------------------------------------------------" << endl;
    // }
    
    int opcion=1;
    while (opcion != 0){
        do {
            cout << "\tMenu" << endl;
            cout << "1. Buscar" << endl;
            cout << "2. Modificar bloque" << endl;
            cout << "3. Añadir bloque" << endl;
            cout << "4. Eliminar datos del bloque" << endl;
            cout << "5. Imprimir por rango de bloques" << endl;
            cout << "6. Minar bloques" << endl;
            cout << "0. Salir" << endl;
            cout << "Ingrese la opción: ";
            cin >> opcion;
        } while (opcion < 0 || opcion > 6);
        cin.ignore(); //limpia el cin para poder luego usar getline sin errores
        string emisor;
        string receptor;
        string fecha;
        int monto;
        if (opcion==1){ // 1 ----------------------------------------------------------------------------------------------
            cout << endl << "Ingrese la modalidad de búsqueda: " << endl;
            cout << "1. Igual a X" << endl;
            cout << "2. Entre X y Y" << endl;
            cout << "3. Inicia con" << endl;
            cout << "4. Está contenido en" << endl;
            cout << "5. Máximo valor" << endl;
            cout << "6. Mínimo valor" << endl;
            cout << "7. Monto total recibido-entregado" << endl;

            int modalidad;
            cin >> modalidad;
            cin.ignore();
            if (modalidad == 1){ // 1 ----------------------------------------------------------------------------------------------
                cout << endl << "¿Por cuál dato desea buscar?" << endl;
                cout << "1. Emisor" << endl;
                cout << "2. Receptor" << endl;
                cout << "3. Monto" << endl;
                cout << "4. Fecha" << endl;
                int dato;
                cin >> dato;
                cin.ignore();
                if(dato==1){
                    string emisor;
                    cout << "Ingrese el emisor: " << endl;
                    getline(cin,emisor);
                    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("emisor",emisor); //búsqueda por igualdad
                    cout << "terminó la búsqueda" << endl;
                    if(transferencias != nullptr){
                        cout << "Número de transferencias encontradas: " << transferencias->get_size() << endl;
                        cout << "Transferencias encontradas con emisor " << emisor << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias->get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << (*transferencias)[i]->fecha.getString() << endl;
                            cout << "Emisor: " << (*transferencias)[i]->emisor << endl;
                            cout << "Receptor: " << (*transferencias)[i]->receptor << endl;
                            cout << "Monto: " << (*transferencias)[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                    else{
                        cout << "No se encontraron transferencias con el emisor " << emisor << endl;
                    }
                }
                else if(dato==2){
                    string receptor;
                    cout << "Ingrese el receptor: " << endl;
                    getline(cin,receptor);
                    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("receptor",receptor); //búsqueda por igualdad
                    cout << "terminó la búsqueda" << endl;
                    if(transferencias!=nullptr){
                        cout << "Número de transferencias encontradas: " << transferencias->get_size() << endl;
                        cout << "Transferencias encontradas con receptor " << receptor << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias->get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << (*transferencias)[i]->fecha.getString() << endl;
                            cout << "Emisor: " << (*transferencias)[i]->emisor << endl;
                            cout << "Receptor: " << (*transferencias)[i]->receptor << endl;
                            cout << "Monto: " << (*transferencias)[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                    else{
                        cout << "No se encontraron transferencias con el receptor " << receptor << endl;
                    }
                }
                else if(dato==3){
                    string monto;
                    cout << "Ingrese el monto: " << endl;
                    getline(cin,monto);
                    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("monto",monto); //búsqueda por igualdad
                    cout << "terminó la búsqueda" << endl;
                    if(transferencias!=nullptr){
                        cout << "Número de transferencias encontradas: " << transferencias->get_size() << endl;
                        cout << "Transferencias encontradas con monto " << monto << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias->get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << (*transferencias)[i]->fecha.getString() << endl;
                            cout << "Emisor: " << (*transferencias)[i]->emisor << endl;
                            cout << "Receptor: " << (*transferencias)[i]->receptor << endl;
                            cout << "Monto: " << (*transferencias)[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                    else{
                        cout << "No se encontraron transferencias con el monto " << monto << endl;
                    }
                }
                else if(dato==4){       //Encuentra si se le pasa la fecha en formato d/m/aaaa (sin 0s adelante)
                    string fecha;
                    cout << "Ingrese la fecha: " << endl;
                    getline(cin,fecha);
                    CircularArray<Transferencia*> *transferencias = bc->searchIgualdad("fecha",fecha); //búsqueda por igualdad
                    if(transferencias!=nullptr){
                        cout << "Número de transferencias encontradas: " << transferencias->get_size() << endl;
                        cout << "Transferencias encontradas con fecha " << fecha << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias->get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << (*transferencias)[i]->fecha.getString() << endl;
                            cout << "Emisor: " << (*transferencias)[i]->emisor << endl;
                            cout << "Receptor: " << (*transferencias)[i]->receptor << endl;
                            cout << "Monto: " << (*transferencias)[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                    else{
                        cout << "No se encontraron transferencias con esa fecha " << fecha << endl;
                    }
                }
                else{
                    cout << "Opción inválida" << endl;
                }
            }
            else if(modalidad == 2){ // 2 ----------------------------------------------------------------------------------------------
                cout << "¿Por cuál dato desea buscar?" << endl;
                cout << "1. Monto" << endl;
                cout << "2. Fecha" << endl;
                int dato;
                cin >> dato;
                cin.ignore();

                if(dato==1){
                    string montoMinimo;
                    string montoMaximo;
                    cout << "Ingrese el monto mínimo:" << endl;
                    cin>>montoMinimo;
                    cout << "Ingrese el monto máximo:" << endl;
                    cin>>montoMaximo;
                    cin.ignore();
                    CircularArray<Transferencia*>transferencias = bc->searchRange("monto",montoMinimo,montoMaximo); //búsqueda por igualdad

                    if(transferencias.get_size()==0){
                        cout << "No se encontraron resultados entre el monto mínimo " << montoMinimo << " y el monto máximo " << montoMaximo << endl;
                    }
                    else{
                        cout << "Número de transferencias encontradas: " << transferencias.get_size() << endl;
                        cout << "Transferencias encontradas con monto entre " << montoMinimo << " y " << montoMaximo << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias.get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << transferencias[i]->fecha.getString() << endl;
                            cout << "Emisor: " << transferencias[i]->emisor << endl;
                            cout << "Receptor: " << transferencias[i]->receptor << endl;
                            cout << "Monto: " << transferencias[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                }
                else if(dato==2){     //Encuentra si se le pasa la fecha en formato dd/mm/aaaa (con 0s adelante)
                    string fechaMinima;
                    string fechaMaxima;
                    cout << "Ingrese la fecha mínima:" << endl;
                    getline(cin,fechaMinima);
                    cout << "Ingrese la fecha máxima:" << endl;
                    getline(cin,fechaMaxima);
                    CircularArray<Transferencia*>transferencias = bc->searchRange("fecha",fechaMinima,fechaMaxima); //búsqueda por igualdad

                    if(transferencias.get_size()==0){
                        cout << "No se encontraron resultados entre la fecha mínima " << fechaMinima << " y la fecha máxima " << fechaMaxima << endl;
                    }
                    else{
                        cout << "Número de transferencias encontradas: " << transferencias.get_size() << endl;
                        cout << "Transferencias encontradas con fecha entre " << fechaMinima << " y " << fechaMaxima << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias.get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << transferencias[i]->fecha.getString() << endl;
                            cout << "Emisor: " << transferencias[i]->emisor << endl;
                            cout << "Receptor: " << transferencias[i]->receptor << endl;
                            cout << "Monto: " << transferencias[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                }
                else{
                    cout << "Opción inválida" << endl;
                }
            }
            else if(modalidad == 3){ // 3 ----------------------------------------------------------------------------------------------
                cout << "¿Por cuál dato desea buscar?" << endl;
                cout << "1. Emisor" << endl;
                cout << "2. Receptor" << endl;
                int dato;
                cin >> dato;
                cin.ignore();
                if(dato==1){
                    string emisor;
                    cout << "Ingrese el prefijo del emisor: " << endl;
                    getline(cin,emisor);
                    CircularArray<Transferencia*>transferencias = bc->searchStartWith("emisor",emisor); //búsqueda por igualdad
                    if(transferencias.get_size()!=0){
                        cout << "Número de transferencias encontradas: " << transferencias.get_size() << endl;
                        cout << "Transferencias encontradas con el prefijo de emisor " << emisor << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias.get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << transferencias[i]->fecha.getString() << endl;
                            cout << "Emisor: " << transferencias[i]->emisor << endl;
                            cout << "Receptor: " << transferencias[i]->receptor << endl;
                            cout << "Monto: " << transferencias[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                    else{
                        cout << "No se encontraron transferencias con el prefijo de emisor " << emisor << endl;
                    }
                }
                else if(dato==2){
                    string receptor;
                    cout << "Ingrese el prefijo del receptor: " << endl;
                    getline(cin,receptor);
                    CircularArray<Transferencia*> transferencias = bc->searchStartWith("receptor",receptor); //búsqueda por igualdad
                    cout << "terminó la búsqueda" << endl;
                    if(transferencias.get_size()!=0){
                        cout << "Número de transferencias encontradas: " << transferencias.get_size() << endl;
                        cout << "Transferencias encontradas con el prefijo de receptor " << receptor << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias.get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << transferencias[i]->fecha.getString() << endl;
                            cout << "Emisor: " << transferencias[i]->emisor << endl;
                            cout << "Receptor: " << transferencias[i]->receptor << endl;
                            cout << "Monto: " << transferencias[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                    else{
                        cout << "No se encontraron transferencias con el prefijo de receptor " << receptor << endl;
                    }
                }
            }
            else if(modalidad == 4){
                cout << endl << "¿Por cuál dato desea buscar?" << endl;
                cout << "1. Emisor" << endl;
                cout << "2. Receptor" << endl;
                int dato;
                cin >> dato;
                cin.ignore();
                if(dato==1){
                    string emisor;
                    cout << "Ingrese el patron del emisor: " << endl;
                    getline(cin,emisor);
                    CircularArray<Transferencia*>transferencias = bc->searchContains(emisor,"emisor"); //búsqueda por contenido
                    if(transferencias.get_size()!=0){
                        cout << "Número de transferencias encontradas: " << transferencias.get_size() << endl;
                        cout << "Transferencias encontradas con el patron de emisor " << emisor << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias.get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << transferencias[i]->fecha.getString() << endl;
                            cout << "Emisor: " << transferencias[i]->emisor << endl;
                            cout << "Receptor: " << transferencias[i]->receptor << endl;
                            cout << "Monto: " << transferencias[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                    else{
                        cout << "No se encontraron transferencias con el patron de emisor " << emisor << endl;
                    }
                }
                else if(dato==2){
                    string receptor;
                    cout << "Ingrese el patron del receptor: " << endl;
                    getline(cin,receptor);
                    CircularArray<Transferencia*> transferencias = bc->searchContains(receptor,"receptor"); //búsqueda por contenido
                    cout << "terminó la búsqueda" << endl;
                    if(transferencias.get_size()!=0){
                        cout << "Número de transferencias encontradas: " << transferencias.get_size() << endl;
                        cout << "Transferencias encontradas con el patron de receptor " << receptor << ":" << endl;
                        cout << "------------------" << endl;

                        for(int i=0;i<transferencias.get_size();i++){
                            cout  << "\tResultado " << i+1 << endl;
                            cout << "Fecha: " << transferencias[i]->fecha.getString() << endl;
                            cout << "Emisor: " << transferencias[i]->emisor << endl;
                            cout << "Receptor: " << transferencias[i]->receptor << endl;
                            cout << "Monto: " << transferencias[i]->monto << endl;
                            cout << "------------------" << endl;
                        }
                    }
                    else{
                        cout << "No se encontraron transferencias con el patron de receptor " << receptor << endl;
                    }
                }else{
                    cout << "Opción inválida" << endl;
                }
            }else if(modalidad ==7){
                cout << "¿Por cuál dato desea buscar?" << endl;
                cout << "1. Emisor" << endl;
                cout << "2. Receptor" << endl;
                int dato;
                cin >> dato;
                cin.ignore();
                if(dato==1){
                    string emisor;
                    cout << "Ingrese el emisor: " << endl;
                    getline(cin,emisor);
                    
                    int montoTotal = bc->MontoTotal("emisor",emisor); 
                    cout << "terminó la búsqueda" << endl;
                    cout << "Monto total transferido por " << emisor << ": " << montoTotal << endl;
                    
                }
                else if(dato==2){
                    string receptor;
                    cout << "Ingrese el receptor: " << endl;
                    getline(cin,receptor);

                    int montoTotal = bc->MontoTotal("receptor",receptor);
                    cout << "terminó la búsqueda" << endl;
                    cout << "Monto total recibido por" << receptor << ": " << montoTotal << endl;
                }

            }
            else if(modalidad == 5){ // 5 ----------------------------------------------------------------------------------------------
                cout << "¿De cuál dato desea hallar el máximo valor?" << endl;
                cout << "1. Monto" << endl;
                cout << "2. Fecha" << endl;
                int dato;
                cin >> dato;
                cin.ignore();
                if(dato==1){
                    Transferencia* t = bc->searchMax("monto");
                    cout << "Monto máximo: " << t->monto << endl;
                    cout << "Transferencia: " << t->emisor << " -> " << t->receptor << endl;
                    cout << "Fecha: " << t->fecha.getString() << endl;
                }
                else if(dato==2){
                    Transferencia* t3 = bc->searchMax("fecha");
                    cout << "Fecha máxima: " << t3->fecha.getString() << endl;
                    cout << "Transferencia: " << t3->emisor << " -> " << t3->receptor << endl;
                    cout << "Monto: " << t3->monto << endl;
                }
            }
            else if(modalidad == 6){ // 6 ----------------------------------------------------------------------------------------------
                cout << "¿De cuál dato desea hallar el mínimo valor?" << endl;
                cout << "1. Monto" << endl;
                cout << "2. Fecha" << endl;
                int dato;
                cin >> dato;
                cin.ignore();
                if(dato==1){
                    Transferencia* t2 = bc->searchMin("monto");
                    cout << "Monto mínimo: " << t2->monto << endl;
                    cout << "Transferencia: " << t2->emisor << " -> " << t2->receptor << endl;
                    cout << "Fecha: " << t2->fecha.getString() << endl;
                }
                else if(dato==2){
                    Transferencia* t4 = bc->searchMin("fecha");
                    cout << "Fecha mínima: " << t4->fecha.getString() << endl;
                    cout << "Transferencia: " << t4->emisor << " -> " << t4->receptor << endl;
                    cout << "Monto: " << t4->monto << endl;
                }
            }
            
        }
        else if (opcion == 2){ // 2 ----------------------------------------------------------------------------------------------
            bc->modifyBlock();
            cout << "--------------------------------------------------------" << endl;
        }
        else if (opcion == 3){ // 3 ----------------------------------------------------------------------------------------------
            bc->newBlock();
            cout << "--------------------------------------------------------" << endl;
        }
        else if (opcion == 4){ // 4 ----------------------------------------------------------------------------------------------
            bc->deleteBlock();
            cout << "--------------------------------------------------------" << endl;
        }
        else if (opcion == 5){ // 5 ----------------------------------------------------------------------------------------------
            bc->printBlocks();
            cout << "--------------------------------------------------------" << endl;
        }
        else if (opcion == 6){ // 6 ----------------------------------------------------------------------------------------------
            bc->mineBlocks();
            cout << "--------------------------------------------------------" << endl;
        }
        else if (opcion == 0) {
            break;
        }
    }
    
    return 0;
}