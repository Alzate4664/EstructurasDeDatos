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