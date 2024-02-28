[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=10055880&assignment_repo_type=AssignmentRepo)


# Project BLOCKCHAIN

![BlockChain by DALL·E](dalleBlock1.png "Diseño de BlockChain generado por modelo DALL·E")

*Generado por sistema CLIP DALL-E-*

## Tabla de Contenidos

## Integrantes

| Nombres y Apellidos                    | Participación    |
|--------------------------------------- | ---------------- |
| Murakami Miyahira, Mitsuo  Sebastián   | 100%             |
| Romani Tafur, Mariajulia Romina        | 100%             |
| Riveros Rodriguez, Yared               | 100%             |
| Rodriguez Valverde, Camila Rosa        | 100%             |



## Introducción

Este proyecto tiene como objetivo desarrollar una aplicación transaccional que permita la interacción de datos de forma segura mediante el uso de tecnología blockchain. Para lograrlo, se implementaron varias estructuras de datos abstractas personalizadas, cuidadosamente seleccionadas por su idoneidad y sus beneficios teóricos.

La seguridad y confiabilidad de la aplicación son esenciales, por lo que se eligió una estructura de datos de cadena de bloques para garantizar la integridad de los datos y la inmutabilidad de las transacciones. Además, las estructuras de datos customizadas permiten una mayor eficiencia en la interacción de los datos, mejorando la escalabilidad y el rendimiento del sistema.

En resumen, este proyecto representa una oportunidad para explorar las ventajas de la tecnología blockchain y su aplicación en una aplicación transaccional segura y eficiente.



## Descripción
Este proyecto es una implementación básica de una cadena de bloques que registra transferencias y busca información en la cadena.

El programa lee información de un archivo CSV que contiene transferencias bancarias y los almacena en una estructura de datos de tipo CircularArray. Una vez que se alcanza el número de transferencias necesario, se crea un nuevo bloque con ellas y se agrega a la cadena de bloques.

Además, el programa proporciona la capacidad de buscar en la cadena de bloques en función de ciertos criterios, como la cantidad de transferencias entre un rango de fechas, aquellas que contienen un cierto prefijo o el monto mínimo y máximo de una transferencia.

El programa también incluye un menú que permite al usuario buscar, eliminar y minar nuevos bloques en la cadena

## Relevancia

Por medio de este proyecto, se puede conocer a mayor detalle en la práctica el uso de las estructuras de datos aprendidas en el curso, así como el lidiar con debilidades que algunas presentan en los casos de implementación; en resumen a través del desarrollo de este, se llega a conocer cabalmente los pormenores de las estructuras de datos.

## Pasos para ejecutar el programa.
    
    loading...

### Desde Terminal

    loading...

### Utilizando Interfaz Gráfica

    loading...

## Explicación de la estructura de datos del Blockchain y la estrategia para asegurar la integridad de su contenido.

El Blockchain es una base de datos distribuida y descentralizada que se utiliza para almacenar información de manera segura y transparente. En ella se registran transacciones, contratos y otros tipos de datos de forma cronológica, lo que permite asegurar su integridad y evitar que se modifiquen o eliminen.

La estructura de datos del Blockchain se organiza en bloques, que contienen información sobre las transacciones realizadas. Cada bloque está conectado al anterior, formando una cadena de bloques (de ahí el nombre de Blockchain). Cada bloque contiene un conjunto de transacciones y un hash, que es un código alfanumérico que se genera a partir de la información contenida en el bloque. Este hash es único para cada bloque y se utiliza para verificar la integridad de la información almacenada en el mismo.

   
### Proof of work

La estrategia utilizada para asegurar la integridad del contenido del Blockchain se conoce como "proof of work" o "prueba de trabajo". Consiste en que los nodos de la red (los ordenadores que participan en el mantenimiento de la red Blockchain) compiten por resolver un complejo problema matemático. 

Este proceso de "prueba de trabajo" es muy difícil y consume mucha energía, lo que hace que sea muy difícil para un atacante manipular la información almacenada en el Blockchain. Además, como cada bloque está conectado al anterior, si alguien intenta modificar un bloque, esto afectará a todos los bloques posteriores, lo que hace que sea muy difícil realizar una modificación sin ser detectado.

En este programa se implementa una versión básica del mecanismo Proof of Work (PoW) para garantizar la seguridad y la inmutabilidad de la cadena de bloques. El proceso de minería se implementa en la función mine() de la clase Block. El objetivo es encontrar un número entero llamado "nonce" tal que, cuando se concatena con la información de la transferencia y se calcula el hash SHA-256 de la cadena resultante, el hash comience con un número determinado de ceros (el prefijo de validación), que por defecto en nuestro programa son 3 ceros. El algoritmo utiliza un bucle while que incrementa el valor del nonce en cada iteración hasta que se encuentra un hash que comienza con el prefijo de validación. Cuando se encuentra un hash válido, se establece como el hash del bloque actual.

En resumen, el proceso de minería en esta implementación Proof of Work se basa en la fuerza bruta para encontrar un valor de nonce que permita generar un hash válido para el bloque, y así garantizar que la información de la cadena de bloques sea inmutable y resistente a la manipulación.

## Estructuras de Datos implementadas

Para poder implementar las cadenas de bloques así como los métodos de las clases utilizadas, se utilizaron las siguientes estructuras de datos:

 - Circular Array

    Uno que contiene los bloques de la cadena.

    ```c++
    CircularArray<T> chain;
    ```

    Uno que contiene todas las transferencias realizadas en el BlockChain.

    ```c++
    CircularArray<Transferencia*> transferencias;
    ```

    

- ChainHash

    Cuatro tablas hash ChainHash que permiten buscar transferencias por fecha, emisor, receptor y monto.

    ```c++
    ChainHash<string, CircularArray<Transferencia*>*> *hashFecha; 
    ChainHash<string, CircularArray<Transferencia*>*> *hashEmisor; 
    ChainHash<string, CircularArray<Transferencia*>*> *hashReceptor; 
    ChainHash<int, CircularArray<Transferencia*>*> *hashMonto; 
    ```

    

- MaxHeap

    Dos árboles MaxHeap que permiten obtener el máximo monto de transferencia y la fecha más reciente respectivamente.

    ```c++
    MaxHeap<int> max; 
    MaxHeap<Fecha> maxFecha; 
    ```

   
    



- MinHeap

     Dos árboles MinHeap que permiten obtener el mínimo monto de transferencia y la fecha más antigua respectivamente.

    ```c++
    MinHeap<Fecha> minFecha; 
    MinHeap<int> min; 
    ```


- Trie Simple

    Dos árboles trie simples que permiten buscar transferencias por emisor y receptor.

    ```c++
    TrieSimple *trieEmisor; 
    TrieSimple *trieReceptor; 
    ```

- Árbol AVL
    
    Dos árboles AVL que permiten buscar transferencias por monto y fecha.

     ```c++
    AVLTree<int> *avlMonto; 
    AVLTree<Fecha> *avlFecha; 
    ```



## Complexity


- insert
    
    loading ...

- search
    
    loading ...


## Conclusion
    loading ...


## Biografy references
    
    Allen Weiss, M. (2014). *Data structures and algorithm analysis in C++* (4th ed.). Pearson Education, Inc.
    
    Antonopoulos, A. M. (2018). *Mastering blockchain: Distributed ledger technology, decentralization, and smart contracts explained* (2nd ed.). O'Reilly Media, Inc.



## Index integrants activity
    
Para la distribución de tareas, se utilizó la sección de *Projects* de GitHub aplicando la plantilla *Basic Kanban*.

- Link directo: [Sección Projects](https://github.com/orgs/utec-cs-aed-2023-0/projects/1/views/1)



