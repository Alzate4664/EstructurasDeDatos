#include <iostream>
#include <vector>
#include <queue>

/*
 * ============================================================================
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * ============================================================================
 * Escenario 6: Logística de Traslado
 * Paradigma: Algoritmos Ávidos (Greedy) / Dijkstra
 * Estructura de Datos: Grafo (Vértices/Aristas) y Cola de Prioridad
 * ============================================================================
 * 
 * Para transportar un órgano en el menor tiempo, el algoritmo de Dijkstra 
 * selecciona iterativamente la ruta vecina de menor costo utilizando una 
 * Cola de Prioridad (Min-Heap). Es Greedy (ávido) porque asume que siempre 
 * ir al nodo visible más cercano construirá globalmente la ruta más corta.
 */

using namespace std;

// Representa {costo_distancia_o_tiempo, hospital_destino}
typedef pair<int, int> Arista; 
const int INF = 1e9; // Valor "infinito" inicial

void calcularMejorRuta(int origen, int num_hospitales, vector<vector<Arista>>& grafo) {
    // Array para almacenar el menor tiempo descubierto hacia cada hospital
    vector<int> distancias(num_hospitales, INF);
    
    // Cola de prioridad Min-Heap para extraer siempre la opción más barata/rápida en O(log V)
    priority_queue<Arista, vector<Arista>, greater<Arista>> min_heap;
    
    // Configuración inicial del punto de origen
    distancias[origen] = 0;
    min_heap.push({0, origen});
    
    while (!min_heap.empty()) {
        // Sacamos siempre el hospital que actualmente esté más "cerca"
        int costo_actual = min_heap.top().first;
        int u = min_heap.top().second;
        min_heap.pop();
        
        // Si descubrimos un costo viejo peor que el actual en tabla, lo descartamos
        if (costo_actual > distancias[u]) continue;
        
        // Evaluamos (relajamos) todos los hospitales conectados al actual
        for (auto& arista : grafo[u]) {
            int v = arista.second;      // Hospital vecino
            int peso = arista.first;    // Tiempo del trayecto
            
            // ¿Llegar al vecino 'v' desde 'u' es más rápido que el tiempo conocido previo?
            if (distancias[u] + peso < distancias[v]) {
                distancias[v] = distancias[u] + peso;
                min_heap.push({distancias[v], v}); // Actualizamos a la cola el nuevo menor tiempo
            }
        }
    }

    // Imprimir el mapa de rutas óptimo
    cout << "Costos mínimos (en min.) trazados desde el Hospital Origen (" << origen << "):\n";
    for(int i = 0; i < num_hospitales; ++i) {
        cout << " -> Hacia Hospital " << i << " tomará: " << distancias[i] << " min\n";
    }
}

int main() {
    int num_hospitales = 4;
    // Grafo dirigido usando Lista de Adyacencias
    vector<vector<Arista>> grafo(num_hospitales);
    
    // Conexiones: {costo_en_minutos, id_destino}
    // Nodos: (0) Hosp. Origen, (1) Hosp. Norte, (2) Hosp. Sur, (3) Hosp. Destino
    grafo[0].push_back({10, 1}); // De Origen a Norte toma 10m
    grafo[0].push_back({15, 2}); // De Origen a Sur toma 15m
    grafo[1].push_back({5, 3});  // De Norte a Destino toma 5m
    grafo[2].push_back({10, 3}); // De Sur a Destino toma 10m

    std::cout << "Calculando la logística óptima del helicóptero de traslado...\n\n";
    calcularMejorRuta(0, num_hospitales, grafo);
    
    return 0;
}
