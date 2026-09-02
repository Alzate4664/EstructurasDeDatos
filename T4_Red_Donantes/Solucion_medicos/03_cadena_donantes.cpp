#include <iostream>
#include <stack>
#include <vector>

/*
 * ============================================================================
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * ============================================================================
 * Escenario 3: Cadena de Donantes Rota
 * Paradigma: Vuelta Atrás (Backtracking) / DFS
 * Estructura de Datos: Pila (Stack - LIFO)
 * ============================================================================
 * 
 * En una cadena de donación, si un paciente rechaza un órgano, no podemos
 * detener el programa ni empezar de cero. Usamos una Pila para retroceder
 * el último paso exitoso (Backtrack) y probar alternativas. 
 */

struct Paciente {
    int id;
    bool tiene_anticuerpos_rechazo;
};

bool armarCadena(std::vector<Paciente>& candidatos) {
    // La pila simula la ruta actual (LIFO - Último en entrar, primero en salir)
    std::stack<Paciente> cadena;
    
    for (Paciente& p : candidatos) {
        // Agregamos el paciente a nuestra ruta de cadena actual
        cadena.push(p);
        
        if (p.tiene_anticuerpos_rechazo) {
            // Ocurre una inconsistencia. Sacamos de la pila (Pop) al último elemento
            // para "deshacer" este paso y retroceder (Backtrack) a un estado seguro.
            std::cout << "Rechazo detectado en paciente " << p.id << ". Ejecutando Backtrack (Pop)...\n";
            cadena.pop(); 
        } else {
            // El paciente es compatible y entra de forma permanente a la ruta
            std::cout << "Paciente " << p.id << " aceptado y agregado a la cadena de donantes.\n";
            
            // Condición de éxito
            if (cadena.size() == 5) {
                std::cout << "\n¡Cadena exitosa de 5 pacientes completada!\n";
                return true; 
            }
        }
    }
    
    return false; // Se evaluaron candidatos y no se logró completar 5.
}

int main() {
    // Vector simulando la fila de posibles receptores. El paciente 4 tiene anticuerpos.
    std::vector<Paciente> candidatos = {
        {1, false}, {2, false}, {3, false}, {4, true}, {5, false}, {6, false}
    };
    
    std::cout << "Iniciando formación de cadena de donación...\n\n";
    if (!armarCadena(candidatos)) {
        std::cout << "Fallo en la cadena general." << std::endl;
    }
    
    return 0;
}
