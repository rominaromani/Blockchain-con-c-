#ifndef PATRICIATRIE_H
#define PATRICIATRIE_H

#include <iostream>
#include <list>
#include <map>
#include "trie.h"
using namespace std;

// const unsigned ALPHA_SIZE = 26;//tamanio del alfabeto: letras minusculas

class TriePatricia : public Trie{
private:
    struct TrieNode{     
        // opcion 1:   TrieNode **children;                
        TrieNode **children;   //array de hijos   
        string prefix;
        bool endWord; //indica si es final de una palabra

        TrieNode(){
            children = new TrieNode*[ALPHA_SIZE]();
            for(int i=0;i<ALPHA_SIZE;i++){
                children[i] = nullptr;
            }
            prefix = "";
            endWord = false;
        }
        ~TrieNode() {
            for (int i = 0; i < ALPHA_SIZE; i++) {
                if (children[i] != nullptr) {
                    delete children[i];
                }
            }
            delete[] children;
        }
    };
    TrieNode* root;
    
public:
    TriePatricia() {
        root = new TrieNode();
    }

    // void insert(string key){
    //     if (search(key)) return;
    //     insert(root, key);
    // }

    // void insert(TrieNode* current, string key){
    //     string word = key;
    //     string coincidence;
    //     string rest;
    //     string index; // necessary?
    //     TrieNode* node = new TrieNode;

    //     cout << "key: '" << key << "'" << endl;
    //     if (current->children[word[0]-'a'] == nullptr || current->children[word[0]-'a']->prefix == word){ // case 1: is empty or equal to prefix
    //         cout << "null" << endl;
    //         TrieNode* newChild = new TrieNode();
    //         current->children[word[0]-'a'] = newChild;
    //         newChild->prefix = word;
    //         newChild->endWord = true;
    //     }
    //     else if (current->children[word[0]-'a'] != nullptr){ // case : half coincidence
    //         cout << "not null" << endl;
    //         // case : not include one in the other
    //         // if (current->children[word[0]-'a']->prefix.length() > word.length()){
    //         //     for (int i=0; i<word.length(); i++){
    //         //         if ()
    //         //     }
    //         // }
    //         // else if (current->children[word[0]-'a']->prefix.length() < word.length()){
    //         //     for (int i=0; i<current->children[word[0]-'a']->prefix.length(); i++){
    //         //         if ()
    //         //     }
    //         // }
    //         cout << current->children[word[0]-'a']->prefix.length() << endl;
    //         cout << word.length() << endl;
    //         if (current->children[word[0]-'a']->prefix.length() <= word.length()){ // case 2: prefix < word
    //             cout << "entra 1" << endl;
    //             for (int i=0; i<word.length(); i++){
    //                 if (rest.length() > 0 || current->children[word[0]-'a']->prefix[i] != word[i]) {
    //                     rest += word[i];
    //                     cout <<"rest: '" << rest  << "'" << endl;
    //                 }
    //                 else {
    //                     coincidence += word[i];
    //                 }
    //             }
    //             current->children[word[0]-'a']->prefix = coincidence;
    //             cout <<"rest1: " << rest << endl;
    //             insert(current->children[word[0]-'a'], rest);
    //             current->children[word[0]-'a']->endWord = false;
    //         }
    //         else if (current->children[word[0]-'a']->prefix.length() > word.length()){ // case 3: prefix > word
    //             cout << "entra 2" << endl;
    //             TrieNode* newChild = new TrieNode();
    //             if (current->children[word[0]-'a'] == nullptr){ // case 3.1: without child
    //                 for (int i=0; i<current->children[word[0]-'a']->prefix.length(); i++){
    //                     if (i > word.length() || current->children[word[0]-'a']->prefix[i] != word[i]) {
    //                         rest += current->children[word[0]-'a']->prefix[i];
    //                     }
    //                 }
    //                 cout <<"rest2: " << rest << endl;
    //                 current->children[word[0]-'a']->prefix = word;
    //                 insert(current->children[word[0]-'a'], rest);
    //             }
    //             else if (current->children[word[0]-'a'] != nullptr) { // case 3.1: with child
    //                 for (int i=0; i<current->children[word[0]-'a']->prefix.length(); i++){
    //                     if (i > word.length() || current->children[word[0]-'a']->prefix[i] != word[i]) {
    //                         rest += current->children[word[0]-'a']->prefix[i];
    //                     }
    //                 }
    //                 cout <<"rest3: " << rest << endl;
    //                 current->children[word[0]-'a']->prefix = word;
    //                 insert(current->children[word[0]-'a'], rest);
    //             }
    //         }
    //     }
    // }



    void insert(string key){
        if(search(key)){ //Si la palabra ya se encuentra, no se hace nada
            return;  
        }
        // cout << "no esta" << endl;
        int letra=0;
        string coincidencia;
        TrieNode* current = root;
        int index = 0; // indice del carater del key
        bool terminoNodo = false;
        bool terminado = false;
        while (index < key.length() || terminado == false ) {
            //Si la key a ingresar es más pequeña que el prefix del último nodo en que se encontró coincidencias, se crea un nuevo nodo
            //y se le asigna a su prefix el prefix restante del nodo actual
            if(index==key.length() ){ 
                TrieNode* nuevo = new TrieNode();
                nuevo->prefix = current->prefix.substr(letra);
                nuevo->endWord = current->endWord;
                nuevo->children = current->children;
                current->children = new TrieNode*[ALPHA_SIZE]();
                current->children[current->prefix[letra]-'a'] = nuevo;
                current->endWord = true;
                terminado = true;
                //actualizo el nodo actual
                current->prefix = coincidencia;
                break;
            }
            
            int childIndex = key[index] - 'a';

            if (current->children[childIndex] == nullptr){ //si no se encuentra
                //Si el nodo actual es el root, crear nuevo nodo con toda la nueva key
                if(current == root){
                    root->children[childIndex] = new TrieNode();
                    root->children[childIndex]->prefix = key;
                    root->children[childIndex]->endWord = true;
                    //cout << root->children[childIndex]->prefix << endl;
                    terminado = true;
                    break;
                }
                //Si el nodo actual no es el root, crear nuevo nodo con la key restante y otro con lo que no coincidió con el prefix
                else{
                    string copiaCurrent_prefix = current->prefix; //guardo el prefix para poder usarlo después
                    if(current->prefix=="")
                        break;
                    //creación del nuevo nodo con lo que no coincidió con el prefix (si es que hubo algo que no coincidió)
                    if(!terminoNodo){
                        TrieNode* newNode = new TrieNode();
                        // if(index<=copiaCurrent_prefix.size()){
                        //     newNode->prefix = copiaCurrent_prefix.substr(letra);
                        // }
                        // else{
                        //     newNode->prefix = copiaCurrent_prefix.substr(index/2);
                        // }
                        newNode->prefix = copiaCurrent_prefix.substr(letra);
                        newNode->endWord = current->endWord;
                        newNode->children = current->children;
                        
                        current->children = new TrieNode*[ALPHA_SIZE]();
                        current->children[copiaCurrent_prefix[index]-'a'] = newNode;
                    }
                    //creación del nuevo nodo con la key restante
                    current->children[childIndex] = new TrieNode();
                    current->children[childIndex]->prefix = key.substr(index);
                    current->prefix = current->prefix.substr(0, index);
                    //Si la nueva palabra abarca toda la palabra del nodo actual, se marca como final de palabra, sino no.
                    if(coincidencia==copiaCurrent_prefix){
                        current->endWord = true;
                    }else{
                        current->endWord = false;
                    }
                    current->children[childIndex]->endWord = true;
                    terminado = true;
                        
                    break;
                }
            }
            else{
                // cout << "No es nulo" << endl;
                terminoNodo = false;
                coincidencia = "";
                current = current->children[childIndex];
                letra=0;
                for(int i = 0; i < current->prefix.length() && current->prefix[i]==key[index]; i++){
                    //cout << "coinciden " << current->prefix[i] << " con " << key[index] << endl;
                    coincidencia += current->prefix[i];
                    index++;
                    letra++;
                    if(i==current->prefix.length()-1)
                        terminoNodo = true;
                }
                // cout << "coincidencia: " << coincidencia << endl;
            }
        }
    }
    
    bool search(string key){ //buscar una palabra en un radix trie. "hola"
        TrieNode* current = root;
        int index = 0;
        string cadena;
        bool terminoNodo = false;
        while (index < key.length()){ //key.length() = 7 (holanga)
            int childIndex = key[index] - 'a'; //h:7
            if (current->children[childIndex] == nullptr) {
                return false;
            }
            else{
                current = current->children[childIndex];
                terminoNodo = false;
                for(int i = 0; i < current->prefix.length() && current->prefix[i]==key[index]; i++){
                    cadena += current->prefix[i];
                    index++;
                    if(i==current->prefix.length()-1)
                        terminoNodo = true;
                }
            }
        }
        // cout << "search: " << cadena << endl;
        if(current->endWord && key==cadena && terminoNodo) //si el nodo es endword, la key es igual a la cadena y recorrió todo el nodo
            return true;                                    //retorna true
        return false;
    }


    void remove(string key){
        TrieNode* current = root;
        int index = 0;
        string cadena;
        bool terminoNodo = false;
        while (index < key.length()) 
        {
            int childIndex = key[index] - 'a'; //h:7
            if (current->children[childIndex] == nullptr)
            {
                try{
                    throw "No existe la palabra";
                }catch(const char* msg){
                    cerr << msg << endl;
                }
                return;
            }
            else{
                current = current->children[childIndex];
                terminoNodo = false;
                for(int i = 0; i < current->prefix.length() && current->prefix[i]==key[index]; i++){
                    cadena += current->prefix[i];
                    index++;
                    if(i==current->prefix.length()-1)
                        terminoNodo = true;
                }
            }
        }
        if(current->endWord && key==cadena && terminoNodo)
            current->endWord = false;     
        else{
            try{
                throw "No existe la palabra";
            }catch(const char* msg){
                cerr << msg << endl;
            }
            return;
        }                               
    }


    string display(){
        return display(root, " ");
    }

    string display(TrieNode* node, string sep){
        if (node == nullptr)
            return "";
        string res = "";
        res += node->prefix + sep;
        for(int i = 0; i < ALPHA_SIZE; i++){
            if(node->children[i] != nullptr){
                res += display(node->children[i], sep);
            }
        }
        return res;
    }
    
    //devuelve las palabras del trie en orden
    string toString(string sep){
        return toString(root,sep,"");
    }

    string toString(TrieNode* node, string sep, string prefix){
        if(node == nullptr){
            return "";
        }
        string result = "";
        if(node->endWord && node!=root){
            result += prefix + node->prefix + sep;
        }
        for(int i = 0; i < ALPHA_SIZE; i++){
            if(node->children[i] != nullptr){
                result += toString(node->children[i], sep, prefix + node->prefix);
            }
        }
        return result;
    }


    string imprimirEndWords(string sep){
        return imprimirEndWords(root,sep,"");
    }

    //imprime ordenado un radix trie
    string imprimirEndWords(TrieNode* node, string sep, string prefix){
        //cout << "nodo: " << node->prefix << endl;
        if(node==nullptr){
            //cout << "nodo es nulo" << endl;
            return "";
        }
        string result="";
        //Caso base: cuando se llega a un nodo marcado como final de palabra, se retorna la palabra construida
        if(node->endWord){
            result += node->prefix + sep;
        }
        for(int i=0;i<ALPHA_SIZE;i++){  //se recorre todo el abecedario
            if(node->children[i]!=nullptr){ //Se ingresa a todo hijo existente, en orden (de la a a la z) 
                //Por cada nodo que recorre, se va agregando al resultado el caracter correspondiente al índice del hijo ingresado
                // cout << "i: " << i << endl;
                // cout << "voy por " << node->children[i]->prefix << endl;
                result += imprimirEndWords(node->children[i],sep,prefix + node->children[i]->prefix); 
            }
        }
        return result;
    }

};  

#endif