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