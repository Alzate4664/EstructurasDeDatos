#ifndef BINARYTREE_H
#define BINARYTREE_H
#include "Person.h"

/*
 * DEFINICIÓN: Árbol Binario (Binary Tree)
 * Estructura jerárquica no lineal. Cada nodo tiene máximo dos hijos.
 * A diferencia del BST, no mantiene un orden específico (se inserta por niveles).
 */

struct BTNode {
    Person data;
    BTNode* left;
    BTNode* right;
    BTNode(Person p);
};

class BinaryTree {
public:
    BTNode* root;
    
    BinaryTree();
    ~BinaryTree();
    void insertar(Person p); // Inserta por niveles (completo)
    
    void destruirArbol(BTNode* node);
};
#endif
