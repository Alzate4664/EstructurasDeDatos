#include <iostream>
#include <unordered_map>
#include <thread>
#include <chrono>

std::unordered_map<int, double> cacheRiesgos;

double calculoPesado(int id) {
    // Simulación de cálculo pesado
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    return 85.5 + (id % 10); 
}

double obtenerRiesgoMortalidad(int id_paciente) {
    if (cacheRiesgos.find(id_paciente) != cacheRiesgos.end()) {
        std::cout << "Recuperado de caché (instantáneo) para ID " << id_paciente << ".\n";
        return cacheRiesgos[id_paciente]; 
    }
    
    std::cout << "Calculando riesgo por primera vez para ID " << id_paciente << "...\n";
    double riesgo = calculoPesado(id_paciente);
    cacheRiesgos[id_paciente] = riesgo; // Memorización
    
    return riesgo;
}

int main() {
    std::cout << "Riesgo ID 1024: " << obtenerRiesgoMortalidad(1024) << "\n";
    std::cout << "Riesgo ID 1024 de nuevo: " << obtenerRiesgoMortalidad(1024) << "\n";
    std::cout << "Riesgo ID 1055: " << obtenerRiesgoMortalidad(1055) << "\n";
    return 0;
}
