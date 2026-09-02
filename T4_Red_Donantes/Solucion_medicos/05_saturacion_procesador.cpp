#include <iostream>
#include <unordered_map>
#include <thread>
#include <chrono>

/*
 * ============================================================================
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * ============================================================================
 * Escenario 5: Saturación del Procesador
 * Paradigma: Memorización (Dynamic Programming) / Sistema de Caché
 * Estructura de Datos: Tabla Hash (Mapeo Indexado)
 * ============================================================================
 * 
 * Si un cálculo médico gasta excesivo poder de CPU y se repite de forma
 * idéntica para el mismo paciente, podemos usar una Tabla Hash para 
 * almacenar el resultado previo y retornarlo instantáneamente O(1) a futuro,
 * aliviando los servidores hospitalarios.
 */

// Memoria caché centralizada en el sistema. Map de <ID Paciente, Riesgo>
std::unordered_map<int, double> cacheRiesgos;

// Función que simula ser muy exigente a nivel de procesamiento
double calculoPesado(int id) {
    // Duerme el hilo para simular que toma minutos en resolver
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    return 85.5 + (id % 10); 
}

// Función orquestadora que decide si hacer el cálculo o sacar de la caché
double obtenerRiesgoMortalidad(int id_paciente) {
    
    // Revisamos si el paciente ya está en la Tabla Hash
    if (cacheRiesgos.find(id_paciente) != cacheRiesgos.end()) {
        std::cout << "Recuperado de caché (instantáneo) para ID " << id_paciente << ".\n";
        return cacheRiesgos[id_paciente]; // Devolución O(1)
    }
    
    // Si no está, ejecutamos el código pesado que toma mucho tiempo
    std::cout << "Calculando riesgo por primera vez para ID " << id_paciente << "...\n";
    double riesgo = calculoPesado(id_paciente);
    
    // Memorizamos (guardamos) el resultado en la Tabla Hash para el futuro
    cacheRiesgos[id_paciente] = riesgo; 
    
    return riesgo;
}

int main() {
    std::cout << "Consulta 1:\n";
    std::cout << "Riesgo -> " << obtenerRiesgoMortalidad(1024) << "\n\n";
    
    std::cout << "Consulta 2 (Repetida - debería ser instantánea):\n";
    std::cout << "Riesgo -> " << obtenerRiesgoMortalidad(1024) << "\n\n";
    
    std::cout << "Consulta 3 (Nuevo paciente):\n";
    std::cout << "Riesgo -> " << obtenerRiesgoMortalidad(1055) << "\n";
    
    return 0;
}
