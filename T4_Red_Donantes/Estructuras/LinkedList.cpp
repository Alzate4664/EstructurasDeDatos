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
#include "LinkedList.h"

// Constructor del Nodo: Inicializa los datos y pone el puntero siguiente a nulo (fin temporal de cadena)
ListNode::ListNode(Person p) : data(p), next(nullptr) {}

// Constructor de la Lista: Inicialmente está vacía (head = nullptr)
LinkedList::LinkedList() : head(nullptr) {}

// Destructor: Recorre la lista liberando la memoria dinámica RAM de cada nodo
LinkedList::~LinkedList() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
}

// Inserción Secuencial (Al final)
void LinkedList::insertar(Person p) {
    ListNode* newNode = new ListNode(p);
    
    // CASO 1: Si la lista está vacía, el nuevo nodo se convierte en la cabeza
    if (!head) { 
        head = newNode; 
        return; 
    }
    
    // CASO 2: Si ya hay elementos, recorremos la lista hasta llegar al último nodo
    ListNode* temp = head;
    while (temp->next) {
        temp = temp->next;
    }
    
    // Conectamos el último nodo con el nuevo
    temp->next = newNode;
}