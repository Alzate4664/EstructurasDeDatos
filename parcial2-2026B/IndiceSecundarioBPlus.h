#ifndef INDICE_SECUNDARIO_BPLUS_H
#define INDICE_SECUNDARIO_BPLUS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Registro almacenado en el indice secundario.
 * claveSecundaria: valor textual indexado, por ejemplo "Miguel".
 * idPrincipal: clave primaria del registro en el Arbol B+ principal.
 */
struct RegistroIndice {
    string claveSecundaria;
    int idPrincipal;
};

/**
 * Nodo del Arbol B+ utilizado como indice secundario.
 */
struct NodoIndiceBPlus {
    bool es_hoja;

    vector<string> claves;
    vector<NodoIndiceBPlus*> hijos;
    vector<RegistroIndice> registros;

    NodoIndiceBPlus* siguiente_hoja;

    NodoIndiceBPlus(bool hoja);
};

/**
 * Arbol B+ secundario.
 *
 * Mantiene pares:
 *
 *      clave textual -> ID principal
 *
 * Ejemplo:
 *
 *      "Ana"    -> 10
 *      "Carlos" -> 30
 *      "Miguel" -> 20
 */
class IndiceSecundarioBPlus {
private:
    NodoIndiceBPlus* raiz;
    int grado;

    NodoIndiceBPlus* buscarHoja(const string& clave);
    NodoIndiceBPlus* buscarPadre(
        NodoIndiceBPlus* cursor,
        NodoIndiceBPlus* hijo
    );

    void insertarInterno(
        const string& clave,
        NodoIndiceBPlus* cursor,
        NodoIndiceBPlus* hijo
    );

    void liberarNodo(NodoIndiceBPlus* nodo);

public:
    IndiceSecundarioBPlus(int _grado = 3);
    ~IndiceSecundarioBPlus();

    void insertar(const string& clave, int idPrincipal);

    vector<int> buscar(const string& clave);

    void vaciar();

    bool estaVacio() const;
};

#endif