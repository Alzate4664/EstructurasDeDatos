#include "IndiceSecundarioBPlus.h"
#include <algorithm>

NodoIndiceBPlus::NodoIndiceBPlus(bool hoja)
    : es_hoja(hoja), siguiente_hoja(nullptr) {}

IndiceSecundarioBPlus::IndiceSecundarioBPlus(int _grado)
    : raiz(nullptr), grado(_grado) {}

IndiceSecundarioBPlus::~IndiceSecundarioBPlus() {
    liberarNodo(raiz);
    raiz = nullptr;
}

void IndiceSecundarioBPlus::liberarNodo(NodoIndiceBPlus* nodo) {
    if (nodo == nullptr) {
        return;
    }

    if (!nodo->es_hoja) {
        for (NodoIndiceBPlus* hijo : nodo->hijos) {
            liberarNodo(hijo);
        }
    }

    delete nodo;
}

void IndiceSecundarioBPlus::vaciar() {
    liberarNodo(raiz);
    raiz = nullptr;
}

bool IndiceSecundarioBPlus::estaVacio() const {
    return raiz == nullptr;
}

NodoIndiceBPlus* IndiceSecundarioBPlus::buscarHoja(
    const string& clave
) {
    NodoIndiceBPlus* cursor = raiz;

    if (cursor == nullptr) {
        return nullptr;
    }

    while (!cursor->es_hoja) {
        size_t posicion = 0;

        while (
            posicion < cursor->claves.size() &&
            clave >= cursor->claves[posicion]
        ) {
            posicion++;
        }

        cursor = cursor->hijos[posicion];
    }

    return cursor;
}

NodoIndiceBPlus* IndiceSecundarioBPlus::buscarPadre(
    NodoIndiceBPlus* cursor,
    NodoIndiceBPlus* hijo
) {
    if (
        cursor == nullptr ||
        cursor->es_hoja
    ) {
        return nullptr;
    }

    for (NodoIndiceBPlus* candidato : cursor->hijos) {
        if (candidato == hijo) {
            return cursor;
        }
    }

    for (NodoIndiceBPlus* candidato : cursor->hijos) {
        NodoIndiceBPlus* padre = buscarPadre(
            candidato,
            hijo
        );

        if (padre != nullptr) {
            return padre;
        }
    }

    return nullptr;
}

void IndiceSecundarioBPlus::insertarInterno(
    const string& clave,
    NodoIndiceBPlus* cursor,
    NodoIndiceBPlus* hijo
) {
    size_t posicion = 0;

    while (
        posicion < cursor->claves.size() &&
        clave > cursor->claves[posicion]
    ) {
        posicion++;
    }

    cursor->claves.insert(
        cursor->claves.begin() + posicion,
        clave
    );

    cursor->hijos.insert(
        cursor->hijos.begin() + posicion + 1,
        hijo
    );

    if (cursor->claves.size() <= static_cast<size_t>(grado)) {
        return;
    }

    size_t puntoMedio = cursor->claves.size() / 2;

    string clavePromovida = cursor->claves[puntoMedio];

    NodoIndiceBPlus* nuevoInterno =
        new NodoIndiceBPlus(false);

    nuevoInterno->claves.assign(
        cursor->claves.begin() + puntoMedio + 1,
        cursor->claves.end()
    );

    nuevoInterno->hijos.assign(
        cursor->hijos.begin() + puntoMedio + 1,
        cursor->hijos.end()
    );

    cursor->claves.erase(
        cursor->claves.begin() + puntoMedio,
        cursor->claves.end()
    );

    cursor->hijos.erase(
        cursor->hijos.begin() + puntoMedio + 1,
        cursor->hijos.end()
    );

    if (cursor == raiz) {
        NodoIndiceBPlus* nuevaRaiz =
            new NodoIndiceBPlus(false);

        nuevaRaiz->claves.push_back(clavePromovida);

        nuevaRaiz->hijos.push_back(cursor);
        nuevaRaiz->hijos.push_back(nuevoInterno);

        raiz = nuevaRaiz;

        return;
    }

    NodoIndiceBPlus* padre =
        buscarPadre(raiz, cursor);

    if (padre != nullptr) {
        insertarInterno(
            clavePromovida,
            padre,
            nuevoInterno
        );
    }
}

void IndiceSecundarioBPlus::insertar(
    const string& clave,
    int idPrincipal
) {
    if (clave.empty()) {
        return;
    }

    if (raiz == nullptr) {
        raiz = new NodoIndiceBPlus(true);

        raiz->claves.push_back(clave);
        raiz->registros.push_back({
            clave,
            idPrincipal
        });

        return;
    }

    NodoIndiceBPlus* hoja = buscarHoja(clave);

    size_t posicion = 0;

    while (
        posicion < hoja->registros.size() &&
        (
            hoja->registros[posicion].claveSecundaria < clave ||
            (
                hoja->registros[posicion].claveSecundaria == clave &&
                hoja->registros[posicion].idPrincipal < idPrincipal
            )
        )
    ) {
        posicion++;
    }

    // Evitar duplicar exactamente el mismo par clave -> ID.
    if (
        posicion < hoja->registros.size() &&
        hoja->registros[posicion].claveSecundaria == clave &&
        hoja->registros[posicion].idPrincipal == idPrincipal
    ) {
        return;
    }

    hoja->registros.insert(
        hoja->registros.begin() + posicion,
        {clave, idPrincipal}
    );

    hoja->claves.insert(
        hoja->claves.begin() + posicion,
        clave
    );

    if (hoja->claves.size() <= static_cast<size_t>(grado)) {
        return;
    }

    size_t puntoMedio =
        hoja->claves.size() / 2;

    NodoIndiceBPlus* nuevaHoja =
        new NodoIndiceBPlus(true);

    nuevaHoja->claves.assign(
        hoja->claves.begin() + puntoMedio,
        hoja->claves.end()
    );

    nuevaHoja->registros.assign(
        hoja->registros.begin() + puntoMedio,
        hoja->registros.end()
    );

    hoja->claves.erase(
        hoja->claves.begin() + puntoMedio,
        hoja->claves.end()
    );

    hoja->registros.erase(
        hoja->registros.begin() + puntoMedio,
        hoja->registros.end()
    );

    nuevaHoja->siguiente_hoja =
        hoja->siguiente_hoja;

    hoja->siguiente_hoja =
        nuevaHoja;

    string clavePromovida =
        nuevaHoja->claves.front();

    if (hoja == raiz) {
        NodoIndiceBPlus* nuevaRaiz =
            new NodoIndiceBPlus(false);

        nuevaRaiz->claves.push_back(
            clavePromovida
        );

        nuevaRaiz->hijos.push_back(hoja);
        nuevaRaiz->hijos.push_back(
            nuevaHoja
        );

        raiz = nuevaRaiz;

        return;
    }

    NodoIndiceBPlus* padre =
        buscarPadre(raiz, hoja);

    if (padre != nullptr) {
        insertarInterno(
            clavePromovida,
            padre,
            nuevaHoja
        );
    }
}

vector<int> IndiceSecundarioBPlus::buscar(
    const string& clave
) {
    vector<int> resultados;

    if (raiz == nullptr) {
        return resultados;
    }

    /*
     * Para una búsqueda secundaria pueden existir claves repetidas.
     *
     * Por eso, cuando una clave coincide con una clave separadora
     * de un nodo interno, descendemos por el hijo izquierdo.
     *
     * De esta manera llegamos a la primera hoja potencial que puede
     * contener esa clave y luego continuamos por siguiente_hoja.
     */
    NodoIndiceBPlus* cursor = raiz;

    while (!cursor->es_hoja) {
        size_t posicion = 0;

        while (
            posicion < cursor->claves.size() &&
            clave > cursor->claves[posicion]
        ) {
            posicion++;
        }

        cursor = cursor->hijos[posicion];
    }

    NodoIndiceBPlus* hoja = cursor;

    while (hoja != nullptr) {
        for (
            const RegistroIndice& registro :
            hoja->registros
        ) {
            if (registro.claveSecundaria < clave) {
                continue;
            }

            if (registro.claveSecundaria == clave) {
                resultados.push_back(
                    registro.idPrincipal
                );
                continue;
            }

            if (registro.claveSecundaria > clave) {
                sort(resultados.begin(), resultados.end());
                return resultados;
            }
        }

        hoja = hoja->siguiente_hoja;
    }

    sort(resultados.begin(), resultados.end());
    return resultados;
}