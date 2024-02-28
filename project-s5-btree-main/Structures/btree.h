#ifndef BTree_H
#define BTree_H

#include <iostream>
#include <cmath>

using namespace std;

// NodeB
template <typename TK>
struct NodeB {
  TK* keys;// array de keys
  NodeB** children;// array de punteros a hijos
  int count; // cantidad de keys
  bool leaf; // indicador de nodo hoja
  int M; // orden o grado

  NodeB() : keys(nullptr), children(nullptr), count(0) {}
  NodeB(int M) {
    keys = new TK[M]; //Son M-1 keys, pero le dejo M espacios para poder hacer el split
    children = new NodeB<TK>*[M];
    count = 0;
    leaf = true;
  }
  NodeB(int M, bool _leaf) {
    keys = new TK[M]; //Son M-1 keys, pero le dejo M espacios para poder hacer el split
    children = new NodeB<TK>*[M];
    count = 0;
    this->M = M;
    leaf = _leaf;
  }

  // Función para liberar la memoria de un nodo

  ~NodeB() {
    // cout << "Eliminando nodo:" << endl;
    // display();
    // if(children[0]->leaf){
    //   cout << "Eliminando hojas" << endl;
    //   for (int i = 0; i < count + 1; i++)
    //     delete children[i];
    // }
    delete[] keys;
    delete[] children;
    // cout << "Terminada la eliminación del nodo" << endl;
  }

  void killSelf() {
    if (!leaf) {
      for (int i = 0; i < count + 1 && !children[0]->leaf; i++) {
        children[i]->killSelf();
      }
    }
    delete this;
  }

  
  // Function to search key k in subtree rooted with this NodeB
  NodeB<TK>* search(int k){
    // Find the first key greater than or equal to k
    // If the found key is equal to k, return this NodeB
    //display();
    int i;
    for(i = 0; i < count && k>=keys[i]; i++){
      if(keys[i] == k)
        return this;
    }
    // while (i < count && k > keys[i])
    //   i++;

    //cout << "Display del nodo: ";
    // if (keys[i] == k){
      
    //   cout << "Key found: " << keys[i] << endl; 
    

    // If key is not found here and this is a leaf NodeB
    if (leaf == true)
      return nullptr;

    // Go to the appropriate child
    return children[i]->search(k);
}

  void insertNonFull(int k)
  {
    // Initialize index as index of rightmost element
    int i = count-1;

    // If this is a leaf NodeB
    if (leaf == true)
    {
      // The following loop does two things
      // a) Finds the location of new key to be inserted
      // b) Moves all greater keys to one place ahead
      while (i >= 0 && keys[i] > k)
      {
        keys[i+1] = keys[i];
        i--;
      }

      // Insert the new key at found location
      keys[i+1] = k;
      count = count+1;
    }
    else // If this NodeB is not leaf
    { 
      // Find the child which is going to have the new key
      while (i >= 0 && keys[i] > k)
        i--;

      // See if the found child is full
      if (children[i+1]->count == M-1)
      {
        // If the child is full, then split it
        splitChild(i+1, children[i+1],k,0); //0 porque es la primera vez que se llama a splitChild si se llama a insertNonFull

        // After split, the middle key of C[i] goes up and
        // C[i] is splitted into two. See which of the two
        // is going to have the new key
        if (keys[i+1] < k)
          i++;
      }
      children[i+1]->insertNonFull(k);
    }
  }


  void splitChild(int i, NodeB<TK> *y, TK k, int it){
    //1. Inserto elemento al nodo en la posición correcta (orden ascendente). No olvidar sumarle 1 a count. Este nodo recibe el nombre de y
    if(it==0){  //solo se ejecuta la primera vez que se llama a splitChild
      int j = y->count-1;
      while (j >= 0 && y->keys[j] > k){
        y->keys[j+1] = y->keys[j];
        j--;
      }
      y->keys[j+1] = k;
      y->count = y->count+1;
    }
    //cout << "a" << endl;
    //cout << "y: " << endl;
     // y->display();     //Si se borra da error, no pregunten por qué porque no lo sé
    //cout  << endl;
    //2. Creo un nuevo nodo que va a almacenar M/2 -1 keys de y
    NodeB<TK> *z = new NodeB<TK>(y->M, y->leaf);
    z->count = 0;

    //3. Le paso a z solamente los elementos que estén después del elemento del medio (count/2). No olvidar reducir la cantidad de keys de y, y aumentar la cantidad de keys de z.
    int minChild = ceil(M/2.0);
    for (int j = 0; j < minChild-1; j++){
      z->keys[j] = y->keys[j+minChild];
      z->count++;
    }


    //4. Le paso a z los hijos correspondientes a los elementos que le pasé.  No olvidar reducir la cantidad de keys de y, y aumentar la cantidad de keys de z.
    if (y->leaf == false){
      for (int j = 0; j < minChild; j++){
        z->children[j] = y->children[j+minChild];
      }
    }
    y->count = minChild;

    //5. Crear espacio (empujar elementos) para insertar a z como nuevo hijo del padre de y
    for (int j = count; j >= i+1; j--){
      children[j+1] = children[j];
    }

    //6. Linkear a z como hijo del padre.
    children[i+1] = z;

    //7. Hacer espacio en el padre para colocar el key que está en medio de y. No olvidar sumarle 1 a count.
    for (int j = count-1; j >= i; j--){ //j>=i porque i es la posición del elemento que fue mayor que el key que se insertó
      keys[j+1] = keys[j];
    }
    
    //8. Insertar el key que está en medio de y en el padre
    keys[i] = y->keys[minChild-1];
    count++;
    y->count--;

  }

  void inOrder(string &s, string sep){
    //display();
  	// There are n keys and n+1 children, traverse through n keys
    // and first n children
    int i;
    for (i = 0; i < count; i++)
    {
      // If this is not leaf, then before printing key[i],
      // traverse the subtree rooted with child C[i].
      if (leaf == false){
        children[i]->inOrder(s, sep);
      }
      
      s += to_string(keys[i]) + sep;
    }

    // Print the subtree rooted with last child
    if (leaf == false)
      children[i]->inOrder(s,sep);
  }


  void display(){
    cout << "Keys: ";
    for(int i=0;i<count;i++){
      cout << keys[i] << ", ";
    }
    cout << "Count: " << count << endl;
    cout << endl;
    cout << "Children: ";
    for(int i=0;i<=count;i++){
      cout << children[i] << ", ";
    }
    cout << endl;
    cout << "---------" << endl;
  }

  TK maxKey(){
    //this->display();
    //cout << "Count:" << count << endl;
    if(leaf){
      return keys[count-1];
    }else{
      return children[count]->maxKey();
    }
  }

  TK minKey(){
    if(leaf){
      return keys[0];
    }else{
      return children[0]->minKey();
    }
  }

  void remove(TK k){
    NodeB<TK>* root = this;
      //1. Busco el nodo que contiene el key
      NodeB<TK>* nodo = this; //comienzo siempre en la raíz
      bool keyFound = false;
      while(!nodo->leaf){
        int i = 0;
        if(nodo->count==0){
          for(int j=0;j<=nodo->count;j++){
            if(nodo->children[j]!=nullptr && keyFound==false){
              for(int m=0;m<nodo->children[j]->count;m++){
                if(nodo->children[j]->keys[m]==k){
                  nodo = nodo->children[j];
                  keyFound = true;
                  break;
                }
              }
            }
          }
        }
        if(keyFound){
          break;
        }
        while (i < nodo->count && k > nodo->keys[i])
          i++;
        if(nodo->keys[i]==k){   //si encuentra al key sale del while
          break;
        }  
        nodo = nodo->children[i];
      }

      //2. Buscar el padre del nodo que contiene el key
      bool hizoMerge = false;
      if(nodo==root){ //si el nodo que contiene el key es la raíz
        //busco el índice donde se encuentra el key
        int idx = 0;
        while (idx < nodo->count && nodo->keys[idx] != k)
          idx++;
        nodo->removeFromNonLeaf(idx, root,hizoMerge);
      }
      else{   //si el nodo que contiene el key no es la raíz
        NodeB<TK>* padre = this; //comienzo siempre en la raíz
      while(!padre->leaf){
        int i = 0;
        while (i < padre->count && nodo->keys[0] > padre->keys[i])
          i++;
        if(padre->children[i]==nodo)  //si encuentra a un padre que tenga entre sus hijos al nodo que contiene el key sale del while
            break;
        padre = padre->children[i];  
        }


        //3. busco a cual hijo del padre pertenece el nodo que se está llenando
          int i = 0;
          while (i < padre->count && nodo->keys[0] > padre->keys[i]) //creo que esto es innecesario pero lo dejo por si acaso
            i++;    
        // cout << "i: " << i << endl;
        //4. Si el nodo es una hoja, entonces solo se elimina
        //4.1 Busco el índice en donde se encuentra el key
        int idx = 0;
        while (idx < nodo->count && nodo->keys[idx] != k)
          idx++;
  
        //4.2 Si el key está en el nodo, lo elimino
        if (idx < nodo->count && nodo->keys[idx] == k)
        {
          // cout << "El key está en el nodo" << endl;
          //4.2.1 Si el nodo es una hoja, entonces el key se elimina
          if (nodo->leaf){
            // cout << "Es hoja" << endl;
            nodo->removeFromLeaf(idx);
          }
          //4.2.2 Si el nodo no es una hoja, entonces el key se reemplaza por su predecesor o sucesor
          else{
            // cout << "No es una hojaaaaaaa" << endl;
            nodo->removeFromNonLeaf(idx,root,hizoMerge);
          }
        }

        //5. Si el hijo que contenía el key tiene menos keys del mínimo, entonces pide prestado al hermano o hace merge
        if (padre->children[i]->count < ceil(M/2.0)-1 && !hizoMerge){
          padre->fill(i);     
        }

        //6. Si el padre tiene menos keys del mínimo, entonces pide prestado al hermano o hace merge
        while(padre->count < ceil(M/2.0)-1 && padre!=root){
          NodeB<TK>* ancestro = padre->findParent(root);
          // cout << "a" << endl;
          int posHijo = findChild(ancestro, padre);
          // cout << "b" << endl;
          ancestro->fill(posHijo);
          // cout << "c" << endl;
          padre = padre->findParent(root); //me muevo al padre del padre
          // cout << "Termina fill padre" << endl;
        }
        }
     
      return;
    }

    int findKey(int k){
      int idx=0;
      while (idx<count && keys[idx] < k)
          ++idx;
      return idx;
    }

    NodeB<TK>* findParent(NodeB<TK>*root){
      NodeB<TK>* padre = root; //comienzo siempre en la raíz
      while(!padre->leaf){
      int i = 0;
      while (i < padre->count && this->keys[0] > padre->keys[i])
        i++;
      if(padre->children[i]==this)  //si encuentra a un padre que tenga entre sus hijos al nodo que contiene el key sale del while
          break;
      padre = padre->children[i];  
      }
      return padre;
    }

    int findChild(NodeB<TK>* padre, NodeB<TK>* hijo){
       int i = 0;
        while (i < padre->count && hijo->keys[0] > padre->keys[i]) //creo que esto es innecesario pero lo dejo por si acaso
          i++;  
      return i;
    }

    void fill(int idx){
      // If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
      // from that child
      if (idx!=0 && children[idx-1]->count>=ceil(M/2.0)){
        borrowFromPrev(idx);
      }
  
      // If the next child(C[idx+1]) has more than t-1 keys, borrow a key
      // from that child
      else if (idx!=count && children[idx+1]->count>=ceil(M/2.0)){
        borrowFromNext(idx);
      }
  
      // Merge C[idx] with its sibling
      // If C[idx] is the last child, merge it with its previous sibling
      // Otherwise merge it with its next sibling
      else
      {
          if (idx != count){
            // cout << "merge con el siguiente hermano" << endl;
            merge(idx);
          }
          else{
            // cout << "merge con el anterior hermano" << endl;
            merge(idx-1);
          }
      }
      return;

  }

  void borrowFromPrev(int idx)
  {
  
      NodeB<TK> *child=children[idx];
      NodeB<TK> *sibling=children[idx-1];
  
      // The last key from C[idx-1] goes up to the parent and key[idx-1]
      // from parent is inserted as the first key in C[idx]. Thus, the  loses
      // sibling one key and child gains one key
  
      // Moving all key in C[idx] one step ahead
      for (int i=child->count-1; i>=0; --i)
          child->keys[i+1] = child->keys[i];
  
      // If C[idx] is not a leaf, move all its child pointers one step ahead
      if (!child->leaf)
      {
          for(int i=child->count; i>=0; --i)
              child->children[i+1] = child->children[i];
      }
  
      // Setting child's first key equal to keys[idx-1] from the current NodeB
      child->keys[0] = keys[idx-1];
  
      // Moving sibling's last child as children[idx]'s first child
      if(!child->leaf)
          child->children[0] = sibling->children[sibling->count];
  
      // Moving the key from the sibling to the parent
      // This reduces the number of keys in the sibling
      keys[idx-1] = sibling->keys[sibling->count-1];
  
      child->count += 1;
      sibling->count -= 1;
  
      return;
  }

  void borrowFromNext(int idx){
    NodeB<TK> *child=children[idx];
    NodeB<TK> *sibling=children[idx+1];
 
    // keys[idx] is inserted as the last key in C[idx]
    child->keys[(child->count)] = keys[idx];
 
    // Sibling's first child is inserted as the last child
    // into C[idx]
    if (!(child->leaf))
        child->children[(child->count)+1] = sibling->children[0];
 
    //The first key from sibling is inserted into keys[idx]
    keys[idx] = sibling->keys[0];
 
    // Moving all keys in sibling one step behind
    for (int i=1; i<sibling->count; ++i)
        sibling->keys[i-1] = sibling->keys[i];
 
    // Moving the child pointers one step behind
    if (!sibling->leaf)
    {
        for(int i=1; i<=sibling->count; ++i)
            sibling->children[i-1] = sibling->children[i];
    }
 
    // Increasing and decreasing the key count of C[idx] and C[idx+1]
    // respectively
    child->count += 1;
    sibling->count -= 1;
 
    return;
  }

  void removeFromNext(int idx){
    NodeB<TK> *child=children[idx];
    NodeB<TK> *sibling=children[idx+1];
 
    // keys[idx] is inserted as the last key in C[idx]
    child->keys[(child->count)] = keys[idx];
 
    // Sibling's first child is inserted as the last child
    // into C[idx]
    if (!(child->leaf))
        child->children[(child->count)+1] = sibling->children[0];
 
    //The first key from sibling is inserted into keys[idx]
    keys[idx] = sibling->keys[0];
 
    // Moving all keys in sibling one step behind
    for (int i=1; i<sibling->count; ++i)
        sibling->keys[i-1] = sibling->keys[i];
 
    // Moving the child pointers one step behind
    if (!sibling->leaf)
    {
        for(int i=1; i<=sibling->count; ++i)
            sibling->children[i-1] = sibling->children[i];
    }
 
    // Increasing and decreasing the key count of C[idx] and C[idx+1]
    // respectively
    child->n += 1;
    sibling->n -= 1;
 
    return;
  }

  void merge(int index){
      NodeB<TK> *hijo = children[index];
      NodeB<TK> *hermano = children[index + 1];

      //1. Agrego la key del padre al hijo
      hijo->keys[hijo->count] = keys[index];
      hijo->count++;

      if(!hijo->leaf){
      }

      //2. Agrego las keys del hermano al hijo
      for (int i = 0; i < hermano->count; i++) {
          hijo->keys[hijo->count] = hermano->keys[i];
          hijo->count++;
      }

      //3. Agrego los hijos del hermano al hijo
      if (!hijo->leaf) {
          for (int i = 0; i <= hermano->count; i++) {
              // cout << "Hijo " << i << " del hermano: " << endl;
              // (hermano->children[i])->display();
              hijo->children[hijo->count+i] = hermano->children[i];
              
          }
      }

      //4. Elimino la key del padre que se movió al hijo
      for (int i = index + 1; i < count; i++)
          keys[i - 1] = keys[i];


      //5. Elimino el índice del hermano
      for (int i = index + 2; i <= count; i++){
          //cout << "valor de i: " << i << endl;
          children[i - 1] = children[i];
      }

      //6. Actualizo el número de keys del padre
      count--;


      delete(hermano);
      return;
  }

  void removeFromLeaf(int idx){
    // cout << "Elimina de hoja" << endl;
    // Move all the keys after the idx-th pos one place backward
    for (int i=idx+1; i<count; ++i){
      // cout << "entra"<<endl;
      keys[i-1] = keys[i];
    }
 
    // Reduce the count of keys
    count--;
    return;
  }

   void removeFromNonLeaf(int index, NodeB* root, bool &hizoMerge){ // idx=0
      TK k = keys[index];
      // caso 2 - hijo izquierdo tiene al menos M/2 claves -> reemplazar el key con el predecesor
      if (children[index]->count >= ceil(M/2.0)) {
          // cout << "El hermano izquierdo presta" << endl;
          TK pred = getPred(index);
          this->remove(pred);
          keys[index] = pred;
      }
      // caso 3 - hijo derecho tiene al menos M/2 claves -> reemplazar el key con el sucesor
      else if (children[index + 1]->count >= ceil(M/2.0)) {
          // cout << "El hermano derecho presta" << endl;
          TK succ = getSucc(index);
          // cout << "Sucesor: " << succ << endl;
          this->remove(succ); //eliminar el sucesor del hijo derecho
          keys[index] = succ;
      }
      // caso 4 - Si no, fusionar con el hijo derecho
      else {
          hizoMerge = true;
          merge(index);
          root->remove(k);
      }
  }

  TK getPred(int index){
    NodeB<TK> *hijo = children[index];
    while (!hijo->leaf)
        hijo = hijo->children[hijo->count];
    return hijo->keys[hijo->count - 1];
  }

  TK getSucc(int index){
      NodeB<TK> *hijo = children[index + 1];
      while (!hijo->leaf)
          hijo = hijo->children[0];
      return hijo->keys[0];
  }
};


// BTree
template <typename TK>
class BTree {
private:
  NodeB<TK>* root;
  int M;  // grado u orden del arbol
  int n; // total de elementos en el arbol 

public:
  // Constructor
  BTree(int _M) : root(nullptr), M(_M), n(0) {}

  bool search(TK key){ //indica si se encuentra o no un elemento
    if (root == nullptr)
      return false;
    
    if(root->search(key) == nullptr)
      return false;
    
    return true;
  }

  void insert(TK k){ //inserta un elemento
    if (root == nullptr) {
      // Allocate memory for root
      root = new NodeB<TK>(M, true);
      root->keys[0] = k; // Insert key
      root->count = 1; // Update number of keys in root
    }
    else {
      //busco el nodo en el que se debe insertar
      NodeB<TK>* nodo = root;
      while(!nodo->leaf){
        int i = 0;
        while (i < nodo->count && k > nodo->keys[i])
          i++;
        nodo = nodo->children[i];
      }

      // If NodeB is full, then split NodeB
      if (nodo->count == M-1){
        int it = 0;
        do {
          if (nodo == root){ //si el nodo es la raíz
            // Allocate memory for new root
            NodeB<TK> *s = new NodeB<TK>(M, false);

            // Make old root as child of new root
            s->children[0] = root;

            // Split the old root and move 1 key to the new root
            s->splitChild(0, root,k,it); //0 porque la raiz actual es el hijo 0 de la nueva raíz

            // // New root has two children now. Decide which of the
            // // two children is going to have new key
            // int i = 0;
            // if (s->keys[0] < k)
            //   i++;
            // s->children[i]->insertNonFull(k);

            // Change root
            root = s;

          }
          else{ //Si el nodo en el que se inserta no es la raíz
            //busco el nodo padre del nodo que se está llenando
            NodeB<TK>* padre = root;
            
            while(!padre->leaf){
              int i = 0;
              while (i < padre->count && nodo->keys[0] > padre->keys[i])
                i++;
              if(padre->children[i]==nodo)
                break;
              padre = padre->children[i];
            }

            //busco a cual hijo del padre pertenece el nodo que se está llenando
            int i = 0;
            while (i < padre->count && nodo->keys[0] > padre->keys[i])
              i++;

            // Split the NodeB and move 1 key to the parent
            padre->splitChild(i, nodo,k,it); //i porque el nodo actual es el hijo i del padre. Si es la primera it. agrega el key

            // Change NodeB
            nodo = padre;
          }
          it++;
        }
        while(nodo->count==M); //si despues del split el padre quedó lleno, se repite el proceso para el padre

      }
      else // If NodeB is not full, call insertNonFull for root
        root->insertNonFull(k);
    }
    n++;
  }

  int height(){ // Altura
    if (root == nullptr)
      return 0;

    //llama la funcion privada empezando desde la raiz
    return height(root);
  }
  
  //elimina un elemento
  void remove(TK key){
     if (!root) //si el arbol esta vacio
    {
        cout << "The tree is empty\n";
        return;
    }
    else if(!search(key)){ //si no se encuentra el elemento
      cout << "The key is not in the tree\n";
      return;
    }
 
    // Call the remove function for root
    root->remove(key);
 
    // If the root NodeB has 0 keys, make its first child as the new root
    //  if it has a child, otherwise set root as NULL
    if (root->count == 0)
    {
      NodeB<TK> *tmp = root;
      if (root->leaf)
        root = nullptr;
      else
        root = root->children[0];

      delete tmp;
    }
    n--;
    return;
  }
  int height(NodeB<TK>* NodeB) {
    if(NodeB->leaf){
      return 0;
    }
    int h = 0;
    //recorre todos los hijos del nodo
    for (int i = 0; i < NodeB->count + 1; i++) {
      h = max(h, height(NodeB->children[i]));
      //tomando el maximo de las alturas de los hijos
    }
    //retorna la altura del nodo hijo mas alto mas 1
    return h + 1;
  }
  
  // recorrido inorder
  string toString(string sep = " ")
	{
    string cadena;
    if (root != nullptr)
      root->inOrder(cadena, sep);
    //elimino separación sobrante del final
    cadena = cadena.substr(0, cadena.length() - sep.length());
    return cadena;
  }

  // minimo valor de la llave en el arbol
  TK minKey(){
    return root->minKey();
  }
  // maximo valor de la llave en el arbol
  TK maxKey(){
    return root->maxKey();
  } 

  // retorna el total de elementos insertados  
  int size(){
    return n;
  }

  ~BTree(){
    clear(root);
    root = nullptr;
  }

  void clear() {
    clear(root);
    root = nullptr;
    n = 0;
  }

  void clear(NodeB<TK>* NodeB) {    // eliminar todos lo elementos del arbol
    // if (NodeB == nullptr) {
    //   return;
    // }
    // else if(NodeB->leaf){
    //   delete NodeB;
    //   return;
    // }
    // else{
    //   for (int i = 0; i < NodeB->count + 1; i++) {
    //     clear(NodeB->children[i]);
    //   }
    //   delete NodeB;
    // }
    //NodeB = nullptr;
    root->killSelf();
  }
};

#endif
