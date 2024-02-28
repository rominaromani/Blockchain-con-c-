#include <iostream>
#include <vector>
using namespace std;

int* buildPositions(string patron){
    int* positions = new int[256];
    for(int i=0;i<256;i++){
        positions[i] = -1;
    }
    for(long unsigned int i=0;i<patron.size();i++){
        positions[(int)patron[i]] = i;
    }
    return positions;
}

bool testBoyerMoore(string texto, string patron){
    const int n = texto.length();
    const int m = patron.length();
    //map<char,int> positions = buildPositions(patron);
    int* positions = buildPositions(patron);
    // for(const auto data:positions){
    //     cout << data.first << ":";
    //     cout << data.second << endl;
    // }
    int index;
    vector<int> results;
    
    for(int i=0;i<=n-m;){
        int j=m-1;
        while(j>=0 && (patron[j]==texto[i+j])){
            j--;
        }
        if(j>=0){
            index = texto[i+m];
            i += (i+m < n) ? m-positions[index] : 1; // evitamos el bucle infinito
        }else{
            results.push_back(i);
            i=i+m;
        }
    }
    if(results.size()>0){
        //cout << "Se encontraron " << results.size() << " coincidencias" << endl;
        return true;
    }else{
        //cout << "No se encontraron coincidencias" << endl;
        return false;
    }
}
