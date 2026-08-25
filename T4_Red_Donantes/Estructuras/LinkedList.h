#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Person.h"

/*
 * DEFINICIÓN: Lista Ligada Simple (Singly Linked List)
 * Una colección lineal de nodos donde cada nodo apunta al siguiente mediante un puntero.
 * No tiene tamaño fijo y permite el crecimiento dinámico de la memoria.
 * 
 * COMPLEJIDAD:
 * - Inserción al final: O(n) (Porque hay que recorrerla toda)
 * - Búsqueda: O(n)
 * 
 * USO MÉDICO: Base de datos original y secuencial sin un orden estricto.
 */

// Estructura envoltorio (Wrapper) que conecta los datos con el siguiente nodo
struct ListNode {
    Person data;
    ListNode* next; // Puntero al siguiente elemento de la cadena
    ListNode(Person p);
};

class LinkedList {
public:
    ListNode* head; // Puntero al inicio de la lista
    LinkedList();
    ~LinkedList();
    void insertar(Person p); // Añade al final de la lista
};
#endif