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
#include "Stack.h"

StackNode::StackNode(Person p) : data(p), next(nullptr) {}

Stack::Stack() : topNode(nullptr) {}

Stack::~Stack() {
    while (topNode != nullptr) {
        StackNode* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }
}

// Push: Inserta un nuevo elemento exactamente en la cima de la pila en O(1)
void Stack::push(Person p) {
    StackNode* newNode = new StackNode(p);
    
    // El nuevo nodo apunta hacia abajo, hacia la cima actual
    newNode->next = topNode;
    
    // La cima se actualiza para ser el nuevo nodo
    topNode = newNode;
}