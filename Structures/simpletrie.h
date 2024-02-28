#ifndef SIMPLETRIE_H
#define SIMPLETRIE_H

#include <iostream>
#include "trie.h"
#include "circulararray.h"
#include "../block.h"

using namespace std;

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
        //convertimos todo a minúsculas: O(key.length)
        for(size_t i =0;i<key.length();i++){
            key[i] = tolower(key[i]);
        }

        string key2;
        //quitamos espacios. Solo agrega la primera palabra en la oración
        for(int i=0;i<key.length();i++){ // O(key.length)
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
        for(size_t i =0;i<key.length();i++){ // O(key.length)
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
        for(size_t i =0;i<key.length();i++){ // O(n)
            key[i] = tolower(key[i]);
        }
        string key2;
        //quitamos espacios. Solo agrega la primera palabra en la oración
        for(int i=0;i<key.length();i++){ // O(n)
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