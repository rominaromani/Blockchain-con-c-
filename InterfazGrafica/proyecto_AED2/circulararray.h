#include <iostream>

using namespace std;

//void countSort(int *array, int size) {
 
//    //1. Hallar el tamaño de los arrays count y output
//    int max = array[0];
 
//    for (int i = 1; i < size; i++) {
//        if (array[i] > max)
//        max = array[i];
//    }

 
//  int output[size];
//  const int tamanioCount = max+1;
//  int count[tamanioCount];
 
//  //2. Inicializar con 0s todos los elementos de count
//  for (int i = 0; i <= max; ++i) {
//    count[i] = 0;
//  }
 
//  //3. Contar repeticiones de cada elemento del array
//  for (int i = 0; i < size; i++) {
//    count[array[i]]++;
//  }
 
//    //4. Se suman todos los elementos con el anterior
//    for (int i = 1; i <= max; i++) {
//        count[i] += count[i - 1];
//    }
 
//    //5. Se llena el array output
//    for (int i = size - 1; i >= 0; i--) {
//        output[count[array[i]] - 1] = array[i];
//        count[array[i]]--;
//    }
 
//  //6. Se copia el array output al array original
//  for (int i = 0; i < size; i++) {
//    array[i] = output[i];
//  }
//}


template <class T>
class CircularArray {
protected:
    T *array;
    int capacity;
    int back, front;
    int size;
    
public:
    CircularArray(int _capacity = 10){
        this->capacity = _capacity;
        this->array = new T[_capacity];
        this->front = this->back = -1;//empty
        this->size = 0;
    };
    ~CircularArray(){
        // cout << "Destructor de CircularArray" << endl;
        // cout << "size: " << size << endl;
        // cout << "capacity: " << capacity << endl;
        // cout << "front: " << front << endl;
        // cout << "back: " << back << endl;
        delete[] array;      //fallaba en función set de chainHash cuando se hacia un delete, pero cambie el arrayCircular que era el value a un puntero y ya no falla
        // array = nullptr;
        // cout << "termina de destruir circularArray" << endl;
    };
    void push_front(T data){
        if(is_full()){
            resize();
            front = prev(front);
        }
        else if(is_empty()){
            front=0;
            back=0;
        }
        else{
            front = prev(front);
        }
        array[front] = data;
        size++;
    }
    void push_back(T data){
        if(is_full()){
            resize();
            back = next(back);
        }
        else if(is_empty()){ //si está vacío front y back apuntan ahora a 0
            front=0;
            back=0;
        }
        else
            back = next(back);
        array[back] = data;
        size++;
    }
    void insert(T data, int pos){
        if(is_full())
            resize();
        else if(is_empty()){
            if(pos==0){
                push_back(data);
            }
            else{
                cout << "El índice está fuera de rango" << endl;
            }
        }
        else if(pos==capacity)
            push_back(data);
        else if(pos==0)
            push_front(data);
        else if(pos>capacity || pos<0)
            cout << "El índice está fuera de rango" << endl;
        else{
            int it=back;
            while(it!=prev((front+pos)%capacity)){ //empuja elementos necesarios a la derecha
                array[next(it)] = array[it];
                it = prev(it);
            }
            array[(front+pos)%capacity] = data; //coloca el elemento en la posición indicada
            size++;
            back++;
        }
    }
    T pop_front(){
        T eliminado;
        if(is_empty())
            cout << "No existen elementos para eliminar"<<endl;
        else{
            eliminado = array[front];
            front = next(front);
            size--;
            if(size==0)
                front = back = -1;
        }
        return eliminado;
    }
    T pop_back(){
        T eliminado;
        if(is_empty())
            cout << "No existen elementos para eliminar"<<endl;
        else{
            eliminado = array[back];
            back = prev(back);
            size--;
            if(size==0)
                front = back = -1;
        }
        return eliminado;
    }
    bool is_full(){
        return back==prev(front);
    }
    bool is_empty(){
        return (back==-1 && front==-1);
    }
    int get_size(){
        return size;
    }
    void clear(){
        while(!is_empty()){
            pop_back();
        }
    }
    
    T& operator[](int pos){
        if(pos>=0 && pos<capacity){
            return array[(front+pos)%capacity];
        }
        else{
            //cout << "El índice está fuera de rango" << endl;
            throw("El índice está fuera de rango");
        }
        
    }   
     
//    void sort(){
//        if(is_sorted()){
//            return;
//        }
//        else{
//            int ordenado[capacity];
//            int pos=0;
//            if(back>front){
//                for(int i=front;i<=back;i++){
//                    ordenado[pos++] = array[i];
//                }
//                //ordeno
//                countSort(ordenado,pos);
//                //Copio al array original
//                pos = 0;
//                for(int i=front;i<=back;i++){
//                    array[i] = ordenado[pos++];
//                }
//            }
//            else{
//                for(int i=front;i<capacity;i++){
//                    ordenado[pos++] = array[i];
//                }
//                //Imprime desde el primer elemento hasta el back
//                for(int i=0;i<=back;i++){
//                    ordenado[pos++] = array[i];
//                }
//                //ordeno
//                countSort(ordenado,pos);
//                //Copio al array original
//                pos = 0;
//                for(int i=front;i<capacity;i++){
//                    array[i] = ordenado[pos++];
//                }
//                for(int i=0;i<=back;i++){
//                    array[i] = ordenado[pos++];
//                }
//            }
//        }
//    }
    bool is_sorted(){
        if(is_empty())
            return true;
        else if(back>front){
            for(int i=front;i<=back;i++){
                if(array[i]>array[next(i)])
                    return false;   
            }
        }
        else{
            //Imprime desde front hasta el último elm.
            for(int i=front;i<capacity;i++){
                if(array[i]>array[next(i)])
                    return false;   
            }
            //Imprime desde el primer elemento hasta el back
            for(int i=0;i<=back;i++){
                if(array[i]>array[next(i)])
                    return false;  
            }
        }
        return true;
    }
    void reverse(){
        if(front<back){
            for(int i=front,j=back;i<j;i=next(i),j=prev(j)){
            swap(array[i],array[j]);
            }
        }
        else{
            for(int i=front,j=back;i>j;i=next(i),j=prev(j)){
            swap(array[i],array[j]);
            }
        }
        
    }

    void remove(T dato){
        if(is_empty())
            cout << "No existen elementos para eliminar"<<endl;
        else{
            int pos = find(dato);
            if(pos!=-1){
                int it=back;
                while(it!=prev((front+pos)%capacity)){ //empuja elementos necesarios a la izquierda
                    array[it] = array[next(it)];
                    it = prev(it);
                }
                array[back] = 0;
                back = prev(back);
                size--;
            }
            else
                cout << "El elemento no existe" << endl;
        }
    }

    int find(T dato){
        if(is_empty())
            return -1;
        else{
            if(back>front){
                for(int i=front;i<=back;i++){
                    if(array[i]==dato)
                        return i-front;   
                }
            }
            else{
                //Imprime desde front hasta el últim elm.
                for(int i=front;i<capacity;i++){
                    if(array[i]==dato)
                        return i-front;   
                }
                //Imprime desde el primer elemento hasta el back
                for(int i=0;i<=back;i++){
                    if(array[i]==dato)
                        return i+capacity-front;  
                }
            }
            return -1;
        }
    }

    string to_string(string sep=" "){
        string elementos;
        
        if(back>front){
            for(int i=front;i<=back;i++){
                elementos += std::to_string(array[i]) + sep;    
            }
        }
        else{
            //Imprime desde front hasta el últim elm.
            for(int i=front;i<capacity;i++){
                elementos += std::to_string(array[i]) + sep;    
            }
            //Imprime desde el primer elemento hasta el back
            for(int i=0;i<=back;i++){
                elementos +=  std::to_string(array[i]) + sep;
            }
        }
        return elementos;
    }
    void resize(){
        //Creo otro array con doble de capacidad
        T* temp = new T[capacity*2];
        int cont = 0;
        //Copio los elementos de mi array actual al nuevo
        for(int i = front; i != back; i = next(i)){
            temp[cont++] = array[i];
        }
        temp[cont] = array[back];
        //Librero memoria del original
        delete[] array;
        //El puntero del array original apunta al nuevo ahora
        array = temp;
        //Seteo front y back de manera conveniente
        front = 0;
        back = cont;
        //Doblo la capacidad del array circular
        capacity *= 2;
    }
    void display(){
        if(is_empty())
            cout << "Array vacío!"<<endl;
        else if(back>front){
            for(int i=front;i<=back;i++){
                cout << array[i] << ",";    
            }
        }
        else{
            //Imprime desde front hasta el últim elm.
            for(int i=front;i<capacity;i++){
                cout << array[i] << ",";    
            }
            //Imprime desde el primer elemento hasta el back
            for(int i=0;i<=back;i++){
                cout << array[i] << ",";
            }
        }
        cout << endl;    
    }
    T get_front(){
        return array[front];
    }
    T get_back(){
        return array[back];
    }

    bool contains(T data){
        if(is_empty())
            return false;
        else{
            if(back>front){
                for(int i=front;i<=back;i++){
                    if(array[i]==data)
                        return true;   
                }
            }
            else{
                //Imprime desde front hasta el últim elm.
                for(int i=front;i<capacity;i++){
                    if(array[i]==data)
                        return true;   
                }
                //Imprime desde el primer elemento hasta el back
                for(int i=0;i<=back;i++){
                    if(array[i]==data)
                        return true;  
                }
            }
            return false;
        }
    }


private:
    int next(int pos){
        return (pos+1)%capacity;
    }
    int prev(int pos){
        if(pos==0)
            return capacity-1;
        else
            return pos-1;
    }
};

