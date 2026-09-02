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
#include "HashTable.h"

// Función Hash: Suma el valor ASCII de las letras del nombre y saca el residuo (Módulo)
int HashTable::hashFunction(const std::string& name) const {
    int hash = 0;
    for (char ch : name) {
        hash += ch; // Acumulador ASCII
    }
    return hash % tableSize; // Limita el resultado al tamaño de la tabla (0-9)
}

void HashTable::insertar(Person p) {
    // Calculamos su índice único basado en el nombre
    int index = hashFunction(p.nombre);
    
    // Lo guardamos en el 'bucket' correspondiente usando push_back de std::list
    table[index].push_back(p);
}