#ifndef STACK_H
#define STACK_H
#include "Person.h"

/*
 * DEFINICIÓN: Pila (Stack)
 * Estructura de datos abstracta que sigue la política LIFO (Last In, First Out).
 * El último elemento en entrar es el primero en salir (como una pila de platos).
 * 
 * COMPLEJIDAD:
 * - Push (Insertar): O(1)
 * - Pop (Extraer): O(1)
 * 
 * USO MÉDICO: Auditoría de los pacientes más recientes que ingresaron al hospital.
 */

struct StackNode {
    Person data;
    StackNode* next; // En una Pila, 'next' apunta al nodo que está "debajo"
    StackNode(Person p);
};

class Stack {
public:
    StackNode* topNode; // Puntero al elemento en la cima
    Stack();
    ~Stack();
    void push(Person p); // Equivalente a 'insertar' pero siempre en la cima
};
#endif