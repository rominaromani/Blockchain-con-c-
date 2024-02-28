#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <iostream> 
//#include "circulararray.h"
//#include "block.h"
#include <string>
#include "Structures/chainhash.h"
#include "Structures/maxHeap.h"
#include "Structures/minHeap.h"
#include "Structures/simpletrie.h"
//#include "funciones.cpp"
#include "Structures/avl.h"
#include <vector>

using namespace std;



template<typename T>
struct Blockchain{
    unsigned int countBlock = 0;

    CircularArray<T> chain; // Vector de bloques
    CircularArray<Transferencia*> transferencias; // Vector de transferencias
    ChainHash<string, CircularArray<Transferencia*>*> *hashFecha; // Hash
    ChainHash<string, CircularArray<Transferencia*>*> *hashEmisor; // Hash
    ChainHash<string, CircularArray<Transferencia*>*> *hashReceptor; // Hash
    ChainHash<int, CircularArray<Transferencia*>*> *hashMonto; // Hash

    MaxHeap<int> max; // Heap
    MaxHeap<Fecha> maxFecha; // Heap
    MinHeap<Fecha> minFecha; // Heap
    MinHeap<int> min; // Heap

    TrieSimple *trieEmisor; // Trie
    TrieSimple *trieReceptor; // Trie

    AVLTree<int> *avlMonto; // AVL
    AVLTree<Fecha> *avlFecha; // AVL

public:


    Blockchain(){
        string *prev = new string("0000000000000000000000000000000000000000000000000000000000000000");
        chain.push_back(new Block()); //se crea el primer bloque de la cadena (puntero a bloque)
        chain[0]->PrevBlockHash = prev;
        chain[0]->setHash();
        chain[0]->mine();
        countBlock++;

        transferencias = CircularArray<Transferencia*>(); // Vector de transferencias

        hashFecha = new ChainHash<string, CircularArray<Transferencia*>*>(); // Hash
        hashEmisor = new ChainHash<string, CircularArray<Transferencia*>*>(); // Hash
        hashReceptor = new ChainHash<string, CircularArray<Transferencia*>*>(); // Hash
        hashMonto = new ChainHash<int, CircularArray<Transferencia*>*>(); // Hash

        trieEmisor = new TrieSimple(); // Trie
        trieReceptor = new TrieSimple(); // Trie

        avlMonto = new AVLTree<int>(); // AVL
        avlFecha = new AVLTree<Fecha>(); // AVL
    }

    int* buildPositions(string patron){
        int* positions = new int[256];
        for(int i=0;i<256;i++){
            positions[i] = -1;
        }
        for(long unsigned int i=0;i<patron.size();i++){
            positions[(int)patron[i]] = i;
        }
        return positions;
    }

    bool testBoyerMoore(string texto, string patron){
        const int n = texto.length();
        const int m = patron.length();
        //map<char,int> positions = buildPositions(patron);
        int* positions = buildPositions(patron);
        // for(const auto data:positions){
        //     cout << data.first << ":";
        //     cout << data.second << endl;
        // }
        int index;
        vector<int> results;

        for(int i=0;i<=n-m;){
            int j=m-1;
            while(j>=0 && (patron[j]==texto[i+j])){
                j--;
            }
            if(j>=0){
                index = texto[i+m];
                i += (i+m < n) ? m-positions[index] : 1; // evitamos el bucle infinito
            }else{
                results.push_back(i);
                i=i+m;
            }
        }
        if(results.size()>0){
            //cout << "Se encontraron " << results.size() << " coincidencias" << endl;
            return true;
        }else{
            //cout << "No se encontraron coincidencias" << endl;
            return false;
        }
    }

    void setPrevAndHash(T block){
        block->PrevBlockHash = chain[(chain.get_size())-1]->get_Hash(); //asigna el hash del último bloque al nuevo bloque
        block->setHash();
    }

    bool addBlock(T bloque){
        bool test = ProofOfWork();
        cout << "Block " << countBlock << ": " << (test? "añadido": "no añadido (minar bloques)") << endl;
        countBlock++;
        chain.push_back(bloque); //Añadimos el bloque al final del BlockChain
        addToOperations(bloque);
        return test;
    }

    void addToOperations(T block){
        // all hash
        // cout << "add to operations" << endl;
        for (int i=0; i<block->transferencias.get_size(); i++){
        // Hash
            // fechas
            if (hashFecha->get(block->transferencias[i]->fecha.getString()) == nullptr){
                CircularArray<Transferencia*> *arr = new CircularArray<Transferencia*>();
                arr->push_back(block->transferencias[i]);
                hashFecha->set(block->transferencias[i]->fecha.getString(), arr);
            }
            else {
                hashFecha->get(block->transferencias[i]->fecha.getString())->push_back(block->transferencias[i]);
            }
            // emisor
            if (hashEmisor->get(block->transferencias[i]->emisor) == nullptr){
                CircularArray<Transferencia*> *arr = new CircularArray<Transferencia*>();
                arr->push_back(block->transferencias[i]);
                hashEmisor->set(block->transferencias[i]->emisor, arr);
            }
            else {
                hashEmisor->get(block->transferencias[i]->emisor)->push_back(block->transferencias[i]);
            }
            // receptor
            if (hashReceptor->get(block->transferencias[i]->receptor) == nullptr){
                CircularArray<Transferencia*> *arr = new CircularArray<Transferencia*>();
                arr->push_back(block->transferencias[i]);
                hashReceptor->set(block->transferencias[i]->receptor, arr);
            }
            else {
                hashReceptor->get(block->transferencias[i]->receptor)->push_back(block->transferencias[i]);
            }
            // monto
            if (hashMonto->get(block->transferencias[i]->monto) == nullptr){
                CircularArray<Transferencia*> *arr = new CircularArray<Transferencia*>();
                arr->push_back(block->transferencias[i]);
                hashMonto->set(block->transferencias[i]->monto, arr);
            }
            else {
                hashMonto->get(block->transferencias[i]->monto)->push_back(block->transferencias[i]);
            }

        // Tries
            trieEmisor->insert(block->transferencias[i]->emisor, block->transferencias[i]);
            trieReceptor->insert(block->transferencias[i]->receptor, block->transferencias[i]);

        // Heap
            max.push(block->transferencias[i]->monto);
            min.push(block->transferencias[i]->monto);
            maxFecha.push(block->transferencias[i]->fecha);
            minFecha.push(block->transferencias[i]->fecha);

        //AVL
            avlMonto->insert(block->transferencias[i]->monto, block->transferencias[i]);
            avlFecha->insert(block->transferencias[i]->fecha, block->transferencias[i]);
        }
    }

    void removeFromOperations(T block){
        for (int i=0; i<block->transferencias.get_size(); i++){
        // Hash
            // fechas
            if (hashFecha->get(block->transferencias[i]->fecha.getString()) != nullptr && (hashFecha->get(block->transferencias[i]->fecha.getString()))->get_size() == 1){
                hashFecha->remove(block->transferencias[i]->fecha.getString());
            }
            else if (hashFecha->get(block->transferencias[i]->fecha.getString()) != nullptr) {
                hashFecha->get(block->transferencias[i]->fecha.getString())->remove(block->transferencias[i]);
            }
            // emisor
            if (hashEmisor->get(block->transferencias[i]->emisor) != nullptr && hashEmisor->get(block->transferencias[i]->emisor)->get_size() == 1){
                hashEmisor->remove(block->transferencias[i]->emisor);
            }
            else if (hashEmisor->get(block->transferencias[i]->emisor) != nullptr){
                (hashEmisor->get(block->transferencias[i]->emisor))->remove(block->transferencias[i]);
            }
            // receptor
            if (hashReceptor->get(block->transferencias[i]->receptor) != nullptr && hashReceptor->get(block->transferencias[i]->receptor)->get_size() == 1){
                hashReceptor->remove(block->transferencias[i]->receptor);
            }
            else if (hashReceptor->get(block->transferencias[i]->receptor) != nullptr) {
                hashReceptor->get(block->transferencias[i]->receptor)->remove(block->transferencias[i]);
            }
            // monto
            if (hashMonto->get(block->transferencias[i]->monto) != nullptr && hashMonto->get(block->transferencias[i]->monto)->get_size() == 1){
                hashMonto->remove(block->transferencias[i]->monto);
            }
            else if (hashMonto->get(block->transferencias[i]->monto) != nullptr) {
                hashMonto->get(block->transferencias[i]->monto)->remove(block->transferencias[i]);
            }

        // Tries
            trieEmisor->remove(block->transferencias[i]->emisor);
            trieReceptor->remove(block->transferencias[i]->receptor);

        // Heap
            max.remove(block->transferencias[i]->monto);
            min.remove(block->transferencias[i]->monto);
            maxFecha.remove(block->transferencias[i]->fecha);
            minFecha.remove(block->transferencias[i]->fecha);

        //AVL
            avlMonto->remove(block->transferencias[i]->monto);
            avlFecha->remove(block->transferencias[i]->fecha);
        }
    }

    void newBlock(){ // añadir bloque de transacciones nuevo
        bool invalid = true;
        int cantidad = 0, count = 0;
        int day, month, year;
        string emisor, receptor, monto, fecha;
        cout << "--------------------------------------------------------" << endl; 
        do {
            cout << "Ingrese la cantidad de transacciones: ";
            cin >> cantidad;
        } while (cantidad < 1);

        Block *block = new Block();
        while(count < cantidad){
            cin.ignore();
            cout << "Ingrese emisor: ";
            // cin >> emisor;
            getline(cin, emisor);
            cout << "Ingrese receptor: ";
            // cin >> receptor;
            getline(cin, receptor);
            cout << "Ingrese monto [int]: ";
            getline(cin, monto);
            do { // fechas validas, excepto bisiesto
                invalid = true;
                cout << "Ingrese fecha (dd/mm/aaaa): ";
                cin >> fecha;
            cin.ignore();
                for (int i=0; i<fecha.size(); i++){
                    if (fecha.size() != 10)
                        break;
                    if (fecha[i]-'0' != '0' && i == 2 || i == 4)
                        break;
                    month = ((fecha[3]-'0')*10)+((fecha[4])-'0');
                    day = ((fecha[0]-'0')*10)+((fecha[1])-'0');
                    if (month > 12 || month < 0)
                        break;
                    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
                        if (day > 31)
                            break;
                    }
                    else if (month == 2){
                        if (day > 28)
                            break;
                    }
                    else {
                        if (day > 30)
                            break;
                    }
                    invalid = false;
                }
            } while (invalid);
            cout << "-----------" << endl;
            Transferencia *transferencia = new Transferencia(fecha,emisor,receptor,stoi(monto));
            block->transferencias.push_back(transferencia);
            count++;
        }
        setPrevAndHash(block);
        addBlock(block);
        // addToOperations(block);
    }

    void mineBlocks(){ // minar bloques
        int pos=0;
        cout << "--------------------------------------------------------" << endl; 
        if (ProofOfWork() == false){
            cout << "Minar bloques" << endl;
            for (int i=0; i<chain.get_size(); i++){
                // cout << "block: " << i << "->" << chain[i]->validatedBlock() << endl;
                if (chain[i]->validatedBlock() == false && pos == 0){
                    pos = i;
                }
                if (chain[i]->validatedBlock() == false){
                    chain[i]->mine();
                }
            }
            cout << "Rango de bloques invalidos: [" << pos << "-" << chain.get_size()-1 << "]" << endl;
            
            for (int i=pos; i<=chain.get_size()-1; i++){
            cout << "--------------------------------------------------------" << endl;
            cout << "Bloque " << i << " updated" << endl;
            cout << "Cantidad de transferencias: " << chain[i]->transferencias.get_size() << endl;
            cout << "Nonce: " << chain[i]->nonce << endl;
            cout << "Hash: " << *chain[i]->get_Hash() << endl;
            cout << "PrevBlockHash: " << *chain[i]->PrevBlockHash << endl;
            cout << "Block valid: " << (chain[i]->validatedBlock() ? "valido" : "invalido") << endl;
        }
        cout << "------------------" << endl;
        }
        else{
            cout << "Todos los bloques son validos" << endl;
        }
    }

    void deleteBlock(){ // eliminar bloque
        int block;
        cout << "--------------------------------------------------------" << endl; 
        cout << "Range blocks: [0-" << chain.get_size()-1 << "]" << endl;
        do {
            cout << "Ingrese bloque a eliminar: ";
            cin >> block;
        } while(block < 1 || block >= chain.get_size()); // != 0, no se puede remover el bloque 0
        Block* temp = chain[block];
        removeFromOperations(temp);
    }

    void modifyBlock(){
        char type;
        int block;
        int dataNum;
        string data;
        cout << "--------------------------------------------------------" << endl; 
        cout << "Range blocks: [0-" << chain.get_size()-1 << "]" << endl;
        do {
            cout << "Ingrese bloque a modificar: ";
            cin >> block;
        } while(block <= 0 || block >= chain.get_size()); // no modifica el bloque 0
        removeFromOperations(chain[block]);
        cout << "------------------" << endl;
        cout << "Emisor [E]" << endl;
        cout << "Receptor [R]" << endl;
        cout << "Monto [M]" << endl;
        cout << "Fecha [F]" << endl;
        while (type != 'E' && type != 'R' && type != 'M' && type != 'F'){
            cout << "Tipo de dato a cambiar: ";
            cin >> type;
            type = toupper(type);
        }
        cout << "------------------" << endl;
        if (type == 'E'){
            for (int i=0; i<chain[block]->transferencias.get_size(); i++){
                cout << i << ": " << chain[block]->transferencias[i]->emisor << endl;
            }
            do {
                cout << "Ingrese el emisor a cambiar: ";
                cin >> dataNum;
            } while(dataNum < 0 || dataNum >= chain[block]->transferencias.get_size());
            cout << "Ingrese nuevo emisor: ";
            cin >> data;
            chain[block]->transferencias[dataNum]->emisor = data;
        }
        else if (type == 'R'){
            for (int i=0; i<chain[block]->transferencias.get_size(); i++){
                cout << i << ": " << chain[block]->transferencias[i]->receptor << endl;
            }
            do {
                cout << "Ingrese el receptor a cambiar: ";
                cin >> dataNum;
            } while(dataNum < 0 || dataNum >= chain[block]->transferencias.get_size());
            cout << "Ingrese nuevo receptor: ";
            cin >> data;
            chain[block]->transferencias[dataNum]->receptor = data;
        }
        else if (type == 'M'){
            for (int i=0; i<chain[block]->transferencias.get_size(); i++){
                cout << i << ": " << chain[block]->transferencias[i]->monto << endl;
            }
            do {
                cout << "Ingrese el monto a cambiar: ";
                cin >> dataNum;
            } while(dataNum < 0 || dataNum >= chain[block]->transferencias.get_size());
            cout << "Ingrese nuevo monto: ";
            cin >> data;
            chain[block]->transferencias[dataNum]->monto = stoi(data);
        }
        else if (type == 'F'){
            bool invalid=true;
            int day, month, year;
            for (int i=0; i<chain[block]->transferencias.get_size(); i++){
                cout << i << ": " << chain[block]->transferencias[i]->fecha.getString() << endl;
            }
            do {
                cout << "Ingrese la fecha a cambiar: ";
                cin >> dataNum;
            } while(dataNum < 0 || dataNum >= chain[block]->transferencias.get_size());
            string newFecha;
            do { // fechas validas, excepto bisiesto
                cout << "Ingrese nueva fecha (dd/mm/aaaa): ";
                cin >> newFecha;
                for (int i=0; i<newFecha.size(); i++){
                    if (newFecha.size() > 10)
                        break;
                    if (newFecha[i]-'0' != '0' && i == 2 || i == 4)
                        break;
                    month = ((newFecha[3]-'0')*10)+((newFecha[4])-'0');
                    day = ((newFecha[0]-'0')*10)+((newFecha[1])-'0');
                    if (month > 12 || month < 0)
                        break;
                    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12){
                        if (day > 31)
                            break;
                    }
                    else if (month == 2){
                        if (day > 28)
                            break;
                    }
                    else {
                        if (day > 30)
                            break;
                    }
                    invalid = false;
                }
            } while (invalid);
            Fecha *fechaTemp = new Fecha(newFecha);
            chain[block]->transferencias[dataNum]->fecha.setFecha(newFecha);
            delete fechaTemp;
        }
        cout << "--------------------------------------------------------" << endl;
        cout << "Bloque " << block << " updated" << endl;
        cout << "Cantidad de transferencias: " << chain[block]->transferencias.get_size() << endl;
        cout << "Nonce: " << chain[block]->nonce << endl;
        cout << "Hash: " << *chain[block]->get_Hash() << endl;
        cout << "PrevBlockHash: " << *chain[block]->PrevBlockHash << endl;
        cout << "------------------" << endl;
        for(int j=0; j<chain[block]->transferencias.get_size(); j++){
            cout << "Bloque " << block << " updated" << endl;
            chain[block]->transferencias[j]->display();
            cout << "------------------" << endl;
        }
        cout << "Block valid: " << (chain[block]->validatedBlock() ? "valido" : "invalido") << endl;
        cout << "--------------------------------------------------------" << endl;
        addToOperations(chain[block]);
    }

    void printBlocks(){
        int begin, end;
        cout << "--------------------------------------------------------" << endl;
        cout << "Range blocks: [0-" << chain.get_size()-1 << "]" << endl;
        cout << "Ingrese el rango de bloques: " << endl;
        do {
            cout << "Inicio: ";
            cin >> begin;
        } while (begin < 0 || begin >= chain.get_size());
        do {
            cout << "Fin: ";
            cin >> end;
        } while(end < begin || end >= chain.get_size());
        for (int i = begin; i <= end; i++){
            cout << "Bloque " << i << endl;
            cout << "Cantidad de transferencias: " << chain[i]->transferencias.get_size() << endl;
            cout << "Nonce: " << chain[i]->nonce << endl;
            cout << "Hash: " << *chain[i]->get_Hash() << endl;
            cout << "PrevBlockHash: " << *chain[i]->PrevBlockHash << endl;
            cout << "------------------" << endl;
            for(int j=0; j<chain[i]->transferencias.get_size(); j++){
                cout << "Bloque " << i << endl;
                chain[i]->transferencias[j]->display();
                cout << "------------------" << endl;
            }
            cout << "Block valid: " << (chain[i]->validatedBlock() ? "valido" : "invalido") << endl;
            cout << "--------------------------------------------------------" << endl;
        }
    }

private:
    bool ProofOfWork(){ // (private)
        for (int i=1; i<chain.get_size(); ++i){
            chain[i]->setHash(); // Recalcula el Hash para validar
            if (!chain[i]->validatedBlock()) {
                return false;
            }
        }
        return true;
    }

public:
    CircularArray<Transferencia*>* searchIgualdad(string atributo, string key){
        if(atributo == "fecha"){
            CircularArray<Transferencia*> *resultados;
            // int cantidadFechas = 0;
            // for (int i=0; i<chain.get_size(); i++){
            //     cantidadFechas += chain[i]->transferencias.get_size();
            // }
            // cout << "hash fecha: " << hashFecha->getSize() << endl;
            // cout << cantidadFechas << endl;
            if (hashFecha->getSize() != 0){ //get (O(1))
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
                cout << "hashEmisor->getSize(): " << hashEmisor->getSize() << endl;
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
             //Se busca la fecha más reciente
            Fecha maximo = maxFecha.top();
            // Busca el bloque que contiene la transferencia con la fecha más reciente
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


    CircularArray<Transferencia*> searchStartWith(string atributo, string prefijo){
        CircularArray<Transferencia*> resultados;
        if(atributo=="emisor"){
            //1. Inserción de palabras O(mn) -> O(n)
            if(trieEmisor->get_size()==0){ //solo si no se ha construido el trie
                cout << "Búsqueda O(n)" << endl;
                for(int i=0;i<chain.get_size();i++){ //O(n)
                    for(int j=0;j<chain[i]->transferencias.get_size();j++){ //O(4)
                        trieEmisor->insert(chain[i]->transferencias[j]->emisor,chain[i]->transferencias[j]); //O(m), siendo m el tamaño de la palabra
                    }
                }
            }
            else
                cout << "Búsqueda O(1)" << endl;
            
            //2. Busqueda O(m), siendo m el tamaño de la palabra más larga con ese prefijo
            return trieEmisor->getWordsWithPrefix(prefijo); //O(m), siendo m el tamaño de la palabra más larga con ese prefijo
        }
        else if(atributo=="receptor"){
            //1. Inserción de palabras O(mn) -> O(n)
            if(trieReceptor->get_size()==0){ //solo si no se ha construido el trie
                cout << "Búsqueda O(n)" << endl;
                for(int i=0;i<chain.get_size();i++){ //O(n)
                    for(int j=0;j<chain[i]->transferencias.get_size();j++){ //O(4)
                        trieReceptor->insert(chain[i]->transferencias[j]->receptor,chain[i]->transferencias[j]); //O(m), siendo m el tamaño de la palabra
                    }
                }
            }
            else
                cout << "Búsqueda O(m)" << endl;

            //2. Busqueda O(m), siendo m el tamaño de la palabra más larga con ese prefijo 
            return trieReceptor->getWordsWithPrefix(prefijo); 
        }

        return resultados;
    }

    CircularArray<Transferencia*> searchContains(string pattern,string atributo){
        CircularArray<Transferencia*> resultados;
        //copio todas las transferencias en un arreglo O(n)
        if(transferencias.get_size()==0){
            for(int i=0;i<chain.get_size();i++){ //cada bloque tiene dentro transferencias O(n)
                for(int j=0;j<chain[i]->transferencias.get_size();j++){ //O(4)
                    transferencias.push_back(chain[i]->transferencias[j]); //O(1)
                }
            }
        } //Podría hacerse en el constructor de la clase Blockchain

        if(atributo=="emisor"){
            //O(4n * E) -> O(n)
            for(int i=0;i<transferencias.get_size();i++){   //O(4n)
                // cout << "Comparando " << transferencias[i]->emisor << " con " << pattern << endl;
                //pasar todo a minusculas
                string emisor = transferencias[i]->emisor;
                string pat = pattern;
                for(int j=0;j<emisor.size();j++){ //O(E)
                    emisor[j] = tolower(emisor[j]);
                }
                for(int j=0;j<pat.size();j++){ //O(m)
                    pat[j] = tolower(pat[j]);
                }

                //O(E*m), pero tiende más a O(E) por los saltos. Nos decidimos por Boyer Moore porque almacenarlo en un preffix
                //tree no mejoraría el tiempo de búsqueda ya que habría que recorrer todo el árbol para encontrar el patrón
                if(testBoyerMoore(emisor,pat)){
                    resultados.push_back(transferencias[i]);
                }
            }
        }
        //O(4n * R) -> O(n)	
        else if(atributo=="receptor"){
            for(int i=0;i<transferencias.get_size();i++){
                string receptor = transferencias[i]->receptor;
                string pat = pattern;
                for(int j=0;j<receptor.size();j++){
                    receptor[j] = tolower(receptor[j]);
                }
                for(int j=0;j<pat.size();j++){
                    pat[j] = tolower(pat[j]);
                }
                //O(R*m), pero tiende más a O(R) por los saltos
                if(testBoyerMoore(receptor,pat)){
                    resultados.push_back(transferencias[i]);
                }
            }
        }

        return resultados;
    }

    //Monto total de un emisor o receptor
    int MontoTotal(string atributo,string key){
        int total = 0;
        CircularArray<Transferencia*> *resultados;
        
        if(atributo=="emisor"){
            if(hashEmisor->getSize() != 0){
            //get O(1)
                cout << "Búsqueda con O(1)" << endl;
                resultados = hashEmisor->get(key);
                for(int i=0;i<resultados->get_size();i++){
                    total += (*resultados)[i]->monto;
                }
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
                //get O(1)
                resultados = hashEmisor->get(key);
                for(int i=0;i<resultados->get_size();i++){
                    total += (*resultados)[i]->monto;
                }
                
            }
        }else if(atributo == "emisor"){
            if(hashReceptor->getSize() != 0){
            //get O(1)
                cout << "Búsqueda con O(1)" << endl;
                resultados = hashReceptor->get(key);
                
                for(int i=0;i<resultados->get_size();i++){
                    total += (*resultados)[i]->monto;
                }
            }
            else{ //si no hay datos en el hashtable
                //n = cantidad de bloques del blockchain
                cout << "Búsqueda con O(n^2)" << endl;
                //O(n^2)
                CircularArray<string> receptores;
                for(long long unsigned int i=0;i<this->chain.get_size();i++){ //O(n)
                    // añadir solo los emisores que no esten en la lista
                    for(long long unsigned int j=0;j<this->chain[i]->transferencias.get_size();j++){ //O(4)
                        if(!receptores.contains(this->chain[i]->transferencias[j]->receptor)){ //O(E) -> O(4n) ; E = cantidad de emisores distintos (4n, pues puede haber un emisor por cada transferencia
                            receptores.push_back(this->chain[i]->transferencias[j]->receptor);
                        }
                    }
                }
                //O(n^2)
                for(long long unsigned int i=0;i<receptores.get_size();i++){  //O(E) -> O(n) ; E = cantidad de emisores distintos (n, pues puede haber un emisor por cada transferencia)
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
                //get O(1)
                resultados = hashReceptor->get(key);
                for(int i=0;i<resultados->get_size();i++){
                    total += (*resultados)[i]->monto;
                }
            }
        }

        return total;
    }
    
    CircularArray<Transferencia*> searchRange(string atributo, string minimo, string maximo){
        CircularArray<Transferencia*> resultados;
        if(atributo=="monto"){
            //1. Si no se ha construido el AVL, se construye
            //O(4nlogn) -> O(nlogn
            if(avlMonto->get_size()==0){
                cout << "Búsqueda O(nlogn)" << endl;
                for(int i=0;i<chain.get_size();i++){ //O(n)
                    for(int j=0;j<chain[i]->transferencias.get_size();j++){ //O(4)
                        avlMonto->insert(chain[i]->transferencias[j]->monto,chain[i]->transferencias[j]); //O(log(n))
                    }
                }
            }
            else
                cout << "Búsqueda O(log(n))" << endl;
            //2. Se busca por rango
            return avlMonto->rangeSearch(stoi(minimo),stoi(maximo)); //O(log(n))
        }
        else if(atributo=="fecha"){
            //1. Si no se ha construido el AVL, se construye
            if(avlFecha->get_size()==0){
                cout << "Búsqueda O(nlogn)" << endl;
                for(int i=0;i<chain.get_size();i++){ //O(n)
                    for(int j=0;j<chain[i]->transferencias.get_size();j++){ //O(4)
                        avlFecha->insert(chain[i]->transferencias[j]->fecha,chain[i]->transferencias[j]); //O(log(n))
                    }
                }
            }
            else
                cout << "Búsqueda O(log(n))" << endl;
            //2. Se busca por rango
            Fecha fechaMinima = Fecha(minimo);
            cout << "Fecha minima: " << fechaMinima.getString() << endl;
            Fecha fechaMaxima = Fecha(maximo);
            cout << "Fecha maxima: " << fechaMaxima.getString() << endl;
            return avlFecha->rangeSearch(fechaMinima,fechaMaxima); //O(log(n))
        }
        return resultados; //si no se encontraron resultados devuelve array vacío
    }

};

#endif
