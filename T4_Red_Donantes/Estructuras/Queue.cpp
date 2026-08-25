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