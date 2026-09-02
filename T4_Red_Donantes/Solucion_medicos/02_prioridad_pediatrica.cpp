#include <iostream>

struct NodoPaciente {
    int id;
    int edad;
    NodoPaciente* izquierdo;
    NodoPaciente* derecho;
};

NodoPaciente* buscarPacientePediatrico(NodoPaciente* raiz) {
    if (raiz == nullptr) return nullptr;
    
    if (raiz->edad < 18) return raiz; // Objetivo encontrado
    
    // Como es un BST ordenado por edad, los menores están a la izquierda
    return buscarPacientePediatrico(raiz->izquierdo);
}

int main() {
    // Ejemplo de uso
    NodoPaciente n1 = {1, 14, nullptr, nullptr};
    NodoPaciente n2 = {2, 35, nullptr, nullptr};
    NodoPaciente n3 = {3, 30, &n1, &n2};
    NodoPaciente n4 = {4, 60, nullptr, nullptr};
    NodoPaciente raiz = {5, 45, &n3, &n4};

    NodoPaciente* resultado = buscarPacientePediatrico(&raiz);
    if (resultado) {
        std::cout << "Paciente pediátrico encontrado con ID: " << resultado->id << " (Edad: " << resultado->edad << ")" << std::endl;
    } else {
        std::cout << "No hay pacientes pediátricos." << std::endl;
    }
    return 0;
}
