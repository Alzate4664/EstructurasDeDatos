#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "Person.h"
#include <list>
#include <string>

/*
 * DEFINICIÓN: Tabla Hash (Hash Table)
 * Estructura de mapeo directo que convierte una "llave" (Ej. el Nombre) en un 
 * "índice" de arreglo numérico usando una Función Matemática (Hash Function).
 * 
 * Resolución de colisiones: Encadenamiento Secuencial (Chaining). Si dos nombres 
 * caen en el mismo índice, se guardan en una lista ligada (std::list) en ese cajón (Bucket).
 * 
 * COMPLEJIDAD:
 * - Búsqueda/Inserción: Promedio O(1), Peor caso O(n) si todos chocan.
 * 
 * USO MÉDICO: Búsqueda ultra-rápida (indexada) del expediente de un paciente por su nombre.
 */

class HashTable {
public:
    static const int tableSize = 10; // Número de "cajones" o Buckets
    std::list<Person> table[tableSize]; // Arreglo de listas (Chaining)
    
    // Convierte el Nombre en un número del 0 al 9
    int hashFunction(const std::string& name) const;
    void insertar(Person p);
};
#endif