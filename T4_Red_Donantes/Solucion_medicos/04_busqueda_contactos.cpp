#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

void buscarContactosBFS(int paciente_cero, std::vector<std::vector<int>>& red) {
    std::queue<int> cola;
    std::unordered_set<int> visitados;
    
    cola.push(paciente_cero);
    visitados.insert(paciente_cero);
    
    while (!cola.empty()) {
        int actual = cola.front();
        cola.pop();
        
        std::cout << "Evaluando paciente: " << actual << "\n";
        
        for (int contacto : red[actual]) {
            if (visitados.find(contacto) == visitados.end()) {
                visitados.insert(contacto);
                cola.push(contacto); // Se encola para explorar su capa después
            }
        }
    }
}

int main() {
    // Ejemplo de uso: Grafo de contactos (0 es el paciente cero)
    std::vector<std::vector<int>> red_contactos = {
        {1, 2, 3}, // Contactos del paciente 0
        {0, 4, 5}, // Contactos del paciente 1
        {0},       // Contactos del paciente 2
        {0},       // Contactos del paciente 3
        {1},       // Contactos del paciente 4
        {1}        // Contactos del paciente 5
    };

    std::cout << "Buscando contactos del paciente 0 (BFS):\n";
    buscarContactosBFS(0, red_contactos);
    return 0;
}
