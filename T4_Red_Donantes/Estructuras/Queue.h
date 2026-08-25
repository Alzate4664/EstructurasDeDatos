#ifndef QUEUE_H
#define QUEUE_H
#include "Person.h"

/*
 * DEFINICIÓN: Cola (Queue)
 * Estructura lineal que sigue la política FIFO (First In, First Out).
 * El primer elemento en entrar es el primero en salir (como una fila en el banco).
 * 
 * COMPLEJIDAD:
 * - Enqueue (Encolar al final): O(1)
 * - Dequeue (Desencolar al frente): O(1)
 * 
 * USO MÉDICO: Lista de espera de trasplantes. Se atiende al paciente que lleva más tiempo esperando.
 */

struct QueueNode {
    Person data;
    QueueNode* next; // Apunta al paciente que está detrás en la fila
    QueueNode(Person p);
};

class Queue {
public:
    QueueNode* frontNode; // Frente de la cola (el próximo a ser atendido)
    QueueNode* rearNode;  // Final de la cola (donde se forman los nuevos)
    
    Queue();
    ~Queue();
    void enqueue(Person p);
};
#endif