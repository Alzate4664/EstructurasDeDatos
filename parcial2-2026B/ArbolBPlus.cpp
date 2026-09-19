/**
 * Archivo: ArbolBPlus.cpp
 * Propósito: Aquí se implementan los métodos declarados en ArbolBPlus.h. 
 *            Esta es la clase central que los estudiantes deberán completar 
 *            como parte de su reto de Estructuras de Datos.
 */
#include "ArbolBPlus.h"

// -----------------------------------------------------------------------------
// Implementaciones de Estructuras Auxiliares
// -----------------------------------------------------------------------------

string Registro::serializar() const {
    // Ejemplo de salida: "1,Juan Perez" (clave + delimitador + datos)
    return to_string(clave) + "," + datos;
}

NodoBPlus::NodoBPlus(bool hoja) {
    // Por defecto al nacer, sabemos si es hoja o interno, pero no tiene nodos adyacentes aún.
    es_hoja = hoja;
    siguiente_hoja = nullptr;
}

// -----------------------------------------------------------------------------
// Constructor del Arbol B+
// -----------------------------------------------------------------------------

ArbolBPlus::ArbolBPlus(int _grado, string _nombre_archivo) : raiz(nullptr), grado(_grado), nombre_archivo(_nombre_archivo) {}

ArbolBPlus::~ArbolBPlus() {
    liberarNodo(raiz);
    raiz = nullptr;
}

void ArbolBPlus::vaciar() {
    liberarNodo(raiz);
    raiz = nullptr;
}

void ArbolBPlus::liberarNodo(NodoBPlus* nodo) {
    if (nodo == nullptr) {
        return;
    }

    // Si es un nodo interno, primero liberar todos sus hijos.
    if (!nodo->es_hoja) {
        for (NodoBPlus* hijo : nodo->hijos) {
            liberarNodo(hijo);
        }
    }

    delete nodo;
}

NodoBPlus* ArbolBPlus::buscarHoja(int clave) {
    NodoBPlus* cursor = raiz;

    while (cursor != nullptr && !cursor->es_hoja) {
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

NodoBPlus* ArbolBPlus::buscarPadre(
    NodoBPlus* cursor,
    NodoBPlus* hijo
) {
    if (cursor == nullptr || cursor->es_hoja) {
        return nullptr;
    }

    // Comprobar si cursor es el padre directo.
    for (NodoBPlus* candidato : cursor->hijos) {
        if (candidato == hijo) {
            return cursor;
        }
    }

    // Buscar recursivamente en los niveles inferiores.
    for (NodoBPlus* candidato : cursor->hijos) {
        NodoBPlus* padreEncontrado = buscarPadre(
            candidato,
            hijo
        );

        if (padreEncontrado != nullptr) {
            return padreEncontrado;
        }
    }

    return nullptr;
}

int ArbolBPlus::obtenerPrimeraClave(NodoBPlus* nodo) {
    NodoBPlus* cursor = nodo;

    while (cursor != nullptr && !cursor->es_hoja) {
        if (cursor->hijos.empty()) {
            return 0;
        }

        cursor = cursor->hijos.front();
    }

    if (cursor == nullptr || cursor->claves.empty()) {
        return 0;
    }

    return cursor->claves.front();
}

void ArbolBPlus::recalcularSeparadores(NodoBPlus* nodo) {
    if (nodo == nullptr || nodo->es_hoja) {
        return;
    }

    for (NodoBPlus* hijo : nodo->hijos) {
        recalcularSeparadores(hijo);
    }

    nodo->claves.clear();

    /*
     * En este Arbol B+, cada clave interna representa
     * la menor clave del hijo situado a su derecha.
     */
    for (size_t i = 1; i < nodo->hijos.size(); i++) {
        nodo->claves.push_back(
            obtenerPrimeraClave(nodo->hijos[i])
        );
    }
}

void ArbolBPlus::rebalancearInterno(NodoBPlus* nodo) {
    if (nodo == nullptr || nodo->es_hoja) {
        return;
    }

    /*
     * Caso especial de la raiz.
     *
     * Si despues de un merge solo queda un hijo,
     * ese hijo pasa a ser la nueva raiz.
     */
    if (nodo == raiz) {
        if (nodo->hijos.empty()) {
            delete nodo;
            raiz = nullptr;
            return;
        }

        if (nodo->hijos.size() == 1) {
            NodoBPlus* nuevaRaiz = nodo->hijos.front();

            nodo->hijos.clear();
            delete nodo;

            raiz = nuevaRaiz;
        }

        if (raiz != nullptr) {
            recalcularSeparadores(raiz);
        }

        return;
    }

    /*
     * Un nodo interno con grado 3 admite hasta
     * 4 hijos y debe conservar como minimo 2.
     */
    int minimoHijos = (grado + 2) / 2;

    if (
        static_cast<int>(nodo->hijos.size()) >=
        minimoHijos
    ) {
        recalcularSeparadores(raiz);
        return;
    }

    NodoBPlus* padre = buscarPadre(raiz, nodo);

    if (padre == nullptr) {
        return;
    }

    size_t posicion = 0;

    while (
        posicion < padre->hijos.size() &&
        padre->hijos[posicion] != nodo
    ) {
        posicion++;
    }

    NodoBPlus* hermanoIzquierdo =
        posicion > 0
            ? padre->hijos[posicion - 1]
            : nullptr;

    NodoBPlus* hermanoDerecho =
        posicion + 1 < padre->hijos.size()
            ? padre->hijos[posicion + 1]
            : nullptr;

    // ==========================================
    // Redistribuir desde el hermano izquierdo
    // ==========================================
    if (
        hermanoIzquierdo != nullptr &&
        static_cast<int>(
            hermanoIzquierdo->hijos.size()
        ) > minimoHijos
    ) {
        NodoBPlus* hijoPrestado =
            hermanoIzquierdo->hijos.back();

        hermanoIzquierdo->hijos.pop_back();

        nodo->hijos.insert(
            nodo->hijos.begin(),
            hijoPrestado
        );

        recalcularSeparadores(raiz);
        return;
    }

    // ==========================================
    // Redistribuir desde el hermano derecho
    // ==========================================
    if (
        hermanoDerecho != nullptr &&
        static_cast<int>(
            hermanoDerecho->hijos.size()
        ) > minimoHijos
    ) {
        NodoBPlus* hijoPrestado =
            hermanoDerecho->hijos.front();

        hermanoDerecho->hijos.erase(
            hermanoDerecho->hijos.begin()
        );

        nodo->hijos.push_back(hijoPrestado);

        recalcularSeparadores(raiz);
        return;
    }

    // ==========================================
    // Merge con hermano izquierdo
    // ==========================================
    if (hermanoIzquierdo != nullptr) {
        hermanoIzquierdo->hijos.insert(
            hermanoIzquierdo->hijos.end(),
            nodo->hijos.begin(),
            nodo->hijos.end()
        );

        padre->hijos.erase(
            padre->hijos.begin() + posicion
        );

        nodo->hijos.clear();
        delete nodo;

        recalcularSeparadores(padre);
        rebalancearInterno(padre);

        if (raiz != nullptr) {
            recalcularSeparadores(raiz);
        }

        return;
    }

    // ==========================================
    // Merge con hermano derecho
    // ==========================================
    if (hermanoDerecho != nullptr) {
        nodo->hijos.insert(
            nodo->hijos.end(),
            hermanoDerecho->hijos.begin(),
            hermanoDerecho->hijos.end()
        );

        padre->hijos.erase(
            padre->hijos.begin() + posicion + 1
        );

        hermanoDerecho->hijos.clear();
        delete hermanoDerecho;

        recalcularSeparadores(padre);
        rebalancearInterno(padre);

        if (raiz != nullptr) {
            recalcularSeparadores(raiz);
        }
    }
}

void ArbolBPlus::rebalancearHoja(NodoBPlus* hoja) {
    if (hoja == nullptr || hoja == raiz) {
        return;
    }

    /*
     * Con grado 3 una hoja admite maximo 3 claves.
     * El minimo normal despues de un split es 2.
     */
    int minimoClaves = (grado + 1) / 2;

    if (
        static_cast<int>(hoja->claves.size()) >=
        minimoClaves
    ) {
        recalcularSeparadores(raiz);
        return;
    }

    NodoBPlus* padre = buscarPadre(raiz, hoja);

    if (padre == nullptr) {
        return;
    }

    size_t posicion = 0;

    while (
        posicion < padre->hijos.size() &&
        padre->hijos[posicion] != hoja
    ) {
        posicion++;
    }

    NodoBPlus* hermanoIzquierdo =
        posicion > 0
            ? padre->hijos[posicion - 1]
            : nullptr;

    NodoBPlus* hermanoDerecho =
        posicion + 1 < padre->hijos.size()
            ? padre->hijos[posicion + 1]
            : nullptr;

    // ==========================================
    // Pedir prestado al hermano izquierdo
    // ==========================================
    if (
        hermanoIzquierdo != nullptr &&
        static_cast<int>(
            hermanoIzquierdo->claves.size()
        ) > minimoClaves
    ) {
        hoja->claves.insert(
            hoja->claves.begin(),
            hermanoIzquierdo->claves.back()
        );

        hoja->registros.insert(
            hoja->registros.begin(),
            hermanoIzquierdo->registros.back()
        );

        hermanoIzquierdo->claves.pop_back();
        hermanoIzquierdo->registros.pop_back();

        recalcularSeparadores(raiz);
        return;
    }

    // ==========================================
    // Pedir prestado al hermano derecho
    // ==========================================
    if (
        hermanoDerecho != nullptr &&
        static_cast<int>(
            hermanoDerecho->claves.size()
        ) > minimoClaves
    ) {
        hoja->claves.push_back(
            hermanoDerecho->claves.front()
        );

        hoja->registros.push_back(
            hermanoDerecho->registros.front()
        );

        hermanoDerecho->claves.erase(
            hermanoDerecho->claves.begin()
        );

        hermanoDerecho->registros.erase(
            hermanoDerecho->registros.begin()
        );

        recalcularSeparadores(raiz);
        return;
    }

    // ==========================================
    // Merge con hermano izquierdo
    // ==========================================
    if (hermanoIzquierdo != nullptr) {
        hermanoIzquierdo->claves.insert(
            hermanoIzquierdo->claves.end(),
            hoja->claves.begin(),
            hoja->claves.end()
        );

        hermanoIzquierdo->registros.insert(
            hermanoIzquierdo->registros.end(),
            hoja->registros.begin(),
            hoja->registros.end()
        );

        hermanoIzquierdo->siguiente_hoja =
            hoja->siguiente_hoja;

        padre->hijos.erase(
            padre->hijos.begin() + posicion
        );

        delete hoja;

        recalcularSeparadores(padre);
        rebalancearInterno(padre);

        if (raiz != nullptr) {
            recalcularSeparadores(raiz);
        }

        return;
    }

    // ==========================================
    // Merge con hermano derecho
    // ==========================================
    if (hermanoDerecho != nullptr) {
        hoja->claves.insert(
            hoja->claves.end(),
            hermanoDerecho->claves.begin(),
            hermanoDerecho->claves.end()
        );

        hoja->registros.insert(
            hoja->registros.end(),
            hermanoDerecho->registros.begin(),
            hermanoDerecho->registros.end()
        );

        hoja->siguiente_hoja =
            hermanoDerecho->siguiente_hoja;

        padre->hijos.erase(
            padre->hijos.begin() + posicion + 1
        );

        delete hermanoDerecho;

        recalcularSeparadores(padre);
        rebalancearInterno(padre);

        if (raiz != nullptr) {
            recalcularSeparadores(raiz);
        }
    }
}

void ArbolBPlus::insertarInterno(
    int clave,
    NodoBPlus* cursor,
    NodoBPlus* hijo
) {
    size_t posicion = 0;

    while (
        posicion < cursor->claves.size() &&
        cursor->claves[posicion] < clave
    ) {
        posicion++;
    }

    // La clave separadora se inserta en posicion.
    cursor->claves.insert(
        cursor->claves.begin() + posicion,
        clave
    );

    // El nuevo hijo queda inmediatamente después de la clave.
    cursor->hijos.insert(
        cursor->hijos.begin() + posicion + 1,
        hijo
    );

    // Si el nodo interno sigue dentro del límite, terminamos.
    if (cursor->claves.size() <= static_cast<size_t>(grado)) {
        return;
    }

    // El nodo interno se desbordó: hay que dividirlo.
    size_t puntoMedio = cursor->claves.size() / 2;
    int clavePromovida = cursor->claves[puntoMedio];

    NodoBPlus* nuevoInterno = new NodoBPlus(false);

    // Las claves posteriores a la promovida pasan al nuevo nodo.
    nuevoInterno->claves.assign(
        cursor->claves.begin() + puntoMedio + 1,
        cursor->claves.end()
    );

    // Los hijos de la parte derecha pasan al nuevo nodo.
    nuevoInterno->hijos.assign(
        cursor->hijos.begin() + puntoMedio + 1,
        cursor->hijos.end()
    );

    // El nodo original conserva la parte izquierda.
    cursor->claves.erase(
        cursor->claves.begin() + puntoMedio,
        cursor->claves.end()
    );

    cursor->hijos.erase(
        cursor->hijos.begin() + puntoMedio + 1,
        cursor->hijos.end()
    );

    // Si se dividió la raíz, crear una nueva raíz.
    if (cursor == raiz) {
        NodoBPlus* nuevaRaiz = new NodoBPlus(false);

        nuevaRaiz->claves.push_back(clavePromovida);
        nuevaRaiz->hijos.push_back(cursor);
        nuevaRaiz->hijos.push_back(nuevoInterno);

        raiz = nuevaRaiz;
        return;
    }

    // Propagar la división al nivel superior.
    NodoBPlus* padre = buscarPadre(raiz, cursor);

    if (padre != nullptr) {
        insertarInterno(
            clavePromovida,
            padre,
            nuevoInterno
        );
    }
}

// =========================================================================
// MÉTODOS A IMPLEMENTAR PARA EL PARCIAL
// =========================================================================

void ArbolBPlus::insertar(int clave, string datos) {
    // [A IMPLEMENTAR EN EL PARCIAL]:
    // Lógica requerida:
    // 1. Si el árbol está vacío (raiz == nullptr), crear el primer nodo hoja.
    // 2. Si no está vacío, recorrer el árbol desde la raíz bajando por los hijos 
    //    correctos comparando la clave, hasta llegar a una hoja.
    // 3. Insertar el 'Registro' en el vector de registros de la hoja, MANTENIENDO EL ORDEN.
    // 4. Verificar condición de llenado: Si la hoja ahora tiene más elementos que el grado 
    //    (se desbordó), se debe dividir (SPLIT).
    // 5. El Split implica:
    //    a) Crear una nueva hoja.
    //    b) Pasar la mitad de los registros a la nueva hoja.
    //    c) Promover la clave media hacia el nodo PADRE.
    //    d) Configurar el puntero "siguiente_hoja" para mantener la lista enlazada unida.
    // 6. Esta propagación puede subir recursivamente hasta la raíz, obligando a crear una nueva raíz si es necesario.

    // Caso 1: el árbol todavía está vacío.
    if (raiz == nullptr) {
        raiz = new NodoBPlus(true);
        raiz->claves.push_back(clave);
        raiz->registros.push_back({clave, datos});

        cout << "Registro insertado correctamente.\n";
        return;
    }

    // Buscar la hoja en la que debería quedar la clave.
    NodoBPlus* hoja = buscarHoja(clave);

    size_t posicion = 0;

    while (
        posicion < hoja->claves.size() &&
        hoja->claves[posicion] < clave
    ) {
        posicion++;
    }

    // No permitir claves primarias repetidas.
    if (
        posicion < hoja->claves.size() &&
        hoja->claves[posicion] == clave
    ) {
        cout << "Error: ya existe un registro con el ID "
             << clave << ".\n";
        return;
    }

    Registro nuevoRegistro{clave, datos};

    // Insertar la clave y el registro en la misma posición.
    hoja->claves.insert(
        hoja->claves.begin() + posicion,
        clave
    );

    hoja->registros.insert(
        hoja->registros.begin() + posicion,
        nuevoRegistro
    );

    cout << "Registro insertado correctamente.\n";

    // Si la hoja supera el máximo de claves, realizar el split.
    if (hoja->claves.size() > static_cast<size_t>(grado)) {
        size_t puntoMedio = hoja->claves.size() / 2;

        NodoBPlus* nuevaHoja = new NodoBPlus(true);

        // Mover la segunda mitad de claves y registros.
        nuevaHoja->claves.assign(
            hoja->claves.begin() + puntoMedio,
            hoja->claves.end()
        );

        nuevaHoja->registros.assign(
            hoja->registros.begin() + puntoMedio,
            hoja->registros.end()
        );

        // La hoja original conserva la primera mitad.
        hoja->claves.erase(
            hoja->claves.begin() + puntoMedio,
            hoja->claves.end()
        );

        hoja->registros.erase(
            hoja->registros.begin() + puntoMedio,
            hoja->registros.end()
        );

        // Conectar las hojas como una lista enlazada.
        nuevaHoja->siguiente_hoja = hoja->siguiente_hoja;
        hoja->siguiente_hoja = nuevaHoja;

        // En un B+, se copia al padre la primera clave de la nueva hoja.
        int clavePromovida = nuevaHoja->claves.front();

        // Caso especial: la hoja dividida era la raíz.
        if (hoja == raiz) {
            NodoBPlus* nuevaRaiz = new NodoBPlus(false);

            nuevaRaiz->claves.push_back(clavePromovida);
            nuevaRaiz->hijos.push_back(hoja);
            nuevaRaiz->hijos.push_back(nuevaHoja);

            raiz = nuevaRaiz;
        } else {
            NodoBPlus* padre = buscarPadre(raiz, hoja);

            if (padre != nullptr) {
                insertarInterno(
                    clavePromovida,
                    padre,
                    nuevaHoja
                );
            }
        }

        cout << "Split realizado correctamente. Clave promovida: "
             << clavePromovida << ".\n";
    }
}

string ArbolBPlus::buscar(int clave) {
    // [A IMPLEMENTAR EN EL PARCIAL]:
    // Lógica requerida:
    // 1. Si la raíz es nullptr, devolver string vacío (no hay datos).
    // 2. Empezar en la raíz y hacer una búsqueda binaria o lineal sobre 'claves'.
    // 3. Si la clave buscada es menor que claves[i], bajar por hijos[i].
    // 4. Si la clave es mayor o igual, seguir iterando o bajar por el último hijo.
    // 5. Al llegar a un nodo hoja (`es_hoja == true`), buscar el registro exacto.
    // 6. Si se encuentra, retornar `registro.datos`, de lo contrario retornar string vacío.
    
    if (raiz == nullptr) {
        return "";
    }

    // Descender desde la raíz hasta la hoja correspondiente.
    NodoBPlus* hoja = buscarHoja(clave);

    if (hoja == nullptr) {
        return "";
    }

    // Buscar la clave exacta dentro de la hoja.
    size_t posicion = 0;

    while (
        posicion < hoja->claves.size() &&
        hoja->claves[posicion] < clave
    ) {
        posicion++;
    }

    if (
        posicion < hoja->claves.size() &&
        hoja->claves[posicion] == clave
    ) {
        return hoja->registros[posicion].datos;
    }

    return "";
}

void ArbolBPlus::eliminar(int clave) {
    // [A IMPLEMENTAR EN EL PARCIAL]:
    // Lógica requerida:
    // 1. Localizar la hoja donde reside la clave.
    // 2. Eliminar el registro del vector.
    // 3. Verificar condición de 'underflow' (menos registros de los requeridos por el grado).
    // 4. Si hay underflow, intentar pedir prestado un registro a un nodo hermano (redistribución).
    // 5. Si no se puede pedir prestado, hacer 'merge' (fusión) con el hermano, 
    //    y eliminar la clave divisora en el nodo padre.
    
    if (raiz == nullptr) {
        cout << "Error: el arbol esta vacio.\n";
        return;
    }

    NodoBPlus* hoja = buscarHoja(clave);

    if (hoja == nullptr) {
        cout << "Error: no se encontro el ID "
             << clave << ".\n";
        return;
    }

    size_t posicion = 0;

    while (
        posicion < hoja->claves.size() &&
        hoja->claves[posicion] != clave
    ) {
        posicion++;
    }

    if (posicion == hoja->claves.size()) {
        cout << "Error: no se encontro el ID "
             << clave << ".\n";
        return;
    }

    hoja->claves.erase(
        hoja->claves.begin() + posicion
    );

    hoja->registros.erase(
        hoja->registros.begin() + posicion
    );

    // Si la raiz tambien es hoja.
    if (hoja == raiz) {
        if (hoja->claves.empty()) {
            delete raiz;
            raiz = nullptr;
        }

        cout << "Registro con ID "
             << clave
             << " eliminado correctamente.\n";

        return;
    }

    int minimoClaves = (grado + 1) / 2;

    if (
        static_cast<int>(hoja->claves.size()) <
        minimoClaves
    ) {
        rebalancearHoja(hoja);
    } else {
        recalcularSeparadores(raiz);
    }

    cout << "Registro con ID "
         << clave
         << " eliminado correctamente.\n";
}

vector<Registro> ArbolBPlus::obtenerTodos() {
    vector<Registro> resultado;
    
    // [A IMPLEMENTAR EN EL PARCIAL]:
    // Lógica requerida:
    // 1. Bajar desde la raíz usando siempre el hijo[0] hasta llegar a la primera hoja (la más a la izquierda).
    // 2. Recorrer los registros de esa hoja e insertarlos en 'resultado'.
    // 3. Usar el puntero 'siguiente_hoja' para saltar a la próxima hoja.
    // 4. Repetir hasta que 'siguiente_hoja' sea nullptr.
    // Esto simula un comportamiento O(n) extremadamente rápido típico de las bases de datos (Full Table Scan).
    
    if (raiz == nullptr) {
        return resultado;
    }

    NodoBPlus* cursor = raiz;

    // Llegar hasta la hoja situada más a la izquierda.
    while (!cursor->es_hoja) {
        cursor = cursor->hijos[0];
    }

    // Recorrer todas las hojas enlazadas.
    while (cursor != nullptr) {
        for (const Registro& registro : cursor->registros) {
            resultado.push_back(registro);
        }

        cursor = cursor->siguiente_hoja;
    }

    return resultado;
}

// =========================================================================
// MÉTODOS DE PERSISTENCIA (ARCHIVO DE TEXTO)
// =========================================================================

void ArbolBPlus::guardarEnArchivo() {
    ofstream archivo(nombre_archivo);

    // [A IMPLEMENTAR EN EL PARCIAL]:
    // 1. Invocar 'obtenerTodos()' o hacer el recorrido manual de hojas.
    // 2. Por cada registro obtenido, llamar a 'registro.serializar()' y escribir esa cadena en el archivo.
    // 3. Añadir un salto de línea (endl) por cada registro.
    
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo "
             << nombre_archivo << ".\n";
        return;
    }

    vector<Registro> registros = obtenerTodos();

    for (const Registro& registro : registros) {
        archivo << registro.serializar() << '\n';
    }

    archivo.close();

    cout << "[Persistencia] Se guardaron "
         << registros.size()
         << " registros en "
         << nombre_archivo << ".\n";
}
    
void ArbolBPlus::cargarDesdeArchivo() {
    ifstream archivo(nombre_archivo);
    
    // [A IMPLEMENTAR EN EL PARCIAL]:
    // 1. Leer línea por línea usando `getline(archivo, linea)`.
    // 2. Partir/Separar (Split) el string basándose en la coma ','.
    // 3. Convertir la primera parte a entero (ID).
    // 4. Pasar la segunda parte como string (Datos).
    // 5. Llamar al método `insertar(id, datos)` del mismo árbol B+ para poblarlo en memoria RAM.

    if (!archivo.is_open()) {
        cout << "No existe archivo previo '"
             << nombre_archivo
             << "'. Se creara al guardar.\n";
        return;
    }

    string linea;
    int registrosCargados = 0;
    int numeroLinea = 0;

    while (getline(archivo, linea)) {
        numeroLinea++;

        if (linea.empty()) {
            continue;
        }

        // Solo la primera coma separa la clave de los datos.
        // Los datos pueden contener comas adicionales.
        size_t posicionComa = linea.find(',');

        if (posicionComa == string::npos) {
            cerr << "Advertencia: linea "
                 << numeroLinea
                 << " invalida en "
                 << nombre_archivo << ".\n";
            continue;
        }

        string textoClave = linea.substr(0, posicionComa);
        string datos = linea.substr(posicionComa + 1);

        try {
            size_t caracteresProcesados = 0;
            int clave = stoi(
                textoClave,
                &caracteresProcesados
            );

            if (caracteresProcesados != textoClave.size()) {
                cerr << "Advertencia: ID invalido en la linea "
                     << numeroLinea << ".\n";
                continue;
            }

            insertar(clave, datos);
            registrosCargados++;
        }
        catch (const invalid_argument&) {
            cerr << "Advertencia: ID invalido en la linea "
                 << numeroLinea << ".\n";
        }
        catch (const out_of_range&) {
            cerr << "Advertencia: ID fuera de rango en la linea "
                 << numeroLinea << ".\n";
        }
    }

    archivo.close();

    cout << "[Persistencia] Se cargaron "
         << registrosCargados
         << " registros desde "
         << nombre_archivo << ".\n";
}