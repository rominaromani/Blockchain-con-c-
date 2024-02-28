#ifndef CHAINHASH_H
#define CHAINHASH_H

#include "forwardList.h"
#include <iostream>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.5;

template<typename TK, typename TV>
class ChainHash {
private:    
    class Entry{
    public:
		TK key;
		TV value;
		size_t hashcode;	

        Entry(TK key, TV value, size_t hashcode){
            this->key = key;
            this->value = value;
            this->hashcode = hashcode;
        }
        Entry(){

        }

        ~Entry(){
            
        }
    };  
	Forward_list<Entry> *array; //array de forward_lists
	int capacity;//tamanio del array
    int size;//cantidad de elementos totales
    int collisions;

public:
    ChainHash(){
		// TODO: asignar un tamanio inicial al array
		capacity = 10; 
		array = new Forward_list<Entry>[capacity];
		size = 0;
        collisions=0;
	}

	void set(TK key, TV value){
		if(fillFactor() >= maxFillFactor){
            rehashing();
        }
        size_t hashcode = getHashCode(key);
		int index = hashcode % capacity;
        //Verifico que no se hayan excedido las colisiones
        if(array[index].getColisiones()>=maxColision){
            //cout << "rehashing" << endl;
            rehashing();
        }  
        int tamanioLista = array[index].size();
		//caso en el que set tiene que actualizar
        //Entry entrada = Entry(key, value,hashcode);
        if(!seEncuentra(key)){
            //Entry entrada = Entry(key, value,hashcode);
            // for(int i=0;i<value.size();i++){
            //     cout << "value[i]: " << value[i] << endl;
            // }
            array[index].push_front(Entry(key,value,hashcode));
            size++;
        }
        else{
            remove(key);
            array[index].push_front(Entry(key, value, hashcode));
        }
        
        //Verifico si hubo colisión
        if(array[index].size()!=tamanioLista)
            array[index].agregarColision();

	}

	TV get(TK key){
        size_t hashcode = getHashCode(key);
        int index = hashcode % capacity;
        bool encontrado=false;
        TV value;
        //Iteramos la lista de colisiones para buscar la key
        for(auto it=array[index].begin();it!=array[index].end();++it){
            if(key==(it.ptr->data.key)){
                value = it.ptr->data.value;
                encontrado = true;
            }
        }
        if(encontrado==false)
            return nullptr;

        return value;
    }

    bool seEncuentra(TK key){
        size_t hashcode = getHashCode(key);
        int index = hashcode % capacity;
        TV value;
        //Iteramos para buscar la key
        for(auto it=array[index].begin();it!=array[index].end();++it){
            if(key==(it.ptr->data.key)) //se deben comparar los objetos, no los punteros. Por eso coloco *key y *(it.ptr->data.key)
                return true;
        }
        return false;
    }

	void remove(TK key){
        size_t hashcode = getHashCode(key);
        int index = hashcode % capacity;
        int cont=0;
        //Iteramos para buscar la key
        for(auto it=array[index].begin();it!=array[index].end();++it){
            if(key==it.ptr->data.key){
                array[index].remove(cont);
            }
            cont++;
        }
        size--;
    }

	//TODO +1 en el parcial: implementar el operador corchete [ ], read/write

    int bucket_count(){
        return capacity;
    }

    int bucket_size(int posList){
        if(posList>=capacity){
            cout << "posición de lista inválida"<<endl;
        }
        return array[posList].size();
    }

    Forward_list_iterator<Entry> begin(int pos){
        return array[pos].begin();
    }

    Forward_list_iterator<Entry> end(int pos){
        return array[pos].end();
    }

    
	double fillFactor(){
        double fillfactor = size / (capacity * maxColision);
		return fillfactor;
	}

    int getSize(){
        return size;
    }

private:
	size_t getHashCode(TK key){
		std::hash<TK> ptr_hash;
		return ptr_hash(key);
	}

	void rehashing(){
        Forward_list<Entry>*array2 = new Forward_list<Entry>[capacity*2];
        for(int i = 0; i<capacity;i++){
            for(auto it = array[i].begin(); it != array[i].end(); ++it){
                int index = it.ptr->data.hashcode % (capacity*2);
                array2[index].push_front(Entry(it.ptr->data.key, it.ptr->data.value,it.ptr->data.hashcode));
            }        
    }
        this->capacity *= 2;
        delete[] array;
        array = array2;
    }
public:
    class Proxy {
        ChainHash<TK, TV> *hash;
        TK key;

    public:
        Proxy(ChainHash<TK, TV> *hash, TK key) : hash(hash), key(key) {}

        Proxy &operator=(TV value) {
            hash->set(key, value);
            return *this;
        }

        operator const TV&() {
            return hash->get(key);
        }

        //overload conversion for ostream
        friend ostream &operator<<(ostream &os, const Proxy &p) {
            os << p.hash->get(p.key);
            return os;
        }

    };

    Proxy operator[](TK key) {
        return Proxy(this, key);
    }
};

#endif