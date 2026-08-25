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