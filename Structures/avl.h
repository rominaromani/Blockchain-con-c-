#ifndef AVLTree_H
#define AVLTree_H

// #include "CircularArray.h"
#include "../block.h"

using namespace std;

// NodeBT
template <typename T> 
struct NodeBT {
    T data;
    Transferencia *transferencia;
    int height;
    NodeBT* left; 
    NodeBT* right;

    // Constructores
    NodeBT() : left(nullptr), right(nullptr), height(0) {}   
    NodeBT(T value) : data(value), left(nullptr), right(nullptr), height(0) {}   

    void killSelf(){
        if(left != nullptr) left->killSelf();
        if(right != nullptr) right->killSelf();
        delete this;
    }
};

// AVLTree
template <typename T>
class AVLTree {     
private:
    NodeBT<T> *root; // NodeBT
    int size=0;
public:

    AVLTree() : root(nullptr) {} // Constructor

    int get_size(){
        return size;
    }

    void insert(T value) {
        insert(this->root, value);
        size++;
    }

    void insert(T value, Transferencia* transferencia) {
        if(transferencia==nullptr)
            cout << "transferencia insertada es nula" << endl;
        insert(this->root, value, transferencia);
        size++;
    }

    bool find(T value) {
        return find(this->root, value);
    }
   
    string getPreOrder(){
            string aux;
            auxPreOrder(root,aux);
            return aux;
    }

    int height(){
        updateHeight(this->root);
        return height(this->root);
    }

    // T minValue()
    // {
    //     T min=999999;
    //     try{
    //         if(root==nullptr){
    //             min = 0;
    //             throw "El árbol no tiene elementos";
    //         }
    //         else{
    //             auxMinValue(root,min);
    //         }
    //     }
    //     catch(const char *msg){
    //         cerr << msg << endl;
    //     }
    //     return min;
    // }

    // T maxValue()
    // {
    //     T max=-99999;
    //         try{
    //             if(root==nullptr){
    //                 max = 0;
    //                 throw "El árbol no tiene elementos";
    //             }
    //             else{
    //                 auxMaxValue(root,max);
    //             }
    //         }
    //         catch(const char *msg){
    //             cerr << msg << endl;
    //         }
    //         return max;
    // }

    T minValue() {
        return minValue(this->root);
    }

    T maxValue() {
        return maxValue(this->root);
    }

    T minValue(NodeBT<T> *node){
        if (node == nullptr)
            throw("The tree is empty");
        else if (node->left == nullptr)
            return node->data;
        else
            return minValue(node->left);
    }

    T maxValue(NodeBT<T> *node){
        if (node == nullptr)
            throw("The tree is empty");
        else if (node->right == nullptr)
            return node->data;
        else
            return maxValue(node->right);
    }

    bool isBalanced() {
        return isBalanced(this->root);
    }

    // int size() {
    //     return size(this->root);
    // }

    void remove(T value) {
        remove(this->root, value);
        size--;
    }

    void displayPretty() {
        displayPretty(this->root, 1);
    }

    ~AVLTree(){ // Destructor
        if(this->root != nullptr){
            this->root->killSelf();
        }
    }

    // vector<T> rangeSearch(T menor, T mayor){
    //     vector<T> elementos;
    //     try{
    //         if(mayor<menor)
    //             throw "El rango es inválido";
    //         auxRangeSearch(root,menor,mayor,elementos);
    //     }
    //     catch(const char *msg){
    //         cerr << msg << endl;
    //     }
    //     return elementos;
    // } 

    CircularArray<Transferencia*> rangeSearch(T menor, T mayor){
        CircularArray<Transferencia*> elementos;
        try{
            if(mayor<menor)
                throw "El rango es inválido";
            auxRangeSearch(root,menor,mayor,elementos);
        }
        catch(const char *msg){
            cerr << msg << endl;
        }
        return elementos;
    } 

private:

    // void auxRangeSearch(NodeBT<T> *node, int limInf, int limSup, vector<T> &result){
    //         if (node == nullptr)
    //             return;
    //         if (limInf < node->data){
    //             auxRangeSearch(node->left, limInf, limSup, result);
    //         }
    //         if (limInf <= node->data && limSup >= node->data)
    //             result.push_back(node->data);
    //         if (limSup > node->data){
    //             auxRangeSearch(node->right, limInf, limSup, result);
    //         }
    //     }

    void auxRangeSearch(NodeBT<T> *node, T limInf, T limSup, CircularArray<Transferencia*> &result){
            if (node == nullptr)
                return;
            //vamos al nodo con el valor mas pequeño que sea mayor o igual a limInf
            if (limInf < node->data){
                auxRangeSearch(node->left, limInf, limSup, result);
            }
            //si el valor del nodo esta en el rango, lo agregamos al vector
            if (limInf <= node->data && limSup >= node->data)
                result.push_back(node->transferencia); //se agregan las transferencias que estan en el rango
                
            //Si aun no hemos llegado al limite superior, seguimos buscando elementos mayores
            if (limSup > node->data){
                auxRangeSearch(node->right, limInf, limSup, result);
            }
        }

    void auxPreOrder(NodeBT<T> *node, string &aux){
        if(node==nullptr)      //cuando llegue a un nodo nulo (hijo inexistente) regresa a la llamada anterior (al padre)  
                return;
        aux += to_string(node->data) + " "; //Imprime primero la raiz y luego se dezplaza
        auxPreOrder(node->left,aux);
        auxPreOrder(node->right,aux);
    }

    void insert(NodeBT<T> *&node, T value){
        if(node == nullptr){
            node = new NodeBT<T>(value);
        }
        else if(value < node->data){
            insert(node->left, value);
        }
        else if(value > node->data){
            insert(node->right, value);
        }
        else{  //Si se encuentra un elemento con el mismo valor, no se inserta
            return;
        }
        //cout << "Balanceo" << endl;
        //Cada vez que se inserta un elemento, se balancea y actualiza la
        //altura toda la rama por donde se recorrió para insertar
        //el elemento
        updateHeight(node);
        balance(node);
    }

    void insert(NodeBT<T> *&node, T value, Transferencia* transferencia){
        if(node == nullptr){
            node = new NodeBT<T>(value);
            node->transferencia = transferencia;
        }
        else if(value < node->data){
            insert(node->left, value,transferencia);
        }
        else if(value > node->data){
            insert(node->right, value,transferencia);
        }
        else{  //Si se encuentra un elemento con el mismo valor, no se inserta
            return;
        }
        //cout << "Balanceo" << endl;
        //Cada vez que se inserta un elemento, se balancea y actualiza la
        //altura toda la rama por donde se recorrió para insertar
        //el elemento
        updateHeight(node);
        balance(node);
        size++;
    }

    bool find(NodeBT<T> *node, T value){
        if (node == nullptr)
            return false;
        else if (value < node->data)
            return find(node->left, value);
        else if (value > node->data)
            return find(node->right, value);
        else
            return true;
    }

    int height(NodeBT<T> *node){
        //cout << "--Entra height--" << endl;
        if(node == nullptr){
            return -1;
        }
        //cout << "calculando altura de " << node->data << endl;
        return node->height;
    }

    bool isBalanced(NodeBT<T> *node){
        if(node == nullptr){
            return true;
        }
        if(abs(balancingFactor(node)) > 1){
            return false;
        }
        return true;
    }

    void auxMinValue(NodeBT<T>*node,T &min){
            //viajo por los hijos izquierdos porque son los menores
            if(node->left!=nullptr)
                auxMinValue(node->left,min);
            //Si se llega al nodo sin hijo izquierdo, estamos en el menor nodo
            if(node->data<min) //comparo para evitar perder el mínimo cuando retorne en las llamadas recursivas
                min = node->data;
    }

    void auxMaxValue(NodeBT<T>*node,T &max){ //misma idea que para hallar min
            if(node->right!=nullptr){
                auxMaxValue(node->right,max);
            }
            if(node->data>max)
                max = node->data;
    }

    // int size(NodeBT<T> *node){
    //     if(node == nullptr){
    //         return 0;
    //     }
    //     return 1 + size(node->left) + size(node->right);
    // }

    void remove(NodeBT<T> *&node, T value){
        if(node == nullptr){ //si no lo encuentra, no hace nada
            return;
        }
        else if(value < node->data){
            remove(node->left, value);
        }
        else if(value > node->data){
            remove(node->right, value);
        }
        else{
            //caso 1: nodo sin hijos
            if(node->left == nullptr && node->right == nullptr){
                delete node;
                node = nullptr;
            }
            //caso 2: nodo con un hijo
            else if(node->left == nullptr){
                NodeBT<T> *aux = node;
                node = node->right;
                aux->right = nullptr;
                delete aux;
            }
            else if(node->right == nullptr){
                NodeBT<T> *aux = node;
                node = node->left;
                aux->left = nullptr;
                delete aux;
            }
            //caso 3: nodo con dos hijos
            else{
                NodeBT<T> *aux = node->left;
                while(aux->right != nullptr){
                    aux = aux->right;
                }
                //Se copia el valor al predecesor a la raiz
                node->data = aux->data;
                //se elimina al predecesor de su anterior posición
                remove(node->left, aux->data); 
            }
        }

        //balancea y actualiza la altura de la rama por donde se recorrió
        //para llegar al nodo a elminar
        balance(node);
        updateHeight(node);
    }
    
    void displayPretty(NodeBT<T> *node, int level){
        if(node == nullptr){
            return;
        }
        displayPretty(node->right, level + 1);
        for(int i = 0; i < level; i++){
            cout << "    ";
        }
        cout << node->data << endl;
        displayPretty(node->left, level + 1);
    }

    /*add for avl*/    //------------------------------------------------------------ AVL ------------------------------------------------------------
    int balancingFactor(NodeBT<T> *node){
        return height(node->left) - height(node->right);
    }

    void updateHeight(NodeBT<T> *node){
        if(node == nullptr)
            return;
        node->height = max(height(node->left), height(node->right)) + 1;
    }

    void balance(NodeBT<T> *&node){
        if (node == nullptr)
            return;
        //Si está desbalanceado a la izquierda
        if (balancingFactor(node) == 2){
            //Si el hijo izquierdo también está desbalanceado a la izquierda
            if(balancingFactor(node->left) == 1){
                right_rota(node); //rotación simple
            } //Si el hijo izquierdo está desbalanceado a la derecha
            else{
                //rotación doble (leftRight)
                left_rota(node->left);
                right_rota(node);
            }
        } //Si el nodo está desbalanceado a la derecha
        else if(balancingFactor(node) == -2){
            //si el hijo derecho también está desbalanceado a la derecha
            if(balancingFactor(node->right) == -1){
                left_rota(node); //rotación simple
            } //si el hijo está desbalanceado a la izquierda
            else{
                //rotación doble (rightLeft)
                right_rota(node->right);
                left_rota(node);
            }
        }
    }
    
    void left_rota(NodeBT<T> *&node){
        NodeBT<T> *aux = node->right;
        node->right = aux->left;
        aux->left = node;
        updateHeight(node);
        updateHeight(aux);
        node = aux;
    } 

    void right_rota(NodeBT<T> *&node){
        NodeBT<T> *aux = node->left;
        node->left = aux->right;
        aux->right = node;
        updateHeight(node);
        updateHeight(aux);
        node = aux;
    } 

    /*Auxiliares*/
    // int max(int a, int b){
    //     return a > b ? a : b;
    // }
};


#endif
