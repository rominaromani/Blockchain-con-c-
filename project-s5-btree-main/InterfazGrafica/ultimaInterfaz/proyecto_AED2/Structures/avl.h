#ifndef AVL_H
#define AVL_H

// #include "CircularArray.h"
#include "../block.h"
#include <iostream>
#include "trie.h"
#include "circulararray.h"
//#include "../block.h"


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



//using namespace std;

//const unsigned ALPHA_SIZE = 26;//tamanio del alfabeto: letras minusculas

class TrieSimple : public Trie {
private:
    struct TrieNode{
        // opcion 1:   TrieNode **children;           
        // opcion 2:   list<pair<char, TrieNode*>> children;        
        // opcion 3:   map<pair<char, TrieNode*>> children;   
        TrieNode **children;        
        bool endWord; //indica si es final de una palabra
        Transferencia* transferencia;

        TrieNode(){
            endWord = false;
            children = new TrieNode*[ALPHA_SIZE];
            for(int i=0;i<ALPHA_SIZE;i++){
                children[i] = nullptr;
            }
        }

        ~TrieNode(){
            clear();
        }

        void clear(){
            for(int i=0;i<ALPHA_SIZE;i++){
                if(children[i]!=nullptr){
                    delete children[i];
                }
            }
            delete[] children;
        }
    };

    TrieNode* root;
    int size;

public:
    TrieSimple(): root(nullptr),size(0) {}

    ~TrieSimple(){
        if(root!=nullptr){
            delete root;
        }
    }

    void insert(string key){
        //convertimos todo a minúsculas
        for(size_t i =0;i<key.length();i++){
            key[i] = tolower(key[i]);
        }
        string key2;
        //quitamos espacios. Solo agrega la primera palabra en la oración
        for(int i=0;i<key.length();i++){
            if(key[i]!=' '){
                key2+=key[i];
            }
            else{
                break;
            }
        }
        key = key2;

        //inserción
        if (root==nullptr) {
            root = new TrieNode();
        }
        TrieNode* node = root;
        for(size_t i =0;i<key.length();i++){
            if(node->children[key[i]-'a']==nullptr){
                node->children[key[i]-'a'] = new TrieNode();
            }
            node = node->children[key[i]-'a'];
        }
        node->endWord = true;
        size++;
    }

    void insert(string key, Transferencia* transferencia){
        //convertimos todo a minúsculas
        for(size_t i =0;i<key.length();i++){
            key[i] = tolower(key[i]);
        }
        string key2;
        //quitamos espacios. Solo agrega la primera palabra en la oración
        for(int i=0;i<key.length();i++){
            if(key[i]!=' '){
                key2+=key[i];
            }
            else{
                break;
            }
        }
        key = key2;

        //inserción
        if (root==nullptr) {
            root = new TrieNode();
        }
        TrieNode* node = root;
        for(size_t i =0;i<key.length();i++){
            if(node->children[key[i]-'a']==nullptr){
                node->children[key[i]-'a'] = new TrieNode();
            }
            node = node->children[key[i]-'a'];
        }
        node->endWord = true;
        node->transferencia = transferencia;
        size++;
    }


    bool search(string key){
        //convertimos todo a minúsculas
        for(size_t i =0;i<key.length();i++){
            key[i] = tolower(key[i]);
        }
        string key2;
        //quitamos espacios. Solo busca la primera palabra en la oración
        for(int i=0;i<key.length();i++){
            if(key[i]!=' '){
                key2+=key[i];
            }
            else{
                break;
            }
        }
        key = key2;


        if(root==nullptr) return false;
        TrieNode* node = root;
        for(size_t i =0;i<key.length();i++){
            if(node->children[key[i]-'a']!=nullptr) {
                node = node->children[key[i]-'a'];
            }
            else return false;
        }

        return node->endWord;
    }

    void remove(string key){
        if(root==nullptr){
            try{
                throw "No hay nada que eliminar";
            }
            catch(const char* msg){
                cerr << msg << endl;
            }
        }

        //convertimos todo a minúsculas
        for(size_t i =0;i<key.length();i++){
            key[i] = tolower(key[i]);
        }
        string key2;
        //quitamos espacios. Solo agrega la primera palabra en la oración
        for(int i=0;i<key.length();i++){
            if(key[i]!=' '){
                key2+=key[i];
            }
            else{
                break;
            }
        }
        key = key2;

        remove(key,root,0);
        size--;
    }   
    
    //imprime ordenado
    string toString(string sep){
        return toString(root,sep,"");
    }

    //Obtiene todas las palabras que empiezan con el prefijo dado
    CircularArray<Transferencia*> getWordsWithPrefix(string prefix){
        //convertimos todo a minúsculas
        for(size_t i =0;i<prefix.length();i++){
            prefix[i] = tolower(prefix[i]);
        }
        string prefix2;
        //quitamos espacios. Solo agrega la primera palabra en la oración
        for(int i=0;i<prefix.length();i++){
            if(prefix[i]!=' '){
                prefix2+=prefix[i];
            }
            else{
                break;
            }
        }
        prefix = prefix2;

        CircularArray<Transferencia*>result;
        if(root==nullptr)
            return result;
        TrieNode* node = root;
        //1. Recorremos letra por letra del prefix para ver si existe al menos una
        //palabra con ese prefijo
        for(size_t i=0;i<prefix.length();i++){
            //2.1 Si se encuentra esa letra en el trie, se avanza al siguiente nodo
            if(node->children[prefix[i]-'a']!=nullptr) {
                node = node->children[prefix[i]-'a'];
            }
            //2.2 Si no se encuentra, se retorna un arreglo vacío
            else
                return result;
        }
        //3. Si existe al menos una palabra con ese prefijo, se recorren todas
        //esas palabras y se agregan al arreglo
        getWordsWithPrefix(node,result);
        //4. Se retorna el arreglo de palabras
        return result;
    }

    int get_size(){
        return size;
    }

    private:
    string toString(TrieNode* node, string sep, string prefix){
        if(node==nullptr) return "";
        string result="";
        //Caso base: cuando se llega a un nodo marcado como final de palabra, se retorna la palabra construida
        if(node->endWord){
            result += prefix + sep;
        }
        for(int i=0;i<ALPHA_SIZE;i++){  //se recorre todo el abecedario
            if(node->children[i]!=nullptr){ //Se ingresa a todo hijo existente, en orden (de la a a la z) 
                //Por cada nodo que recorre, se va agregando al resultado el caracter correspondiente al índice del hijo ingresado
                result += toString(node->children[i],sep,prefix+(char)(i+'a')); 
            }
        }
        return result;
    }

    void remove(string key, TrieNode* node, int index){
        if(node==nullptr) return;
        //2. Cuando se llega al fin de palabra, se desmcarca y se regresa a la llamada anterior
        if(index==key.length()){
            if(node->endWord){
                node->endWord = false; //Se desmarca el fin de palabra
            }
            return;
        }
        //1. Avanzo hasta encontrar el fin de palabra
        remove(key,node->children[key[index]-'a'],index+1); 
        //3. Se eliminan todos los nodos (desde la última letra hasta la primera) que contenían la palabra hasta encontrar una que sea parte de otra palabra
        if(node->children[key[index]-'a']->endWord==false){
            //Recorro todos los hijos del hijo al que me voy a dirigir
            for(int i=0;i<ALPHA_SIZE;i++){
                //Si tiene por lo menos 1 hijo, se regresa a la llamada anterior (subo a la letra anterior). 
                //Basta que no se elimine uno y no se eliminarán sus ancestros.
                if(node->children[key[index]-'a']->children[i]!=nullptr){
                    return;
                }
            }
            //Si todos son nulos, se libera la memoria del nodo y hacemos que apunte a nullptr
            delete node->children[key[index]-'a'];
            node->children[key[index]-'a'] = nullptr;
        }
    }

    void getWordsWithPrefix(TrieNode* node, CircularArray<Transferencia*>& result){
        if(node==nullptr) return;
        if(node->endWord){
            result.push_back(node->transferencia);
        }
        for(int i=0;i<ALPHA_SIZE;i++){
            if(node->children[i]!=nullptr){
                getWordsWithPrefix(node->children[i],result);
            }
        }
    }

};

#endif
