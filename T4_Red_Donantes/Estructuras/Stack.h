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