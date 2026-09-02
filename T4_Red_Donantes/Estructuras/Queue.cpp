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
#include "Queue.h"

QueueNode::QueueNode(Person p) : data(p), next(nullptr) {}

Queue::Queue() : frontNode(nullptr), rearNode(nullptr) {}

Queue::~Queue() {
    while (frontNode != nullptr) {
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
    }
}

// Enqueue: Forma a un nuevo paciente al final de la fila en O(1)
void Queue::enqueue(Person p) {
    QueueNode* newNode = new QueueNode(p);
    
    // Si no hay nadie en la fila, el frente y el final son la misma persona
    if (!rearNode) { 
        frontNode = rearNode = newNode; 
        return; 
    }
    
    // El último actual apunta al recién llegado
    rearNode->next = newNode;
    
    // El recién llegado se convierte en el nuevo final de la fila
    rearNode = newNode;
}