#include <iostream>

struct Expediente {
    int id_paciente;
    bool es_anomalo;
    Expediente* siguiente;
};

Expediente* buscarCasoAnomalo(Expediente* cabeza) {
    Expediente* actual = cabeza;
    while (actual != nullptr) {
        if (actual->es_anomalo) {
            return actual; // Se encontró la anomalía
        }
        actual = actual->siguiente;
    }
    return nullptr; // No se encontró
}

int main() {
    // Ejemplo de uso
    Expediente exp3 = {3, true, nullptr};
    Expediente exp2 = {2, false, &exp3};
    Expediente exp1 = {1, false, &exp2};

    Expediente* resultado = buscarCasoAnomalo(&exp1);
    if (resultado) {
        std::cout << "Caso anómalo encontrado en paciente ID: " << resultado->id_paciente << std::endl;
    } else {
        std::cout << "No se encontraron anomalías." << std::endl;
    }
    return 0;
}
