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
#include "DoublyLinkedList.h"

DLLNode::DLLNode(Person p) : data(p), next(nullptr), prev(nullptr) {}

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr) {}

DoublyLinkedList::~DoublyLinkedList() {
    DLLNode* temp = head;
    while (temp != nullptr) {
        DLLNode* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }
}

// Inserción en tiempo constante O(1) usando el puntero tail
void DoublyLinkedList::insertar(Person p) {
    DLLNode* newNode = new DLLNode(p);
    
    // Si la lista está vacía, cabeza y cola apuntan al mismo nuevo nodo
    if (!head) { 
        head = tail = newNode; 
        return; 
    }
    
    // Enlace bidireccional
    tail->next = newNode; // La antigua cola apunta al nuevo nodo
    newNode->prev = tail; // El nuevo nodo apunta hacia atrás a la antigua cola
    tail = newNode;       // Actualizamos la cola para que sea el nuevo nodo
}