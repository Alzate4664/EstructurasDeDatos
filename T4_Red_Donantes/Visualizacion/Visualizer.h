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
#ifndef VISUALIZER_H
#define VISUALIZER_H
#include <string>
#include "Person.h"
#include "LinkedList.h"
#include "DoublyLinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "BST.h"
#include "HashTable.h"
#include "Graph.h"
#include "BinaryTree.h"

/*
 * DEFINICIÓN: Módulo Visualizador (Patrón Arquitectónico de Capas)
 * Separa de manera limpia la manipulación de datos (Capa de Modelo)
 * de su dibujo y estética en la terminal (Capa de Vista).
 * Las estructuras de datos puras no deben tener instrucciones de consola (cout).
 */

namespace Visualizer {
    void printHeader(const std::string& title);
    void printPerson(const Person& p);
    void printLinkedList(const LinkedList& list);
    void printDoublyLinkedList(const DoublyLinkedList& list);
    void printStack(const Stack& stack);
    void printQueue(const Queue& queue);
    void printBST(const BST& bst);
    void printBinaryTree(const BinaryTree& bt);
    void printHashTable(const HashTable& ht);
    void printGraph(const Graph& graph);
}
#endif