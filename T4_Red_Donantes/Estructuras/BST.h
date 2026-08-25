#ifndef BST_H
#define BST_H
#include "Person.h"

/*
 * DEFINICIÓN: Árbol Binario de Búsqueda (Binary Search Tree - BST)
 * Estructura jerárquica no lineal. Cada nodo tiene máximo dos hijos (izquierdo y derecho).
 * Propiedad clave: Los valores menores van a la rama izquierda, los mayores a la rama derecha.
 * 
 * COMPLEJIDAD:
 * - Inserción/Búsqueda: Promedio O(log n), Peor caso O(n) si está desbalanceado.
 * 
 * USO MÉDICO: Organizar automáticamente a los pacientes por su EDAD para priorizar
 * rápidamente a niños o adultos mayores sin tener que usar un algoritmo de ordenamiento tradicional.
 */

struct TreeNode {
    Person data;
    TreeNode* left;  // Hijo menor
    TreeNode* right; // Hijo mayor
    TreeNode(Person p);
};

class BST {
public:
    TreeNode* root; // Raíz principal del árbol
    
    BST();
    ~BST();
    void insertar(Person p); // Inserta un paciente ruteándolo por edad
    
    // Funciones recursivas internas
    void insertarNodo(TreeNode*& node, Person p);
    void destruirArbol(TreeNode* node);
};
#endif