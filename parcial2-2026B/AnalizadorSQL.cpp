/**
 * Archivo: AnalizadorSQL.cpp
 * Propósito: Implementación del Analizador Léxico y Sintáctico rudimentario.
 *            Este archivo procesa los strings ingresados por el usuario,
 *            extrae los tokens (como el ID o los datos a guardar) y llama a
 *            las funciones del árbol (ArbolBPlus.h).
 */
#include "AnalizadorSQL.h"

// Inicializa el analizador acoplando el puntero del árbol B+ que se pasó en el main
AnalizadorSQL::AnalizadorSQL(ArbolBPlus* base_datos)
    : bd(base_datos), tablaCreada(true), nombreTabla("usuarios") {}

string AnalizadorSQL::aMayusculas(string cadena) {
    string cadenaMayus = "";
    // Itera letra por letra convirtiendo a su equivalente en mayúscula
    for (char c : cadena) cadenaMayus += toupper(c);
    return cadenaMayus;
}

string AnalizadorSQL::recortar(string cadena) {
    size_t inicio = cadena.find_first_not_of(" \t\r\n");

    if (inicio == string::npos) {
        return "";
    }

    size_t fin = cadena.find_last_not_of(" \t\r\n");

    return cadena.substr(inicio, fin - inicio + 1);
}

void AnalizadorSQL::ejecutarConsulta(string consulta) {
    if (consulta.empty()) return;

    // Stringstream permite leer palabras de un string separadas por espacios
    stringstream ss(consulta);
    string comando;

    // Extrae la primera palabra (Ej. "INSERT", "SELECT")
    ss >> comando;
    comando = aMayusculas(comando); // Normalizamos a mayúsculas para las comparaciones

    // Derivación según familia de instrucciones SQL
    if (comando == "CREATE" || comando == "DROP") {
        analizarDDL(consulta, comando);
    } else if (comando == "SELECT" || comando == "INSERT" || comando == "DELETE") {
        analizarDQL_DML(consulta, comando);
    } else if (comando == "HELP") {
        mostrarAyuda();
    } else {
        cout << "Error: Comando SQL no reconocido. Escriba HELP para mas informacion.\n";
    }
}

void AnalizadorSQL::analizarDDL(string consulta, string comando) {
    string consultaLimpia = recortar(consulta);
    // [A IMPLEMENTAR EN EL PARCIAL]:
    // Ejemplo: CREATE TABLE usuarios (id INT, nombre STR);
    // Permitir punto y coma al final.
    if (!consultaLimpia.empty() && consultaLimpia.back() == ';') {
        consultaLimpia.pop_back();
        consultaLimpia = recortar(consultaLimpia);
    }

    string consultaMayuscula = aMayusculas(consultaLimpia);

    // ==========================================
    // CREATE
    // ==========================================
    if (comando == "CREATE") {

        // CREATE INDEX se implementará en el siguiente paso.
        if (consultaMayuscula.find("CREATE INDEX") == 0) {
            cout << "[Ejecutando DDL] -> Analizando creacion de INDICE secundario...\n";
            return;
        }

        stringstream ss(consultaLimpia);

        string palabraCreate;
        string palabraTable;
        string nombre;

        ss >> palabraCreate >> palabraTable >> nombre;

        if (aMayusculas(palabraTable) != "TABLE" || nombre.empty()) {
            cout << "Error: sintaxis esperada: "
                 << "CREATE TABLE <nombre> (columnas...)\n";
            return;
        }

        if (tablaCreada) {
            if (aMayusculas(nombre) == aMayusculas(nombreTabla)) {
                cout << "Error: la tabla '" << nombre
                     << "' ya existe.\n";
            } else {
                cout << "Error: este motor administra una sola tabla a la vez. "
                     << "Actualmente existe '" << nombreTabla << "'.\n";
            }

            return;
        }

        tablaCreada = true;
        nombreTabla = nombre;

        cout << "Tabla '" << nombreTabla
             << "' creada correctamente.\n";

        return;
    }

    // ==========================================
    // DROP
    // ==========================================
    if (comando == "DROP") {
        stringstream ss(consultaLimpia);

        string palabraDrop;
        string palabraTable;
        string nombre;

        ss >> palabraDrop >> palabraTable >> nombre;

        if (aMayusculas(palabraTable) != "TABLE" || nombre.empty()) {
            cout << "Error: sintaxis esperada: DROP TABLE <nombre>\n";
            return;
        }

        if (!tablaCreada) {
            cout << "Error: no existe ninguna tabla para eliminar.\n";
            return;
        }

        if (aMayusculas(nombre) != aMayusculas(nombreTabla)) {
            cout << "Error: la tabla '" << nombre
                 << "' no existe.\n";
            return;
        }

        // Eliminar todos los registros y nodos del Árbol B+.
        bd->vaciar();

        // Sobrescribir el archivo de persistencia con la tabla vacía.
        bd->guardarEnArchivo();

        tablaCreada = false;
        nombreTabla = "";

        cout << "Tabla '" << nombre
             << "' eliminada correctamente.\n";
    }
}

void AnalizadorSQL::analizarDQL_DML(string consulta, string comando) {
    // [A IMPLEMENTAR EN EL PARCIAL]:
    // Aquí el estudiante debe decidir cómo extraer los datos (id y contenido)
    // del string `consulta`. Puede usar manipulacion de strings (`find`, `substr`) o Expresiones Regulares (regex).

    if (comando == "INSERT") {
        // Ejemplo esperado:
        // INSERT INTO usuarios VALUES (10, 'Juan Perez')

        stringstream ssInsert(consulta);

        string palabraInsert;
        string palabraInto;
        string tablaInsert;

        ssInsert >> palabraInsert >> palabraInto >> tablaInsert;

        if (
            aMayusculas(palabraInto) != "INTO" ||
            tablaInsert.empty()
        ) {
            cout << "Error: sintaxis esperada: "
                 << "INSERT INTO <tabla> VALUES (...)\n";
            return;
        }

        if (!tablaCreada) {
            cout << "Error: no existe una tabla creada. "
                 << "Use CREATE TABLE primero.\n";
            return;
        }

        if (aMayusculas(tablaInsert) != aMayusculas(nombreTabla)) {
            cout << "Error: la tabla '" << tablaInsert
                 << "' no existe.\n";
            return;
        }

        string consultaMayuscula = aMayusculas(consulta);
        size_t posicionValues = consultaMayuscula.find("VALUES");

        if (posicionValues == string::npos) {
            cout << "Error: la instruccion INSERT debe contener VALUES.\n";
            return;
        }

        size_t parentesisInicial = consulta.find('(', posicionValues);
        size_t parentesisFinal = consulta.rfind(')');

        if (
            parentesisInicial == string::npos ||
            parentesisFinal == string::npos ||
            parentesisFinal <= parentesisInicial
        ) {
            cout << "Error: los valores deben estar entre parentesis.\n";
            return;
        }

        string contenido = consulta.substr(
            parentesisInicial + 1,
            parentesisFinal - parentesisInicial - 1
        );

        // La primera coma separa el ID del resto de los datos.
        size_t posicionComa = contenido.find(',');

        if (posicionComa == string::npos) {
            cout << "Error: debe ingresar un ID y los datos del registro.\n";
            return;
        }

        string textoClave = recortar(
            contenido.substr(0, posicionComa)
        );

        string datos = recortar(
            contenido.substr(posicionComa + 1)
        );

        if (textoClave.empty() || datos.empty()) {
            cout << "Error: el ID y los datos no pueden estar vacios.\n";
            return;
        }

        // Quitar comillas simples o dobles alrededor de los datos.
        if (
            datos.size() >= 2 &&
            (
                (datos.front() == '\'' && datos.back() == '\'') ||
                (datos.front() == '"' && datos.back() == '"')
            )
        ) {
            datos = datos.substr(1, datos.size() - 2);
        }

        try {
            size_t caracteresProcesados = 0;
            int clave = stoi(textoClave, &caracteresProcesados);

            // Evitar aceptar valores como "10abc".
            if (caracteresProcesados != textoClave.size()) {
                cout << "Error: el ID debe ser un numero entero.\n";
                return;
            }

            bd->insertar(clave, datos);
        }
        catch (const invalid_argument&) {
            cout << "Error: el ID debe ser un numero entero.\n";
        }
        catch (const out_of_range&) {
            cout << "Error: el ID esta fuera del rango permitido.\n";
        }
    }
    else if (comando == "SELECT") {
        // Ejemplos:
        // SELECT * FROM usuarios
        // SELECT * FROM usuarios WHERE id = 10

        stringstream ssSelect(consulta);

        string palabraSelect;
        string asterisco;
        string palabraFrom;
        string tablaSelect;

        ssSelect >> palabraSelect
                 >> asterisco
                 >> palabraFrom
                 >> tablaSelect;

        // SELECT * FROM usuarios;
        // Quitar ; del nombre si viene inmediatamente después.
        if (!tablaSelect.empty() && tablaSelect.back() == ';') {
            tablaSelect.pop_back();
        }

        if (
            asterisco != "*" ||
            aMayusculas(palabraFrom) != "FROM" ||
            tablaSelect.empty()
        ) {
            cout << "Error: sintaxis esperada: "
                 << "SELECT * FROM <tabla>\n";
            return;
        }

        if (!tablaCreada) {
            cout << "Error: no existe una tabla creada. "
                 << "Use CREATE TABLE primero.\n";
            return;
        }

        if (aMayusculas(tablaSelect) != aMayusculas(nombreTabla)) {
            cout << "Error: la tabla '" << tablaSelect
                 << "' no existe.\n";
            return;
        }

        string consultaMayuscula = aMayusculas(consulta);
        size_t posicionWhere = consultaMayuscula.find("WHERE");

        if (posicionWhere != string::npos) {
            size_t posicionIgual = consulta.find(
                '=',
                posicionWhere
            );

            if (posicionIgual == string::npos) {
                cout << "Error: la condicion WHERE debe contener '='.\n";
                return;
            }

            string textoClave = recortar(
                consulta.substr(posicionIgual + 1)
            );

            // Permitir el punto y coma final.
            if (!textoClave.empty() && textoClave.back() == ';') {
                textoClave.pop_back();
                textoClave = recortar(textoClave);
            }

            if (textoClave.empty()) {
                cout << "Error: debe indicar el ID que desea buscar.\n";
                return;
            }

            try {
                size_t caracteresProcesados = 0;
                int clave = stoi(
                    textoClave,
                    &caracteresProcesados
                );

                if (caracteresProcesados != textoClave.size()) {
                    cout << "Error: el ID debe ser un numero entero.\n";
                    return;
                }

                string resultado = bd->buscar(clave);

                if (resultado.empty()) {
                    cout << "No se encontro un registro con el ID "
                         << clave << ".\n";
                } else {
                    cout << "ID: " << clave
                         << " | Datos: " << resultado << "\n";
                }
            }
            catch (const invalid_argument&) {
                cout << "Error: el ID debe ser un numero entero.\n";
            }
            catch (const out_of_range&) {
                cout << "Error: el ID esta fuera del rango permitido.\n";
            }
        } else {
            vector<Registro> registros = bd->obtenerTodos();

            if (registros.empty()) {
                cout << "La tabla no contiene registros.\n";
                return;
            }

            cout << "\n=== REGISTROS DE LA TABLA ===\n";

            for (const Registro& registro : registros) {
                cout << "ID: " << registro.clave
                     << " | Datos: " << registro.datos << "\n";
            }

            cout << "Total de registros: "
                 << registros.size() << "\n";
        }
    }
    else if (comando == "DELETE") {
        // Ejemplo esperado: DELETE FROM tabla WHERE id = 10;
        // 1. Extraer el ID después del "WHERE id = "
        // 2. Ejecutar: bd->eliminar(id);
        cout << "[Ejecutando DML] -> Analizando DELETE. (Debe llamar a ArbolBPlus::eliminar)\n";
    }
}

void AnalizadorSQL::mostrarAyuda() {
    // Configuración de colores estándar ANSI para terminales modernas
    const string RESET = "\033[0m";
    const string BOLD_YELLOW = "\033[1;33m";
    const string BOLD_CYAN = "\033[1;36m";
    const string BOLD_GREEN = "\033[1;32m";
    const string BOLD_WHITE = "\033[1;37m";

    cout << BOLD_YELLOW << "\n=== Sistema Gestor SQL basado en Árboles B+ ===" << RESET << "\n";
    cout << BOLD_WHITE << "Comandos Soportados (Esqueleto):" << RESET << "\n";

    // Categoría DDL
    cout << BOLD_YELLOW << "  [DDL - Lenguaje de Definición de Datos]" << RESET << "\n";
    cout << BOLD_CYAN << "    Sintaxis: CREATE TABLE <nombre> (columnas...)" << RESET << "\n";
    cout << BOLD_GREEN << "    Ejemplo : CREATE TABLE usuarios (id INT, nombre STR)" << RESET << "\n\n";

    cout << BOLD_CYAN << "    Sintaxis: CREATE INDEX <nombre> ON <tabla> (columna)" << RESET << "\n";
    cout << BOLD_GREEN << "    Ejemplo : CREATE INDEX idx_nombre ON usuarios (nombre)" << RESET << "\n\n";

    cout << BOLD_CYAN << "    Sintaxis: DROP TABLE <nombre>" << RESET << "\n";
    cout << BOLD_GREEN << "    Ejemplo : DROP TABLE usuarios" << RESET << "\n\n";

    // Categoría DML/DQL
    cout << BOLD_YELLOW << "  [DQL / DML - Manipulación y Consulta]" << RESET << "\n";
    cout << BOLD_CYAN << "    Sintaxis: INSERT INTO <nombre> VALUES (<id>, <datos>)" << RESET << "\n";
    cout << BOLD_GREEN << "    Ejemplo : INSERT INTO usuarios VALUES (10, 'Juan Perez, 25')" << RESET << "\n\n";

    cout << BOLD_CYAN << "    Sintaxis: SELECT * FROM <nombre>" << RESET << "\n";
    cout << BOLD_GREEN << "    Ejemplo : SELECT * FROM usuarios" << RESET << "\n\n";

    cout << BOLD_CYAN << "    Sintaxis: SELECT * FROM <nombre> WHERE id = <id>" << RESET << "\n";
    cout << BOLD_GREEN << "    Ejemplo : SELECT * FROM usuarios WHERE id = 10" << RESET << "\n\n";

    cout << BOLD_CYAN << "    Sintaxis: DELETE FROM <nombre> WHERE id = <id>" << RESET << "\n";
    cout << BOLD_GREEN << "    Ejemplo : DELETE FROM usuarios WHERE id = 10" << RESET << "\n\n";

    // Controles Base
    cout << BOLD_YELLOW << "  [Otros Comandos]" << RESET << "\n";
    cout << BOLD_CYAN << "    HELP  - Muestra este menu" << RESET << "\n";
    cout << BOLD_CYAN << "    EXIT  - Guarda los datos y sale del programa" << RESET << "\n";
    cout << BOLD_YELLOW << "================================================" << RESET << "\n\n";
}
