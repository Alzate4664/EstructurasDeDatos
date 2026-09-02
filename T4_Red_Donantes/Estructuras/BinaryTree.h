/*
 * ============================================================================
 * Código de Aplicación / Estructura de Datos
 * 
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * Curso: Estructuras de Datos y Algoritmos (EDA)
 * Institución: Universidad EAFIT
 * ============================================================================
 * Este código forma parte de las soluciones algorítmicas y estructuras de datos
 * desarrolladas para la resolución de casos de estudio hospitalarios (T4).
 * Contiene comentarios detallados sobre la complejidad y el funcionamiento.
 * ============================================================================
 */
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
