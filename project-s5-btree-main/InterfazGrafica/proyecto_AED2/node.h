
#ifndef NODE_H
#define NODE_H

template <typename T>
struct Node {
    T data;
    Node<T>* next;
    Node<T>* prev;

    Node(){ 
        // TODO
        next = prev = nullptr;
    }

    Node(T value){
        // TODO
        this->data = value;
        this->next = this->prev = nullptr;
    }

    void killSelf(){
        // TODO      
    }    
};

#endif
