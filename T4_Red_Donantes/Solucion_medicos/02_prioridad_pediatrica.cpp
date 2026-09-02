#include <iostream>

/*
 * ============================================================================
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * ============================================================================
 * Escenario 2: Prioridad Pediátrica Urgente
 * Paradigma: Divide y Vencerás / Búsqueda Binaria
 * Estructura de Datos: Árbol Binario de Búsqueda (BST)
 * ============================================================================
 * 
 * Se requiere encontrar rápidamente a un paciente infantil. Un BST organizado
 * por edad permite ignorar mitades enteras del árbol en cada paso.
 * Los menores están a la izquierda y los mayores a la derecha. 
 * El tiempo de búsqueda se reduce de O(N) a O(log N).
 */

struct NodoPaciente {
    int id;
    int edad;
    NodoPaciente* izquierdo; // Subárbol con pacientes de menor edad
    NodoPaciente* derecho;   // Subárbol con pacientes de mayor edad
};

// Función recursiva que aplica Divide y Vencerás para hallar al paciente
NodoPaciente* buscarPacientePediatrico(NodoPaciente* raiz) {
    // Caso base: llegamos a una hoja sin encontrar el objetivo
    if (raiz == nullptr) return nullptr;
    
    // Condición de éxito: encontramos a alguien menor de 18 años
    if (raiz->edad < 18) return raiz; 
    
    // Si la edad actual es 18 o mayor, descartamos a todos los mayores (rama derecha)
    // y nos adentramos únicamente en la rama izquierda (menores).
    return buscarPacientePediatrico(raiz->izquierdo);
}

int main() {
    // Construimos un Árbol Binario de Búsqueda (BST) indexado por edad
    NodoPaciente n1 = {1, 14, nullptr, nullptr}; // Objetivo (Infantil)
    NodoPaciente n2 = {2, 35, nullptr, nullptr}; // Adulto joven
    NodoPaciente n3 = {3, 30, &n1, &n2};         // Nodo intermedio
    NodoPaciente n4 = {4, 60, nullptr, nullptr}; // Adulto mayor
    NodoPaciente raiz = {5, 45, &n3, &n4};       // Raíz del árbol

    std::cout << "Buscando paciente pediátrico en base masiva...\n";
    NodoPaciente* resultado = buscarPacientePediatrico(&raiz);
    
    if (resultado) {
        std::cout << "¡Éxito! Paciente pediátrico encontrado con ID: " 
                  << resultado->id << " (Edad: " << resultado->edad << " años)" << std::endl;
    } else {
        std::cout << "No hay pacientes pediátricos en el registro." << std::endl;
    }
    
    return 0;
}
