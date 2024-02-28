#ifndef BLOCK_H
#define BLOCK_H

// #include <iostream>
// #include <ctime>
// #include "circularList.h"
// #include "Structures/sha256.cpp"
// #include "Structures/sha256.h"
#include "Structures/fecha.h"

// using namespace std;

const unsigned int prefixSize = 3; // Cantidad de ceros como prefijo de validacion

struct Transferencia{
    int monto;
    string emisor;
    string receptor;
    Fecha fecha;
 
    // Constructores
    Transferencia(): emisor(""), receptor(""), monto(0), fecha(Fecha()){}
    Transferencia(Fecha fecha, string emisor, string receptor, int monto): fecha(fecha), emisor(emisor), receptor(receptor), monto(monto){}
    
    // Destructor
    ~Transferencia(){}

    // sobrecarga de ==
    bool operator==(const Transferencia& other) const{
        return (fecha == other.fecha && emisor == other.emisor && receptor == other.receptor && monto == other.monto);
    }

    void display(){
        cout << "Emisor: " << emisor << endl;
        cout << "Receptor: "  << receptor << endl;
        cout << "Monto: " << monto << endl;
        cout << "Fecha: " << fecha.getString() << endl;
    }
};

class Block{
private:
    string prefixValidator; // Proof of work (private)

public:
    // int numBloque;
    size_t nonce;
    string *Hash;           // Huella // punteros a string de Hash para que cuando se cambie, afecte también al puntero del bloque siguiente
    string *PrevBlockHash;  // Huella del bloque anterior // punteros a string de Hash para que cuando se cambie, afecte también al puntero del bloque siguiente
    CircularList<Transferencia*> transferencias; // Informacion / data

    // Constructor
    Block(): nonce(0){
        this->PrevBlockHash = new string();
        this->Hash = new string();
        for (int i=0; i<prefixSize; ++i){
            prefixValidator += "0";
        }
        // this->numBloque = 0;
    }

    // Destructor
    ~Block(){
        transferencias.clear();
        delete Hash;
        delete PrevBlockHash;
    }   

    void setHash(){ // genera el Hashcode a partir de sus atributos
        string* info = new string();
        for(int i=0;i<transferencias.size();i++){
            *info += transferencias[i]->fecha.getString();
            *info += transferencias[i]->emisor;
            *info += transferencias[i]->receptor;
            *info += std::to_string(transferencias[i]->monto);
        }
        *info += std::to_string(nonce);
        *info += (*PrevBlockHash);
        SHA256 sha256;
        sha256.add(info->c_str(), info->size());
        *info = sha256.getHash();
        *Hash = *info;
        delete info;
    }

    void setHash(string info){ // genera el Hashcode a partir de sus atributos
        info += std::to_string(nonce);
        info += (*PrevBlockHash);
        SHA256 sha256;
        sha256.add(info.c_str(), info.size());
        info = sha256.getHash();
        *Hash = info;
    }
    
    string* get_Hash(){
        setHash();
        return Hash;
    }

    void addTransferencia(Transferencia* transferencia){
        transferencias.push_back(transferencia);
    }

    void mine(){
        string prefix;
        string info;
        for (int i=0; i<prefixSize; i++)
            prefix += (*Hash)[i];

        for(int i=0;i<transferencias.size();i++){
            info += transferencias[i]->fecha.getString();
            info += transferencias[i]->emisor;
            info += transferencias[i]->receptor;
            info += std::to_string(transferencias[i]->monto);
        }
        
        while (prefix != prefixValidator){
            nonce += 1;
            setHash(info);
            prefix = "";
            for (int i=0; i<prefixSize; i++)
                prefix += (*Hash)[i];
        }
    }

    bool validatedBlock(){
        string prefix;
        setHash();
        for (int i=0; i<prefixSize; i++)
            prefix += (*Hash)[i];
        return prefix == prefixValidator ? true: false;
    }
};

#endif