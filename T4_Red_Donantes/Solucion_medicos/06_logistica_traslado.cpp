#include <iostream>
#include <vector>
#include <queue>

using namespace std;
typedef pair<int, int> Arista; 
const int INF = 1e9;

void calcularMejorRuta(int origen, int num_hospitales, vector<vector<Arista>>& grafo) {
    vector<int> distancias(num_hospitales, INF);
    priority_queue<Arista, vector<Arista>, greater<Arista>> min_heap;
    
    distancias[origen] = 0;
    min_heap.push({0, origen});
    
    while (!min_heap.empty()) {
        int costo_actual = min_heap.top().first;
        int u = min_heap.top().second;
        min_heap.pop();
        
        if (costo_actual > distancias[u]) continue;
        
        for (auto& arista : grafo[u]) {
            int v = arista.second;
            int peso = arista.first;
            
            if (distancias[u] + peso < distancias[v]) {
                distancias[v] = distancias[u] + peso;
                min_heap.push({distancias[v], v}); // Se prioriza el de menor costo
            }
        }
    }

    cout << "Costos mínimos (en min.) desde el hospital " << origen << ":\n";
    for(int i = 0; i < num_hospitales; ++i) {
        cout << "A hospital " << i << ": " << distancias[i] << " min\n";
    }
}

int main() {
    int num_hospitales = 4;
    vector<vector<Arista>> grafo(num_hospitales);
    
    // Conexiones: {costo_en_minutos, destino}
    // 0: Hosp. Origen, 1: Hosp. Norte, 2: Hosp. Sur, 3: Hosp. Destino
    grafo[0].push_back({10, 1});
    grafo[0].push_back({15, 2});
    grafo[1].push_back({5, 3});
    grafo[2].push_back({10, 3});

    calcularMejorRuta(0, num_hospitales, grafo);
    
    return 0;
}
