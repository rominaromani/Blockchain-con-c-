#ifndef TRIE_H
#define TRIE_H

#include <string>
const unsigned ALPHA_SIZE = 26;//tamanio del alfabeto: letras minusculas
using namespace std;

class Trie
{
public:
    virtual void insert(string key) = 0;
    virtual bool search(string key) = 0;
    virtual void remove(string key) = 0;        
    
    // //imprime ordenado
    virtual string toString(string sep=",") = 0;
};
#endif