#include <iostream>

/*
 * ============================================================================
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * ============================================================================
 * Escenario 1: Auditoría Exhaustiva
 * Paradigma: Fuerza Bruta / Búsqueda Lineal
 * Estructura de Datos: Lista Ligada (Simple)
 * ============================================================================
 * 
 * En este problema necesitamos revisar cada expediente médico uno por uno
 * ya que no tenemos un índice que nos diga dónde está la anomalía.
 * La búsqueda lineal en una lista ligada es O(N) porque visita cada nodo.
 */

struct Expediente {
    int id_paciente;
    bool es_anomalo;
    Expediente* siguiente; // Puntero al siguiente expediente (Lista Ligada)
};

// Función que busca secuencialmente en la lista ligada
Expediente* buscarCasoAnomalo(Expediente* cabeza) {
    Expediente* actual = cabeza; // Empezamos por el primer elemento (cabeza)
    
    // Recorremos hasta llegar al final de la lista (nullptr)
    while (actual != nullptr) {
        if (actual->es_anomalo) {
            // Retornamos el primer caso anómalo que encontremos
            return actual; 
        }
        // Avanzamos al siguiente nodo en la memoria
        actual = actual->siguiente;
    }
    
    // Si iteramos por toda la lista y no encontramos nada, devolvemos NULO
    return nullptr; 
}

int main() {
    // Ejemplo de uso: Creamos una lista ligada de expedientes
    Expediente exp3 = {3, true, nullptr}; // Caso anómalo al final
    Expediente exp2 = {2, false, &exp3};  // Enlaza con exp3
    Expediente exp1 = {1, false, &exp2};  // Enlaza con exp2 (Cabeza de lista)

    std::cout << "Iniciando auditoría exhaustiva...\n";
    Expediente* resultado = buscarCasoAnomalo(&exp1);
    
    if (resultado) {
        std::cout << "Caso anómalo encontrado en paciente ID: " << resultado->id_paciente << std::endl;
    } else {
        std::cout << "No se encontraron anomalías en el historial." << std::endl;
    }
    
    return 0;
}
