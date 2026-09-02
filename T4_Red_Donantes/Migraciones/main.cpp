/*
 * ============================================================================
 * Código de Aplicación / Estructura de Datos
 * 
 * Autor: Prof. Edison Valencia Díaz
 * Perfil: https://www.eafit.edu.co/nuestros-profesores/edison-valencia-diaz
 * Curso: Estructuras de Datos y Algoritmos (EDA)
 * Institución: Universidad EAFIT
 * ============================================================================
 * Este código forma parte de las soluciones algorítmicas y estructuras de datos
 * desarrolladas para la resolución de casos de estudio hospitalarios (T4).
 * Contiene comentarios detallados sobre la complejidad y el funcionamiento.
 * ============================================================================
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Person.h"
#include "LinkedList.h"
#include "DoublyLinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "BST.h"
#include "HashTable.h"
#include "Graph.h"
#include "BinaryTree.h"
#include "Visualizer.h"
#include "Colors.h"

using namespace std;
using namespace Visualizer;

vector<Person> loadData(const string& filePath) {
    vector<Person> data;
    ifstream file(filePath);
    if (!file.is_open()) return data;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Person p;
        if (ss >> p.nombre >> p.edad >> p.tipoSangre >> p.organo) {
            int numComp = 0;
            if (ss >> numComp) {
                for (int i = 0; i < numComp; i++) {
                    string comp;
                    ss >> comp;
                    p.compatibles.push_back(comp);
                }
            }
            data.push_back(p);
        }
    }
    return data;
}

void agregarPaciente(const string& validPath, vector<Person>& rawData) {
    Person p;
    cout << "\n" << BOLD << MAGENTA << "=== AÑADIR NUEVO PACIENTE ===" << RESET << "\n";
    cout << "Nombre (sin espacios): "; cin >> p.nombre;
    cout << "Edad: "; cin >> p.edad;
    cout << "Tipo de Sangre (ej. O+): "; cin >> p.tipoSangre;
    cout << "Órgano: "; cin >> p.organo;
    
    int numComp;
    cout << "¿Cuántos compatibles tiene? (0 si ninguno): "; cin >> numComp;
    for (int i = 0; i < numComp; i++) {
        string comp;
        cout << "  -> Nombre del compatible #" << (i+1) << " (sin espacios): "; cin >> comp;
        p.compatibles.push_back(comp);
    }

    // Modo ios::app añade (append) el texto al final del archivo existente
    ofstream file(validPath, ios::app); 
    if (file.is_open()) {
        file << "\n" << p.nombre << " " << p.edad << " " << p.tipoSangre << " " << p.organo;
        if (numComp > 0) {
            file << " " << numComp;
            for (const string& c : p.compatibles) {
                file << " " << c;
            }
        }
        file.close();
        rawData.push_back(p); // Actualizar los datos cargados en memoria
        cout << GREEN << "[✓] Paciente '" << p.nombre << "' guardado con éxito en " << validPath << RESET << "\n";
    } else {
        cout << RED << "[!] Error al intentar escribir en " << validPath << RESET << "\n";
    }
}

void collectBST(TreeNode* node, vector<Person>& buf) {
    if (!node) return;
    collectBST(node->left, buf);
    buf.push_back(node->data);
    collectBST(node->right, buf);
}

void collectBT(BTNode* node, vector<Person>& buf) {
    if (!node) return;
    collectBT(node->left, buf);
    buf.push_back(node->data);
    collectBT(node->right, buf);
}

int mostrarMenu(const string& titulo) {
    cout << "\n" << BOLD << CYAN << "=== " << titulo << " ===" << RESET << "\n";
    cout << "1. Lista Ligada Simple\n";
    cout << "2. Lista Doblemente Ligada\n";
    cout << "3. Pila (Stack LIFO)\n";
    cout << "4. Cola (Queue FIFO)\n";
    cout << "5. Árbol Binario de Búsqueda (BST - Ordena por Edad)\n";
    cout << "6. Árbol Binario Simple (Inserción por niveles)\n";
    cout << "7. Tabla Hash (Búsqueda por nombre)\n";
    cout << "8. Grafo (Red Médica de Compatibilidad Cruzada)\n";
    cout << "9. Volver al menú principal\n";
    cout << BOLD << YELLOW << "Seleccione una opción: " << RESET;
    int opt;
    if (!(cin >> opt)) {
        cin.clear(); cin.ignore(10000, '\n');
        return -1;
    }
    return opt;
}

void ejecutarMigracion(int src, int dst, const vector<Person>& rawData) {
    vector<Person> buffer;

    printHeader("FASE 1: CARGA EN ESTRUCTURA DE ORIGEN");
    
    switch (src) {
        case 1: {
            LinkedList o;
            for (const auto& p : rawData) o.insertar(p);
            printLinkedList(o);
            ListNode* t = o.head;
            while(t) { buffer.push_back(t->data); t = t->next; }
            break;
        }
        case 2: {
            DoublyLinkedList o;
            for (const auto& p : rawData) o.insertar(p);
            printDoublyLinkedList(o);
            DLLNode* t = o.head;
            while(t) { buffer.push_back(t->data); t = t->next; }
            break;
        }
        case 3: {
            Stack o;
            for (const auto& p : rawData) o.push(p);
            printStack(o);
            StackNode* t = o.topNode;
            while(t) { buffer.push_back(t->data); t = t->next; }
            break;
        }
        case 4: {
            Queue o;
            for (const auto& p : rawData) o.enqueue(p);
            printQueue(o);
            QueueNode* t = o.frontNode;
            while(t) { buffer.push_back(t->data); t = t->next; }
            break;
        }
        case 5: {
            BST o;
            for (const auto& p : rawData) o.insertar(p);
            printBST(o);
            collectBST(o.root, buffer);
            break;
        }
        case 6: {
            BinaryTree o;
            for (const auto& p : rawData) o.insertar(p);
            printBinaryTree(o);
            collectBT(o.root, buffer);
            break;
        }
        default:
            cout << RED << "Opción de origen inválida.\n" << RESET;
            return;
    }

    printHeader("FASE 2: MIGRACIÓN A ESTRUCTURA DE DESTINO");

    switch (dst) {
        case 1: { LinkedList d; for (const auto& p : buffer) d.insertar(p); printLinkedList(d); break; }
        case 2: { DoublyLinkedList d; for (const auto& p : buffer) d.insertar(p); printDoublyLinkedList(d); break; }
        case 3: { Stack d; for (const auto& p : buffer) d.push(p); printStack(d); break; }
        case 4: { Queue d; for (const auto& p : buffer) d.enqueue(p); printQueue(d); break; }
        case 5: { BST d; for (const auto& p : buffer) d.insertar(p); printBST(d); break; }
        case 6: { BinaryTree d; for (const auto& p : buffer) d.insertar(p); printBinaryTree(d); break; }
        case 7: { HashTable d; for (const auto& p : buffer) d.insertar(p); printHashTable(d); break; }
        case 8: { Graph d; for (const auto& p : buffer) d.addPerson(p); d.buildCompatibilityTies(); printGraph(d); break; }
        default: cout << RED << "Destino no válido.\n" << RESET; return;
    }
}

int main() {
    cout << "\n" << BOLD << BLUE << "========================================================\n" << RESET;
    cout << BOLD << CYAN << "    SISTEMA MÉDICO CLI: RED DE DONANTES DE ORGANOS  \n" << RESET;
    cout << BOLD << BLUE << "========================================================\n" << RESET;

    string paths[] = {"Datos/data.txt", "../Datos/data.txt", "data.txt"};
    vector<Person> rawData;
    string validPath = "";

    for (const string& path : paths) {
        rawData = loadData(path);
        if (!rawData.empty()) {
            validPath = path;
            break;
        }
    }

    if (rawData.empty()) {
        cerr << RED << BOLD << "\n[!] ERROR CRITICO DE LECTURA: No se pudo localizar la base de datos." << RESET << endl;
        return 1;
    }

    cout << GREEN << "[✓] Base de datos cargada exitosamente (" << rawData.size() << " pacientes).\n" << RESET;

    while (true) {
        cout << "\n" << BOLD << MAGENTA << "=== MENÚ PRINCIPAL ===" << RESET << "\n";
        cout << "1. Migrar datos entre estructuras\n";
        cout << "2. Añadir nuevo paciente (Guardar permanentemente en disco)\n";
        cout << "3. Salir del programa\n";
        cout << BOLD << YELLOW << "Seleccione una opción: " << RESET;
        
        int mainOpt;
        if (!(cin >> mainOpt)) {
            cin.clear(); cin.ignore(10000, '\n');
            continue;
        }

        if (mainOpt == 1) {
            int src = mostrarMenu("SELECCIONE ESTRUCTURA DE ORIGEN");
            if (src == 9 || src == -1) continue;
            if (src == 7 || src == 8) {
                cout << RED << "Advertencia: Hash y Grafos no son estructuras de origen en esta demo.\n" << RESET;
                continue;
            }

            int dst = mostrarMenu("SELECCIONE ESTRUCTURA DE DESTINO");
            if (dst == 9 || dst == -1) continue;

            ejecutarMigracion(src, dst, rawData);
            
        } else if (mainOpt == 2) {
            agregarPaciente(validPath, rawData);
        } else if (mainOpt == 3) {
            cout << YELLOW << "Cerrando el sistema...\n" << RESET;
            break;
        } else {
            cout << RED << "Opción inválida.\n" << RESET;
        }
    }

    return 0;
}