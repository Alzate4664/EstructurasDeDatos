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
#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H
#include "Person.h"

/*
 * DEFINICIÓN: Lista Doblemente Ligada (Doubly Linked List)
 * Similar a la Lista Ligada, pero cada nodo almacena DOS punteros: uno hacia el nodo siguiente
 * y otro hacia el nodo anterior. Esto permite viajar por la lista en ambas direcciones.
 * 
 * COMPLEJIDAD:
 * - Inserción al final: O(1) (Gracias al puntero 'tail')
 * - Recorrido: O(n) en cualquier dirección.
 * 
 * USO MÉDICO: Navegación de registros hacia adelante y hacia atrás.
 */

struct DLLNode {
    Person data;
    DLLNode* next; // Puntero al siguiente
    DLLNode* prev; // Puntero al anterior
    DLLNode(Person p);
};

class DoublyLinkedList {
public:
    DLLNode* head; // Puntero al inicio
    DLLNode* tail; // Puntero al final (Optimiza inserciones)
    
    DoublyLinkedList();
    ~DoublyLinkedList();
    void insertar(Person p);
};
#endif