#include <iostream>
#include <stack>
#include <vector>

struct Paciente {
    int id;
    bool tiene_anticuerpos_rechazo;
};

bool armarCadena(std::vector<Paciente>& candidatos) {
    std::stack<Paciente> cadena;
    
    for (Paciente& p : candidatos) {
        cadena.push(p);
        
        if (p.tiene_anticuerpos_rechazo) {
            std::cout << "Rechazo en paciente " << p.id << ". Backtrack...\n";
            cadena.pop(); // Backtracking
        } else {
            std::cout << "Paciente " << p.id << " agregado a la cadena.\n";
            if (cadena.size() == 5) {
                std::cout << "Cadena exitosa de 5 pacientes completada.\n";
                return true; 
            }
        }
    }
    return false;
}

int main() {
    std::vector<Paciente> candidatos = {
        {1, false}, {2, false}, {3, false}, {4, true}, {5, false}, {6, false}
    };
    
    if (!armarCadena(candidatos)) {
        std::cout << "Fallo en la cadena general." << std::endl;
    }
    return 0;
}
