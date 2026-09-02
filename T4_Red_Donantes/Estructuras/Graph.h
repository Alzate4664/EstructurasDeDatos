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
#ifndef GRAPH_H
#define GRAPH_H
#include "Person.h"
#include <string>
#include <vector>
#include <map>

/*
 * DEFINICIÓN: Grafo (Red) No Dirigido o Dirigido
 * Estructura matemática que modela relaciones (Aristas) entre objetos (Vértices).
 * Aquí usamos una representación de "Lista de Adyacencia".
 * 
 * COMPLEJIDAD:
 * - Recorrido general (DFS/BFS): O(V + E) donde V = Vértices, E = Aristas.
 * 
 * USO MÉDICO: Motor de emparejamiento (Matchmaking). Analiza de forma cruzada 
 * quién es compatible con quién para generar cadenas de donación de múltiples órganos.
 */

struct GraphNode {
    Person data;
    std::vector<GraphNode*> matches; // Aristas: Punteros a los vértices compatibles
    GraphNode(Person p);
};

class Graph {
public:
    // Mapea el Nombre (std::string) al Nodo del Grafo correspondiente para búsquedas eficientes O(log n)
    std::map<std::string, GraphNode*> vertices;
    
    Graph();
    ~Graph();
    
    // Paso 1: Añadir vértices desconectados
    void addPerson(Person p);
    
    // Paso 2: Dibujar las aristas basados en el vector de strings 'compatibles'
    void buildCompatibilityTies();
};
#endif