#include <iostream> 
#include "circulararray.h"
#include "block.h"
#include "chainhash.h"
#include "maxHeap.h"
#include "minHeap.h"

using namespace std;

template<typename T>
struct Blockchain{
    public:
    CircularArray<T> chain; // Vector
    ChainHash<string, CircularArray<Transferencia*>*> *hashFecha; // Hash
    ChainHash<string, CircularArray<Transferencia*>*> *hashEmisor; // Hash
    ChainHash<string, CircularArray<Transferencia*>*> *hashReceptor; // Hash
    ChainHash<int, CircularArray<Transferencia*>*> *hashMonto; // Hash

    MaxHeap<int> max; // Heap
    MaxHeap<Fecha> maxFecha; // Heap
    MinHeap<Fecha> minFecha; // Heap
    MinHeap<int> min; // Heap

    Blockchain(){
        chain.push_back(new Block()); //se crea el primer bloque de la cadena (puntero a bloque)
        hashFecha = new ChainHash<string, CircularArray<Transferencia*>*>(); // Hash
        hashEmisor = new ChainHash<string, CircularArray<Transferencia*>*>(); // Hash
        hashReceptor = new ChainHash<string, CircularArray<Transferencia*>*>(); // Hash
        hashMonto = new ChainHash<int, CircularArray<Transferencia*>*>(); // Hash
    }

    void addBlock(T bloque){
        T lastBlocK = chain[chain.get_size()-1]; //obtiene el último bloque de la cadena
        bloque->PrevBlockHash = lastBlocK->Hash; //asigna el hash del último bloque al nuevo bloque
        //Añadimos el bloque al final del BlockChain
        chain.push_back(bloque);
    }


    CircularArray<Transferencia*>* searchIgualdad(string atributo, string key){
        if(atributo == "fecha"){
            CircularArray<Transferencia*> *resultados;
            
            if(hashFecha->getSize() != 0){
                //get (O(1))
                resultados = hashFecha->get(key);
                cout << "Búsqueda con O(1)" << endl;
            }
            else{ //si no hay datos en el hashtable
                //n = cantidad de bloques del blockchain
                //1. set (O(4n) -> O(n)))
                cout << "Búsqueda con O(n)" << endl;
                CircularArray<Fecha> fechas;
                //obtener la lista de fechas //O(n*4*365) == O(1465n) -> Como trabajamos con un n muy grande: O(n)
                for(long long unsigned int i=0;i<this->chain.get_size();i++){ //O(n)
                    // añadir solo los fechas que no esten en la lista
                    for(long long unsigned int j=0;j<this->chain[i]->transferencias.get_size();j++){ //O(T) -> O(4) ; T = cantidad de transferencias en el bloque (4)
                        if(!fechas.contains(this->chain[i]->transferencias[j]->fecha)){  //O(F) -> O(365); F = cantidad de fechas distintas (365 en este caso, pues los datos están en un intervalo de un año)
                            fechas.push_back(this->chain[i]->transferencias[j]->fecha);
                        }
                    }
                }
                //Setear el hash con pares (fecha, lista de transacciones con esa fecha)
                //Complejidad: O(n*4*365) == O(1465n) -> Como trabajamos con un n muy grande: O(n)
                for(long long unsigned int i=0;i<fechas.get_size();i++){ //O(F) -> O(365) ; F = cantidad de fechas distintas (365)
                    CircularArray<Transferencia*> *arr = new CircularArray<Transferencia*>();
                    for(long long unsigned int j=0;j<this->chain.get_size();j++){ //O(n)
                        for(long long unsigned int k=0;k<this->chain[j]->transferencias.size();k++){ //O(4) ; T = cantidad de transferencias en el bloque (4)
                            if(this->chain[j]->transferencias[k]->fecha == fechas[i]){
                                arr->push_back(this->chain[j]->transferencias[k]);
                            }
                        }
                    }
                    hashFecha->set(fechas[i].getString(),arr);
                }
                //2. get (O(1))
                resultados = hashFecha->get(key);
            }
            return resultados;
        }
        else if(atributo=="emisor"){
            CircularArray<Transferencia*> *resultados;
            if(hashEmisor->getSize() != 0){
                //get O(1)
                cout << "Búsqueda con O(1)" << endl;
                resultados = hashEmisor->get(key);
            }
            else{ //si no hay datos en el hashtable
                //n = cantidad de bloques del blockchain
                cout << "Búsqueda con O(n^2)" << endl;
                //O(n^2)
                CircularArray<string> emisores;
                for(long long unsigned int i=0;i<this->chain.get_size();i++){ //O(n)
                    // añadir solo los emisores que no esten en la lista
                    for(long long unsigned int j=0;j<this->chain[i]->transferencias.get_size();j++){ //O(4)
                        if(!emisores.contains(this->chain[i]->transferencias[j]->emisor)){ //O(E) -> O(4n) ; E = cantidad de emisores distintos (4n, pues puede haber un emisor por cada transferencia
                            emisores.push_back(this->chain[i]->transferencias[j]->emisor);
                        }
                    }
                }
                //O(n^2)
                for(long long unsigned int i=0;i<emisores.get_size();i++){  //O(E) -> O(n) ; E = cantidad de emisores distintos (n, pues puede haber un emisor por cada transferencia)
                    CircularArray<Transferencia*> *arr = new CircularArray<Transferencia*>();
                    for(long long unsigned int j=0;j<this->chain.get_size();j++){ //O(n)
                        for(long long unsigned int k=0;k<this->chain[j]->transferencias.size();k++){    //O(4)
                            if(this->chain[j]->transferencias[k]->emisor == emisores[i]){
                                arr->push_back(this->chain[j]->transferencias[k]);
                            }
                        }
                    }
                    hashEmisor->set(emisores[i],arr);
                }
                //2. get O(1)
                resultados = hashEmisor->get(key);
            }
            return resultados;
        }
        else if(atributo=="receptor"){
            CircularArray<Transferencia*> *resultados;
            if(hashReceptor->getSize() != 0){
                //get O(1)
                cout << "Búsqueda con O(1)" << endl;
                resultados = hashReceptor->get(key);
            }
            else{ //si no hay datos en el hashtable
                //n = cantidad de bloques del blockchain
                cout << "Búsqueda con O(n^2)" << endl;
                //O(n^2)
                CircularArray<string> receptores;
                for(long long unsigned int i=0;i<this->chain.get_size();i++){ //O(n)
                    // añadir solo los emisores que no esten en la lista
                    for(long long unsigned int j=0;j<this->chain[i]->transferencias.get_size();j++){ //O(4)
                        if(!receptores.contains(this->chain[i]->transferencias[j]->receptor)){
                            receptores.push_back(this->chain[i]->transferencias[j]->receptor);  //O(R) -> O(4n) ; R = cantidad de receptores distintos (4n, pues puede haber un receptor por cada transferencia)
                        }
                    }
                }
                //O(n^2)
                for(long long unsigned int i=0;i<receptores.get_size();i++){  //O(R) -> O(4n) ; R = cantidad de receptores distintos (4n, pues puede haber un receptor por cada transferencia)
                    CircularArray<Transferencia*> *arr = new CircularArray<Transferencia*>();
                    for(long long unsigned int j=0;j<this->chain.get_size();j++){ //O(n)
                        for(long long unsigned int k=0;k<this->chain[j]->transferencias.size();k++){    //O(4)
                            if(this->chain[j]->transferencias[k]->receptor == receptores[i]){
                                arr->push_back(this->chain[j]->transferencias[k]);
                            }
                        }
                    }
                    hashReceptor->set(receptores[i],arr);
                }
                //2. get O(1)
                resultados = hashReceptor->get(key);
            }
            return resultados;
        }
        else if(atributo == "monto"){
            int monto = stoi(key);
            CircularArray<Transferencia*> *resultados;
            if(hashMonto->getSize() != 0){
                //get O(1)
                cout << "Búsqueda con O(1)" << endl;
                resultados = hashMonto->get(monto);
            }
            else{ //si no hay datos en el hashtable
                //n = cantidad de bloques del blockchain
                cout << "Búsqueda con O(n^2)" << endl;
                //O(n^2)
                CircularArray<int> montos;
                for(long long unsigned int i=0;i<this->chain.get_size();i++){ //O(n)
                    // añadir solo los emisores que no esten en la lista
                    for(long long unsigned int j=0;j<this->chain[i]->transferencias.get_size();j++){ //O(4)
                        if(!montos.contains(this->chain[i]->transferencias[j]->monto)){  //O(M) -> O(4n) ; M = cantidad de montos distintos (4n, pues puede haber un monto por cada transferencia)
                            montos.push_back(this->chain[i]->transferencias[j]->monto); 
                        }
                    }
                }
                //O(n^2)
                for(long long unsigned int i=0;i<montos.get_size();i++){  //O(M) -> O(4n) ; M = cantidad de montos distintos (4n, pues puede haber un monto por cada transferencia)
                    CircularArray<Transferencia*> *arr = new CircularArray<Transferencia*>();
                    for(long long unsigned int j=0;j<this->chain.get_size();j++){ //O(n)
                        for(long long unsigned int k=0;k<this->chain[j]->transferencias.size();k++){    //O(4)
                            if(this->chain[j]->transferencias[k]->monto == montos[i]){
                                arr->push_back(this->chain[j]->transferencias[k]);
                            }
                        }
                    }
                    hashMonto->set(montos[i],arr);
                }
                //2. get O(1)
                resultados = hashMonto->get(monto);
            }
            return resultados;
        }
        else{
            return nullptr;
        }
    }


    //Maximo monto entre todas las transferencias
    Transferencia* searchMax(string atributo="monto"){
        if(atributo == "monto"){
            if(max.get_size()==0){
                //construimos el heap
                for(int i=0;i<chain.get_size();i++){ //cada bloque tiene dentro transferencias
                    for(int j=0;j<chain[i]->transferencias.get_size();j++){
                        max.push(chain[i]->transferencias[j]->monto);
                    }
                }
            }
             //Se busca el monto más alto
            int maximo = max.top();
            // Busca el bloque que contiene la transferencia con el monto más alto
            for(int i=0;i<chain.get_size();i++){
                for(int j=0;j<chain[i]->transferencias.get_size();j++){
                    if(chain[i]->transferencias[j]->monto == maximo){
                        return chain[i]->transferencias[j];
                    }
                }
            }      
        }
        else if(atributo=="fecha"){
            if(maxFecha.get_size()==0){
                //construimos el heap
                for(int i=0;i<chain.get_size();i++){ //cada bloque tiene dentro transferencias
                    for(int j=0;j<chain[i]->transferencias.get_size();j++){
                        maxFecha.push(chain[i]->transferencias[j]->fecha);
                    }
                }
            }
             //Se busca el monto más alto
            Fecha maximo = maxFecha.top();
            // Busca el bloque que contiene la transferencia con el monto más alto
            for(int i=0;i<chain.get_size();i++){
                for(int j=0;j<chain[i]->transferencias.get_size();j++){
                    if(chain[i]->transferencias[j]->fecha == maximo){
                        return chain[i]->transferencias[j];
                    }
                }
            }
           
        }
        return nullptr;
    }

    //Maximo monto entre todas las transferencias
    Transferencia* searchMin(string atributo="monto"){
        if(atributo=="monto"){
            if(min.get_size()==0){
                //construimos el heap
                for(int i=0;i<chain.get_size();i++){ //cada bloque tiene dentro transferencias
                    for(int j=0;j<chain[i]->transferencias.get_size();j++){
                        min.push(chain[i]->transferencias[j]->monto);
                    }
                }
                
            }
            //Se busca el monto más alto
            int minimo = min.top();
            // Busca el bloque que contiene la transferencia con el monto más alto
            for(int i=0;i<chain.get_size();i++){
                for(int j=0;j<chain[i]->transferencias.get_size();j++){
                    if(chain[i]->transferencias[j]->monto == minimo){
                        return chain[i]->transferencias[j];
                    }
                }
            }      
        }
        else if(atributo=="fecha"){
            if(minFecha.get_size()==0){
                //construimos el heap
                for(int i=0;i<chain.get_size();i++){ //cada bloque tiene dentro transferencias
                    for(int j=0;j<chain[i]->transferencias.get_size();j++){
                        minFecha.push(chain[i]->transferencias[j]->fecha);
                    }
                }
            }
             //Se busca el monto más alto
            Fecha minimo = minFecha.top();
            // Busca el bloque que contiene la transferencia con el monto más alto
            for(int i=0;i<chain.get_size();i++){
                for(int j=0;j<chain[i]->transferencias.get_size();j++){
                    if(chain[i]->transferencias[j]->fecha == minimo){
                        return chain[i]->transferencias[j];
                    }
                }
            }
        }
        return nullptr;
    }

};