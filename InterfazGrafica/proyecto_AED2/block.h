#include <iostream>
#include <ctime>
//#include "sha256.cpp"
#include "sha256.h"
#include "circularList.h"
#include "fecha.h"

using namespace std;

struct Transferencia{
    int monto;
    string emisor;
    string receptor;
    Fecha fecha;
 
    // Constructores
    Transferencia(): emisor(""), receptor(""), monto(0), fecha(Fecha()){}
    Transferencia(Fecha fecha, string emisor, string receptor, int monto): emisor(emisor), receptor(receptor), monto(monto),fecha(fecha){}
    
    // Destructor
    ~Transferencia(){}

    // sobrecarga de ==
    bool operator==(const Transferencia& other) const{
        return (fecha == other.fecha && emisor == other.emisor && receptor == other.receptor && monto == other.monto);
    }
};

class Block{
public:
    string PrevBlockHash;   // punteros a string de Hash para que cuando se cambie, afecte también al puntero del bloque siguiente
    string Hash;            // punteros a string de Hash para que cuando se cambie, afecte también al puntero del bloque siguiente
    size_t nonce;
    CircularList<Transferencia*> transferencias;
    
    // Constructor
    Block(): PrevBlockHash(""), nonce(0){}

    // Destructor
    ~Block(){
        transferencias.clear();
    }

    // void cambiarEmisor(string nuevosEmisores){
    //     string prevHashSiguienBloque = prevHashsiguienteBloque(this);
    //     //cambio los emisores -> cambia Hash
    //     if(Hash!=prevHashSiguienBloque){
            
    //     }

    // }

    void setHash(){ // genera el Hashcode a partir de sus atributos
        string info;
        for(int i=0;i<transferencias.size();i++){
            info += transferencias[i]->fecha.getString();
            info += transferencias[i]->emisor;
            info += transferencias[i]->receptor;
            info += std::to_string(transferencias[i]->monto);
        }
        // Agregar prevHash al info para generar el hash para actualizarlo en el siguiente.
        // punteros de hashes


        // info += std::to_string(nonce);
        SHA256 sha256;
        sha256.add(info.c_str(), info.size());
        Hash = sha256.getHash();
    }

    void addTransferencia(Transferencia* transferencia){
        transferencias.push_back(transferencia);
    }

    void mine(){
        while(Hash.substr(0,4)!="0000"){ //O(4) -> O(1)
            setHash();
            // falta que nonce cambie
        }
                
    }
};

// class Block_transferencias:public Block
// {
//    public:
//     string emisor;
//     string receptor;
//     int monto;
 
//     Block_transferencias():Block(){
//         cout << "Se creo un bloque de transferencias" << endl;
//         emisor = "";
//         receptor = "";
//         monto = 0;
//         setHash(); //genera hashcode a partir de sus atributos
//     }

//     Block_transferencias(string fecha, string emisor, string receptor, int monto):Block(fecha), emisor(emisor), receptor(receptor), monto(monto){
//         setHash(); //genera hashcode a partir de sus atributos
//     }
//     ~Block_transferencias(){}

//     void setHash(){ //genera hashcode a partir de sus atributos
//         string info = fecha;
//         info += std::to_string(nonce);
//         info += emisor;
//         info += receptor;
//         info += std::to_string(monto);
//         SHA256 sha256;
//         sha256.add(info.c_str(), info.size());
//         Hash = sha256.getHash();
//     }

//     //sobrecarga de ==
//     bool operator==(const Block_transferencias& other) const{
//         return (fecha == other.fecha && emisor == other.emisor && receptor == other.receptor && monto == other.monto);
//     }
// };




// struct Block_algo: public Block
// {
//     string fecha;
//     string hora;
//     string data;
//     size_t PrevBlockHash;
//     size_t Hash;

//     void setHash(){ //genera hashcode a partir de sus atributos
//         string info = fecha;
//         info += hora;
//         info += data;
//         info += to_string(PrevBlockHash);
//         hash<string> hash_fn; // creo objeto de tipo hash
//         Hash = hash_fn(info); // guardo el hashcode en el atributo Hash
//     }
// };

// // funciones auxiliares
// Block newBlock(string data, size_t PrevBlockHash){ //Crea nuevo bloque
//     Block block;
//     //fecha usa objeto de tipo tm para obtener la fecha y hora actual
//     time_t now = time(0);
//     tm* time = localtime(&now);
//     string dia = to_string(time->tm_mday);
//     string mes = to_string(time->tm_mon+1);
//     string year = to_string(1900+time->tm_year);
//     block.fecha = dia+'/'+mes+'/'+year; //qui habría que poner la fecha actual
//     block.hora = to_string(time->tm_hour)+':'+to_string(time->tm_min)+':'+to_string(time->tm_sec);
//     block.data = data;
//     block.PrevBlockHash = PrevBlockHash;
//     block.setHash();
//     return block;
// }
