#ifndef CIRCULAR_H
#define CIRCULAR_H

#include <iostream>
#include "list.h"

template <class T>
class CircularListIterator {
private:
    Node<T>* current;
    int nodes;
    
public:
    CircularListIterator(){
        this->current = nullptr;
        this->nodes = 0;
    }

    CircularListIterator(Node<T>* iter, int nodes){
        this->current = iter;    
        this->nodes = nodes;
    }

    bool operator != (const CircularListIterator &other){
        return this->current != other.current;
    }

    bool operator == (const CircularListIterator &other){
        return this->current == other.current;
    }

    CircularListIterator& operator++(){
        this->current = this->current->next;
        return *this;
    }

    CircularListIterator& operator--(){
        this->current = this->current->prev;
        return *this;
    }
    
    T& operator*(){
        return this->current->data;
    }
};

// TODO: Implement all methods
template <typename T>
class CircularList : public List<T> {  
        //add for iterator
    public:
        typedef CircularListIterator<T> iterator;
        iterator begin(){
            return iterator(this->head->next, nodes);
        }
        iterator end(){
            return iterator(this->head, nodes);
        }

    private:
        Node<T>* head; //sentinel
        int nodes; 
        
    public:
        CircularList() : List<T>() { // generar sentinel
            Node<T>* nodo = new Node<T>;
            head = nodo;
            head->next = head->prev = head;
            nodes = 0;
        }

        ~CircularList(){ // eliminar sentinel
            // if(nodes == 0)
            //     return;
            clear();
            head = nullptr;
        }

        T front(){ // O(1)
            // throw ("sin definir");
            try{
                if (is_empty())
                    throw "Lista vacia";
            }
            catch (const char* msg){
                cerr << msg << endl;
            }

            return head->next->data;
        }

        T back(){ // O(1)
            // throw ("sin definir");
            try{
                if (is_empty())
                    throw "Lista vacia";
            }
            catch (const char* msg){
                cerr << msg << endl;
            }

            return head->prev->data;
        }

        void push_front(T data){ // O(1)
            // throw ("sin definir");
            Node<T>* nodo = new Node<T>;
            nodo->data = data;
            if (is_empty()){
                nodo->next = head;
                nodo->prev = head;
                head->next = nodo;
                head->prev = nodo;
            }
            else {
                nodo->next = head->next;
                nodo->prev = head;
                nodo->next->prev = nodo;
                head->next = nodo;
            }
            nodes++;
        }

        void push_back(T data){ // O(1)
            // throw ("sin definir");
            Node<T>* nodo = new Node<T>;
            nodo->data = data;
            if (is_empty()){
                nodo->next = head;
                nodo->prev = head;
                head->next = nodo;
                head->prev = nodo;
            }
            else {
                nodo->prev = head->prev;
                nodo->prev->next = nodo;
                nodo->next = head;
                head->prev = nodo;
            }
            nodes++;
        }

        bool contains(T data){ // O(n)
            // throw ("sin definir");
            Node<T>* nodo = new Node<T>;
            nodo = head->next;
            for (int i=0; i<nodes; i++){
                if (nodo->data == data)
                    return true;
                nodo = nodo->next;
            }
            return false;
        }

        T pop_front(){ // O(1)
            // // throw ("sin definir");

            Node<T>* temp = new Node<T>();
            temp = head->next; //apunta al único elemento
            head->next = temp->next;
            temp->next->prev = head;
            
            T data = temp->data;
        
            delete temp;
            nodes--;
            return data;
        }

        T pop_back(){ // O(1)
            // throw ("sin definir");
            try{
                if (is_empty())
                    throw "Lista vacia";
            }
            catch (const char* msg){
                cerr << msg << endl;
            }

            T data; 
            Node<T>* nodo = new Node<T>;
            nodo = head->prev;

            nodo->prev->next = head;
            head->prev = nodo->prev;
            data = nodo->data;
            delete nodo;
            nodo = nullptr;
            nodes--;
            return data;
        }

        void insert(T data, int pos){ // O(n)
            // throw ("sin definir");
            try{
                if (is_empty())
                    throw "Lista vacia";
            }
            catch (const char* msg){
                cerr << msg << endl;
            }
            try{
                if (pos > nodes || pos < 0)
                    throw "Posicion no valida";
            }
            catch (const char* msg){
                cerr << msg << endl;
            }

            Node<T>* nodo = new Node<T>;
            Node<T>* temp = new Node<T>;
            nodo->data = data;
            temp = head->next;

            if (pos == nodes){
                push_back(data);
            }
            else if (pos == 0){
                push_front(data);
            }
            else {
                for (int i=1; i<pos; i++){
                    temp = temp->next;
                }
                nodo->next = temp->next;
                temp->next = nodo;
            }
            nodes++;
        }

        bool remove(int pos){ // O(n)
            // throw ("sin definir");
            try{
                if (is_empty())
                    throw "Lista vacia";
            }
            catch (const char* msg){
                cerr << msg << endl;
            }
            try{
                if (nodes<pos)
                    throw "Posicion no valida";
            }
            catch (const char* msg){
                cerr << msg << endl;
            }
            
            Node<T>* nodo = new Node<T>;
            nodo = head->next;
            for (int i=0; i<pos; i++){
                nodo = nodo->next;
            }

            nodo->prev->next = nodo->next;
            nodo->next->prev = nodo->prev;

            delete nodo;
            nodo = nullptr;
            nodes--;
            return true;
        }

        T& operator[](int pos){
            // throw ("sin definir");
            try{
                if (pos > nodes || pos < 0)
                    throw "Posicion no valida";
            }
            catch (const char* msg){
                cerr << msg << endl;
            }
            Node<T>* nodo = new Node<T>;
            nodo = head->next;
            for (int i=0; i<pos; i++){
                nodo = nodo->next;
            }
            return nodo->data;
        }

        bool is_empty(){
            // throw ("sin definir");
            return head->next == head && head->prev == head;
        }

        int size(){
            // throw ("sin definir");
            return nodes;
        }

        void clear(){ // todos menos el sentinel
            // throw ("sin definir");
            if (is_empty())
                return;

            while (!is_empty()){
                pop_front();
            }
        }
        
        // void sort(){
        //     // throw ("sin definir");
        //     if (is_empty())
        //         return;

        //     Node<T>* nodo = new Node<T>;
        //     nodo = head->next;
            
        //     int n = 0;
        //     int size = CircularList<T>::size();
        //     T* array = new T[size];

        //     while (nodo != head){
        //         array[n++] = nodo->data;
        //         nodo = nodo->next;   
        //     }

        //     // Counting Sort
        //     //1. Hallar el tamaño de los arrays count y output
        //     int max = array[0];
        //     for (int i = 1; i < size; i++) {
        //         if (array[i] > max)
        //         max = array[i];
        //     }
            
        //     T* output = new T[size];
        //     int tamanioCount = max+1;
        //     T* count = new T[tamanioCount];
            
        //     //2. Inicializar con 0s todos los elementos de count
        //     for (int i = 0; i <= max; ++i) {
        //         count[i] = 0;
        //     }
            
        //     //3. Contar repeticiones de cada elemento del array
        //     for (int i = 0; i < size; i++) {
        //         count[array[i]]++;
        //     }
            
        //     //4. Se suman todos los elementos con el anterior
        //     for (int i = 1; i <= max; i++) {
        //         count[i] += count[i - 1];
        //     }
            
        //     //5. Se llena el array output
        //     for (int i = size - 1; i >= 0; i--) {
        //         output[count[array[i]] - 1] = array[i];
        //         count[array[i]]--;
        //     }
            
        //     //6. Se copia el array output al array original
        //     for (int i = 0; i < size; i++) {
        //         array[i] = output[i];
        //     }

        //     nodo = head->next;
        //     for (int i=0; i<size; i++){
        //         nodo->data = array[i];
        //         nodo = nodo->next;
        //     }

        //     // Node<T>* nodo1 = new Node<T>;
        //     // nodo1 = head->next;
        //     // while (nodo1 != head){
        //     //     cout << nodo1->data << " ";
        //     //     nodo1 = nodo1->next;
        //     // }
        //     // cout << endl;

        //     delete[] array;
        //     delete[] output;
        //     delete[] count;
        // }

        // bool is_sorted(){
        //     // throw ("sin definir");
        //     try{
        //         if (is_empty())
        //             throw "Lista vacia";
        //     }
        //     catch (const char* msg){
        //         cerr << msg << endl;
        //     }

        //     Node<T>* nodo = new Node<T>;
        //     nodo = head->next;

        //     while (nodo->next != head){
        //         // cout << nodo->data << " " << nodo->next->data << endl;
        //         if (nodo->data > nodo->next->data) {
        //             return false;
        //         }
        //         nodo = nodo->next;
        //     }
        //     return true;
        // }

        void reverse(){ // Terminar
            // throw ("sin definir");
            if (is_empty())
                return;
            
            int nodesT = nodes;
            Node<T>* temp = new Node<T>;
            Node<T>* back = new Node<T>;
            temp = head->next;
            back = head->next;
            while (temp != head){
                push_front(temp->data);
                temp = temp->next;
            }
            back = back->prev;
            while (head->prev != back){
                pop_back();
            }
            nodes = nodesT;
        }

        std::string name(){
            return "CircularList";
        }

        void display(){
            if(is_empty()){
                cout << "La lista está vacía!" << endl;
            }
                
            else{
                Node<T>* temp = head->next;
                while(temp!=head){
                    cout << temp->data << ",";
                    temp = temp->next;
                }
                cout << endl;
            }
        } 

        int get_size(){
            return nodes;
        }
};

#endif
