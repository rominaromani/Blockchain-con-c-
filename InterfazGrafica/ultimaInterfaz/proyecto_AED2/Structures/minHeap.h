#ifndef MINHEAP_H
#define MINHEAP_H

#include <iostream>
using namespace std;

template <class T>
class MinHeap{
private:
    T* elements;
    int capacity;
    int size;
    
public:
    MinHeap(){
        this->capacity = 10;
        this->size = 0;
        this->elements = new T[capacity];
    }

    MinHeap(T* elements, int _n){
        this->elements = new T[_n];
        for(int i=0;i<_n;i++)
            this->elements[i] = elements[i];
        this->capacity = _n;
        this->size = _n;
        buildHeap();
    }

    T top(){
        return elements[0];
    }

    void push(T value){
        if(size == capacity)
            resize();
        elements[size++] = value;
        heapify_up(size-1);
    }

    void pop(){
        if(size==0)
            return;

        swap(elements[0],elements[size-1]);
        size--; 
        heapify_down(0);
    }

    void remove(T dato){
        int pos = 0;
        T *nuevo = new T[this->capacity];
        for (int i=0; i<size; i++){
            if (elements[i] == dato){
                pos = i;
                break;
            }
            nuevo[i] = elements[i];
        }
        for (int i=pos; i<size; i++){
            nuevo[i] = elements[i+1];
        }
        delete[] elements;
        elements = nuevo;
        size--;
    }

    int get_size(){
        return size;
    }
    
    ~MinHeap(){
        delete[] elements;
    }

private:
    void buildHeap(){
        for(int i=size/2; i>=0; --i){
            heapify_down(i);
        }
    }

    int left(int index){
        return 2*index +1;
    }

    int right(int index){
        return 2*index+2;
    }

    int parent(int index){
        return (index-1)/2;
    }

    void heapify_down(int parent){
        int l = left(parent);
        int r = right(parent);
        int largest = parent;
        if(l < size && elements[l] < elements[largest])
            largest = l;
        if(r < size && elements[r] < elements[largest])
            largest = r;
        if(largest != parent){
            swap(elements[parent], elements[largest]);
            heapify_down(largest);
        }
    }

    void heapify_up(int child){
        int hijo = child;
        while(elements[parent(hijo)] > elements[hijo]){
            int pos = parent(hijo);
            swap(elements[parent(hijo)], elements[hijo]);
            hijo = pos;
        }
    }

    void resize(){
        T *nuevo = new T[2*this->capacity];
        for(int i=0;i<size;i++){
            nuevo[i] = elements[i];
        }
        delete[] elements;
        elements = nuevo;
        this->capacity *= 2;
    }
};

#endif