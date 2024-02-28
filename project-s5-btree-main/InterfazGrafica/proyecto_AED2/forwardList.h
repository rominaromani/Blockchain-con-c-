#ifndef FORWARDLIST_H
#define FORWARDLIST_H

#include <iostream>
#include "list.h"

using namespace std;


//Clase iterador forward_list
template<class T>
class Forward_list_iterator{
public:
    Node<T>* ptr;
    Forward_list_iterator(Node<T>* p = nullptr){
        ptr = p;
    }
    Forward_list_iterator<T>& operator++(){
        ptr = ptr->next;
        return *this;
    }
    Forward_list_iterator<T> operator++(int){
        Forward_list_iterator<T> temp = *this;
        ptr = ptr->next;
        return temp;
    }
    T& operator*(){
        return ptr->data;
    }
    bool operator==(const Forward_list_iterator<T>& rhs){
        return ptr == rhs.ptr;
    }
    bool operator!=(const Forward_list_iterator<T>& rhs){
        return ptr != rhs.ptr;
    }
};

template<class T>
class Forward_list{
private:
    Node<T>* head;
    int nodes;
    int colisiones;
public:
    typedef Forward_list_iterator<T> iterator;
    iterator begin(){
        return iterator(head);
    }
    iterator end(){
        return iterator(nullptr);
    }
    
    Forward_list(){
        head = nullptr;
        nodes = 0;
        colisiones = 0;
    }
    ~Forward_list(){
        Node<T>* temp;
        while(head!=nullptr){
            temp = head;
            head = head->next;
            delete temp;
        }
    }
    T front(){ //Funciona
        try{
            if(empty())
                throw "La lista está vacía";
        }
        catch(const char* msg){
            cerr << msg << endl;
        }
        return head->data;
    }
    T back(){ //Funciona
        Node<T>* temp = head;
        while(temp->next!=nullptr){
            temp = temp->next;
        }
        return temp->data;
    }
    void push_front(T elm){ //Funciona
        Node<T>* nuevo_head = new Node<T>;
        nuevo_head->data = elm;
        nuevo_head->next = head;
        head = nuevo_head;
        nodes++;
    }
    void push_back(T elm){ //Funciona
        Node<T>* nuevo_nodo = new Node<T>;
        nuevo_nodo->data = elm;
        if(empty()){
            nuevo_nodo->next = nullptr;
            head = nuevo_nodo;
        }
        else{
            Node<T>* temp = head;
            while(temp->next!=nullptr){
                temp = temp->next;
            }
            temp->next = nuevo_nodo;
            nuevo_nodo->next = nullptr;
        }
        nodes++;
    }
    T pop_front(){ //Funciona
        T elm;
        try{
            if(empty())
                throw "La lista está vacía!";
            else if(head->next==nullptr){
                elm = head->data;
                delete head;
                head = nullptr;
            }
            else{
                elm = head->data;
                Node<T>* temp = head;
                head = head->next;
                delete temp;  
            }
        }
        catch(const char* msg){
            cerr << msg << endl;
        }
        return elm;
    }
    T pop_back(){ //funciona
        T elm;
        try{
            if(empty()){
                throw "La lista está vacía!";
            }
            else if(head->next==nullptr){
                elm = head->data;
                delete head;
                head = nullptr;
            }
            else{
                Node<T>* temp = head;
                while(temp->next->next!=nullptr){
                    temp = temp->next;
                }
                elm = temp->next->data;
                delete temp->next;
                temp->next = nullptr;
            }    
        }
        catch(const char* msg){
            cerr << msg << endl;
        }
        return elm;    
    }
    bool empty(){
        return head==nullptr;
    }
    void display(){
        if(empty())
            cout << "La lista está vacía!" << endl;
        else{
            Node<T>* temp = head;
            while(temp!=nullptr){
                cout << temp->data << ",";
                temp = temp->next;
            }
            cout << endl;
        }
    }
    int size(){
        return nodes;
    }
    void clear(){
        Node<T>* temp = new Node<T>;
        while(head!=nullptr){
            temp = head; //temp apunta a la dirección en donde está head
            head = head->next; //head apunta a al siguiente nodo, pero temp sigue apuntando al anterior
            delete temp; //se libera la memoria
        }
    }
    T& operator[](int pos){
        //if(pos<0){
        //    cout < "La posición está fuera de rango"<<endl;
        //}
        Node<T>* temp = head;
        try{
            if(pos<0){
                throw "La posición está fuera de rango";
            }
            for(int i=0;i<pos;i++){
                if(temp->next==nullptr)
                   throw "La posición está fuera de rango";
                temp = temp->next;
            }
            //return temp->data;
        }
        catch(const char* msg){
            cerr << msg << endl;
        }
        return temp->data;        
    }

    void sort(){
        //bubble sort con listas
        if(!empty()){
            Node<T>* actual , *siguiente = new Node<T>;
            actual = head;
            T t;
        
            while(actual->next != NULL){
            siguiente = actual->next; //Siguiente apunta al elemento que va después del actual
            
            while(siguiente!=NULL){
                if(actual->data > siguiente->data){
                        //Swap de la data de cada nodo
                        t = siguiente->data;            
                        siguiente->data = actual->data;
                        actual->data = t;          
                }
                siguiente = siguiente->next;                    
            }    
            actual = actual->next; //actual apunta al siguiente elemento
            }    
        }
        else
            cout << "La lista está vacía!"<<endl;
    }

    void reverse(){
        if(!empty()){
            Node<T>* temp = new Node<T>;
            Forward_list *lista2 = new Forward_list();
            while(head!=nullptr){
                lista2->push_front(head->data);
                temp = head;
                head = head->next;
                delete temp;
            }
            head = lista2->head;
        }
        else
            cout << "La lista está vacía!" << endl;
    }

    Node<T>* get_head(){
        return head;
    }

    bool remove(int pos){
            bool exito = true;
            try{
                if(pos<0 || pos>=nodes){
                    exito = false;
                    throw "Posición fuera de rango";
                }
                else if(empty()){
                    exito = false;
                    throw "No hay elementos para eliminar";
                }
                else if(pos==0){
                    pop_front();
                }
                else if(pos==nodes-1){
                    pop_back();
                }
                else{
                    Node<T>* temp = head;
                    for(int i=0;i<pos-1;i++){
                        temp = temp->next;
                    }
                    Node<T>* temp2 = temp->next;
                    temp->next = temp->next->next;
                    delete temp2;
                }
            }
            catch(const char* msg){
                cerr << msg << endl;
            }
            nodes--;
            return exito;
        }

    void agregarColision(){
        colisiones++;
    }
    int getColisiones(){
        return colisiones;
    }
};

#endif