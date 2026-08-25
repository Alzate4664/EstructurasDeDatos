#include "Graph.h"

GraphNode::GraphNode(Person p) : data(p) {}

Graph::Graph() {}

Graph::~Graph() {
    // Liberar la memoria de todos los vértices creados
    for (auto const& pair : vertices) {
        delete pair.second;
    }
}

// Agrega un paciente como un nodo (Vértice) aislado a la red
void Graph::addPerson(Person p) {
    // Solo lo añadimos si no existe ya en el diccionario (map)
    if (vertices.find(p.nombre) == vertices.end()) {
        vertices[p.nombre] = new GraphNode(p);
    }
}

// Interpreta la información médica y "dibuja" los cables (Aristas) en la red
void Graph::buildCompatibilityTies() {
    for (auto const& pair : vertices) {
        GraphNode* node = pair.second;
        
        // Iteramos sobre los nombres (strings) que el paciente dice ser compatible
        for (const std::string& comp : node->data.compatibles) {
            
            // Si el paciente compatible existe en la red del hospital, creamos un enlace directo
            if (vertices.find(comp) != vertices.end()) {
                node->matches.push_back(vertices[comp]);
            }
        }
    }
}