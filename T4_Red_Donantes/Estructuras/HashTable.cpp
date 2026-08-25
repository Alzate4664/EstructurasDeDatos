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