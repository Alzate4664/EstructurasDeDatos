#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

/*
 * ============================================================================
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * ============================================================================
 * Escenario 4: Búsqueda de Contactos Directos
 * Paradigma: Búsqueda en Anchura (BFS) / Exploración por Niveles
 * Estructura de Datos: Cola (Queue - FIFO)
 * ============================================================================
 * 
 * Ante un virus infeccioso, es prioritario contactar a los directamente
 * expuestos (1er grado) antes de indagar en los expuestos de forma secundaria.
 * Utilizar una Cola (Queue) asegura que procesaremos por capas u "ondas"
 * concéntricas partiendo del paciente cero.
 */

// Grafo representado por Lista de Adyacencia (red)
void buscarContactosBFS(int paciente_cero, std::vector<std::vector<int>>& red) {
    // La cola prioriza el orden FIFO (Primero en entrar, primero en salir)
    std::queue<int> cola;
    
    // Set para evitar procesar al mismo paciente dos veces (Ciclos infinitos)
    std::unordered_set<int> visitados;
    
    // Insertamos el punto de inicio
    cola.push(paciente_cero);
    visitados.insert(paciente_cero);
    
    while (!cola.empty()) {
        // Sacamos de la fila al paciente que le toca evaluación
        int actual = cola.front();
        cola.pop();
        
        std::cout << "Evaluando paciente: " << actual << "\n";
        
        // Iteramos sobre todos sus contactos directos (expansión por vecindad)
        for (int contacto : red[actual]) {
            // Si el contacto aún no ha sido evaluado
            if (visitados.find(contacto) == visitados.end()) {
                visitados.insert(contacto); // Lo marcamos como visto
                cola.push(contacto);        // Lo encolamos al final para explorar su capa después
            }
        }
    }
}

int main() {
    // Ejemplo de uso: Grafo de contactos (índice de fila es el paciente)
    // El paciente 0 tiene exposición directa con 1, 2 y 3.
    std::vector<std::vector<int>> red_contactos = {
        {1, 2, 3}, // Contactos del paciente 0 (Capa 1)
        {0, 4, 5}, // Contactos del paciente 1 (Capa 2)
        {0},       // Contactos del paciente 2 
        {0},       // Contactos del paciente 3
        {1},       // Contactos del paciente 4
        {1}        // Contactos del paciente 5
    };

    std::cout << "Iniciando rastreo de contactos BFS desde Paciente Cero (ID: 0)...\n\n";
    buscarContactosBFS(0, red_contactos);
    
    return 0;
}
