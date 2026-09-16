/* 
 * ============================================================================
 * LIBRERÍAS ESTÁNDAR DE C UTILIZADAS
 * ============================================================================
 */
#include <stdio.h>   // Para operaciones de Entrada/Salida (printf, scanf, archivos, etc.)
#include <stdlib.h>  // Para asignación dinámica de memoria (malloc) y conversiones (atoi, atof)
#include <string.h>  // Para el manejo y manipulación de cadenas de texto (strcpy, strtok, strcspn)
#include <stdbool.h> // Para utilizar variables booleanas (true, false) de manera más legible

/* 
 * ============================================================================
 * CONSTANTES FUNDAMENTALES DEL ÁRBOL B+
 * ============================================================================
 */
// ORDER define el "Orden" del árbol, es decir, la cantidad MÁXIMA de hijos que puede tener un nodo.
#define ORDER 4

// MAX_KEYS define la cantidad MÁXIMA de llaves/datos que puede almacenar un solo nodo.
// Matemáticamente en un B-Tree o B+, una regla estricta es que MaxKeys = Orden - 1.
#define MAX_KEYS (ORDER - 1)

// Colores ANSI para dar formato y visualizar operaciones en la terminal de Linux/Mac.
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/* 
 * ============================================================================
 * ESTRUCTURAS DE DATOS (VARIABLES COMPUESTAS)
 * ============================================================================
 */
// Estructura Record: Actúa como el "Payload" o la carga útil de datos.
// En un árbol B+ real, las hojas almacenan un puntero a un registro en disco o el registro completo.
typedef struct {
    char nombre[50]; // Almacena el nombre del cliente (máx 50 caracteres)
    char ciudad[50]; // Almacena la ciudad de procedencia
    float saldo;     // Almacena el dinero/saldo en una variable con decimales
} Record;

// Estructura Node: Es el bloque fundamental de construcción del Árbol B+.
typedef struct Node {
    bool is_leaf;                // Bandera que indica si este nodo es una HOJA (true) o un NODO INTERNO (false).
    int num_keys;                // Contador actual de cuántas llaves tiene guardadas este nodo específico.
    int keys[MAX_KEYS];          // Arreglo de enteros para almacenar las llaves (IDs de los registros) ordenadas.
    Record values[MAX_KEYS];     // Arreglo con la carga útil. Solo tienen sentido/se usan si 'is_leaf' es true.
    struct Node* children[ORDER];// Arreglo de punteros a los nodos hijos. (Tamaño = Orden).
    struct Node* next;           // Puntero vital del Árbol B+: Conecta secuencialmente las hojas como una lista enlazada.
} Node;

/* 
 * ============================================================================
 * VARIABLES GLOBALES
 * ============================================================================
 */
Node* root = NULL;                   // Puntero inicial que siempre apuntará a la raíz (cima) del árbol.
const char* FILENAME = "datos.txt";  // Nombre del archivo plano en disco donde se persiste la información.
bool show_logs = false;              // Interruptor (flag) para encender/apagar las trazas de visualización del algoritmo.

// Prototipos de funciones
Node* create_node(bool is_leaf);
void insert(int key, Record record);
void _insert_non_full(Node* node, int key, Record record);
void _split_child(Node* parent, int i, Node* child);
Record* search(int key);
bool update(int key, Record new_record);
bool delete_key(int key);
void save_data();
void load_data();
void display_all();
void clear_input_buffer();
void print_tree_recursive(Node* node, int level);
void print_tree();
void print_file_contents();

// Limpia el buffer para evitar bucles infinitos con scanf
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

Node* create_node(bool is_leaf) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->is_leaf = is_leaf;
    new_node->num_keys = 0;
    new_node->next = NULL;
    for (int i = 0; i < ORDER; i++) {
        new_node->children[i] = NULL;
    }
    return new_node;
}

Record* search(int key) {
    if (root == NULL) return NULL;
    
    Node* current = root;
    while (!current->is_leaf) {
        int i = 0;
        while (i < current->num_keys && key >= current->keys[i]) {
            i++;
        }
        if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Búsqueda: Descendiendo al hijo en la ruta %d..." ANSI_COLOR_RESET "\n", i);
        current = current->children[i];
    }
    
    if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Búsqueda: Escaneando hoja en busca del ID %d..." ANSI_COLOR_RESET "\n", key);
    for (int i = 0; i < current->num_keys; i++) {
        if (current->keys[i] == key) {
            if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Búsqueda: ID %d encontrado en la posición %d de la hoja." ANSI_COLOR_RESET "\n", key, i);
            return &(current->values[i]);
        }
    }
    if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Búsqueda: El ID %d no existe." ANSI_COLOR_RESET "\n", key);
    return NULL;
}

/*
 * FUNCION: insert (Crear)
 * PROPOSITO: Manejar la inserción de un nuevo registro en el Árbol B+.
 * ALGORITMO B+:
 * 1. Si el árbol está vacío, creamos una raíz que por defecto es hoja.
 * 2. Si la raíz está completamente llena (num_keys == MAX_KEYS), la altura del árbol DEBE crecer.
 *    Para ello, se crea una nueva raíz vacía y se divide la raíz antigua como hija de la nueva.
 * 3. Posteriormente (o si no estaba llena), se baja a insertar de forma segura en las hojas usando _insert_non_full.
 */
void insert(int key, Record record) {
    if (root == NULL) {
        root = create_node(true);
        root->keys[0] = key;
        root->values[0] = record;
        root->num_keys = 1;
        return;
    }
    
    if (search(key) != NULL) {
        update(key, record);
        return;
    }

    if (root->num_keys == MAX_KEYS) {
        Node* new_root = create_node(false);
        new_root->children[0] = root;
        _split_child(new_root, 0, root);
        root = new_root;
    }
    
    _insert_non_full(root, key, record);
}

/* 
 * FUNCION: _split_child
 * PROPOSITO: Maneja la división (split) de un nodo cuando se ha llenado (alcanza MAX_KEYS).
 * ALGORITMO B+:
 * En un Árbol B+, la división tiene un comportamiento diferente si el nodo a dividir es una HOJA o un nodo INTERNO:
 * 1. Si es HOJA: Se divide a la mitad. La mitad derecha se va a un nuevo nodo. 
 *    IMPORTANTE: La llave que sube al padre TAMBIÉN se queda en la nueva hoja (porque las hojas deben contener
 *    absolutamente todos los datos). Las hojas mantienen la lista enlazada conectando 'next'.
 * 2. Si es nodo INTERNO: La llave de en medio SUBE al nodo padre, pero NO se queda abajo. Los nodos
 *    internos solo usan las llaves como "señales de tránsito" para saber a qué hijo ir.
 */
void _split_child(Node* parent, int i, Node* child) {
    if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Nodo lleno (Capacidad máxima alcanzada). Realizando operación de SPLIT (División de Nodo)..." ANSI_COLOR_RESET "\n");
    Node* new_node = create_node(child->is_leaf);
    int mid = MAX_KEYS / 2;
    
    if (child->is_leaf) {
        new_node->num_keys = MAX_KEYS - mid;
        for(int j = 0; j < new_node->num_keys; j++) {
            new_node->keys[j] = child->keys[mid + j];
            new_node->values[j] = child->values[mid + j];
        }
        child->num_keys = mid;
        
        new_node->next = child->next;
        child->next = new_node;
        
        for (int j = parent->num_keys; j > i; j--) {
            parent->children[j + 1] = parent->children[j];
            parent->keys[j] = parent->keys[j - 1];
        }
        parent->children[i + 1] = new_node;
        parent->keys[i] = new_node->keys[0]; 
        parent->num_keys++;
        if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] División de Hoja: Copiando llaves a nueva hoja y subiendo copia al padre." ANSI_COLOR_RESET "\n");
    } else {
        new_node->num_keys = MAX_KEYS - mid - 1;
        for(int j = 0; j < new_node->num_keys; j++) {
            new_node->keys[j] = child->keys[mid + 1 + j];
        }
        for(int j = 0; j <= new_node->num_keys; j++) {
            new_node->children[j] = child->children[mid + 1 + j];
        }
        child->num_keys = mid;
        
        for (int j = parent->num_keys; j > i; j--) {
            parent->children[j + 1] = parent->children[j];
            parent->keys[j] = parent->keys[j - 1];
        }
        parent->children[i + 1] = new_node;
        parent->keys[i] = child->keys[mid];
        parent->num_keys++;
        if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] División Interna: Moviendo llave central al padre." ANSI_COLOR_RESET "\n");
    }
}

/*
 * FUNCION: _insert_non_full
 * PROPOSITO: Insertar una llave (y su valor) en un nodo que sabemos que NO está lleno.
 * ALGORITMO B+:
 * - Si el nodo actual es una HOJA: Aquí es donde reside físicamente la data. Movemos las llaves y valores
 *   existentes (que sean mayores a la nueva llave) un espacio a la derecha para mantener el orden,
 *   y luego insertamos en el hueco que quedó.
 * - Si el nodo actual es INTERNO: Aquí NO guardamos la data, solo buscamos por cuál "hijo" debemos bajar.
 *   Si el hijo elegido está lleno, lo dividimos (haciendo Split) ANTES de bajar.
 */
void _insert_non_full(Node* node, int key, Record record) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Inserción: Desplazando llaves mayores hacia la derecha en la hoja..." ANSI_COLOR_RESET "\n");
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = record;
        node->num_keys++;
        if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Inserción: ID %d guardado físicamente en la hoja." ANSI_COLOR_RESET "\n", key);
    } else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;
        if (node->children[i]->num_keys == MAX_KEYS) {
            _split_child(node, i, node->children[i]);
            if (key > node->keys[i]) {
                i++;
            }
        }
        _insert_non_full(node->children[i], key, record);
    }
}

bool update(int key, Record new_record) {
    if (root == NULL) return false;
    
    Node* current = root;
    while (!current->is_leaf) {
        int i = 0;
        while (i < current->num_keys && key >= current->keys[i]) i++;
        current = current->children[i];
    }
    
    for (int i = 0; i < current->num_keys; i++) {
        if (current->keys[i] == key) {
            if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Actualización: Sobrescribiendo campos del ID %d directamente en la hoja en la posición %d." ANSI_COLOR_RESET "\n", key, i);
            current->values[i] = new_record;
            return true;
        }
    }
    return false;
}

/*
 * FUNCION: delete_key (Eliminar)
 * PROPOSITO: Eliminar de manera simplificada un registro de una hoja.
 * ALGORITMO B+:
 * 1. Desciende por el árbol utilizando las llaves de los nodos internos para llegar a la hoja apropiada.
 * 2. Si encuentra la llave en la hoja, simplemente desplaza las llaves y registros que estén a la derecha
 *    un espacio a la izquierda, sobrescribiendo (borrando) físicamente al que se quería eliminar.
 * NOTA DE CLASE: Un Árbol B+ real implementaría aquí la lógica de "Underflow" (cuando el nodo queda con menos del
 * mínimo de llaves permitidas), lo cual involucraría pedir prestado un nodo a los hermanos (Redistribución)
 * o unificarse con ellos (Merge/Fusión). Esta es una versión académica que solo realiza el borrado físico sin balanceo inverso.
 */
bool delete_key(int key) {
    if (root == NULL) return false;
    
    Node* current = root;
    while (!current->is_leaf) {
        int i = 0;
        while (i < current->num_keys && key >= current->keys[i]) i++;
        current = current->children[i];
    }
    
    for (int i = 0; i < current->num_keys; i++) {
        if (current->keys[i] == key) {
            if (show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG B+] Eliminación: Desplazando las llaves derechas hacia la izquierda para borrar el ID %d de la hoja..." ANSI_COLOR_RESET "\n", key);
            for (int j = i; j < current->num_keys - 1; j++) {
                current->keys[j] = current->keys[j + 1];
                current->values[j] = current->values[j + 1];
            }
            current->num_keys--;
            return true;
        }
    }
    return false;
}

void save_data() {
    FILE* file = fopen(FILENAME, "w");
    if (!file) {
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }
    
    if (root == NULL) {
        fclose(file);
        return;
    }
    
    Node* current = root;
    while (!current->is_leaf) {
        current = current->children[0];
    }
    
    while (current != NULL) {
        for (int i = 0; i < current->num_keys; i++) {
            fprintf(file, "%d,%s,%s,%.2f\n", 
                current->keys[i], 
                current->values[i].nombre,
                current->values[i].ciudad,
                current->values[i].saldo);
        }
        current = current->next;
    }
    
    fclose(file);
}

void load_data() {
    FILE* file = fopen(FILENAME, "r");
    if (!file) return;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        
        char* token = strtok(line, ",");
        if (token) {
            int key = atoi(token);
            
            Record r;
            char* nombre = strtok(NULL, ",");
            if (nombre) strcpy(r.nombre, nombre);
            
            char* ciudad = strtok(NULL, ",");
            if (ciudad) strcpy(r.ciudad, ciudad);
            
            char* saldo_str = strtok(NULL, ",");
            if (saldo_str) r.saldo = atof(saldo_str);
            
            insert(key, r);
        }
    }
    fclose(file);
}

void display_all() {
    if (root == NULL) {
        printf("Arbol vacio.\n");
        return;
    }
    
    Node* current = root;
    while (!current->is_leaf) {
        current = current->children[0];
    }
    
    printf("\n--- Todos los Registros (Lectura Secuencial de Hojas) ---\n");
    printf("%-5s | %-20s | %-15s | %-10s\n", "ID", "Nombre", "Ciudad", "Saldo");
    printf("------------------------------------------------------------\n");
    while (current != NULL) {
        for (int i = 0; i < current->num_keys; i++) {
            printf("%-5d | %-20s | %-15s | $%-9.2f\n", 
                current->keys[i], 
                current->values[i].nombre,
                current->values[i].ciudad,
                current->values[i].saldo);
        }
        current = current->next;
    }
    printf("------------------------------------------------------------\n");
}

void print_tree_recursive(Node* node, int level) {
    if (node == NULL) return;
    
    // Imprimir indentación basada en el nivel
    for (int i = 0; i < level; i++) {
        printf("    ");
    }
    
    if (node->is_leaf) {
        printf(ANSI_COLOR_GREEN "🌿 [HOJA]: " ANSI_COLOR_RESET);
        printf("| ");
        for (int i = 0; i < node->num_keys; i++) {
            printf(ANSI_COLOR_YELLOW "%d " ANSI_COLOR_RESET "(%s - $%.2f) | ", 
                node->keys[i], 
                node->values[i].nombre, 
                node->values[i].saldo);
        }
        printf("\n");
    } else {
        printf(ANSI_COLOR_BLUE "🔵 [NODO INTERNO]: " ANSI_COLOR_RESET);
        printf("| ");
        for (int i = 0; i < node->num_keys; i++) {
            printf(ANSI_COLOR_YELLOW "%d " ANSI_COLOR_RESET "| ", node->keys[i]);
        }
        printf("\n");
    }
    
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            print_tree_recursive(node->children[i], level + 1);
        }
    }
}

void print_tree() {
    printf("\n" ANSI_COLOR_CYAN "=== ESTRUCTURA VISUAL DEL ARBOL B+ ===" ANSI_COLOR_RESET "\n");
    if (root == NULL) {
        printf("El arbol esta vacio.\n");
    } else {
        print_tree_recursive(root, 0);
    }
    printf(ANSI_COLOR_CYAN "======================================" ANSI_COLOR_RESET "\n");
}

void print_file_contents() {
    printf("\n" ANSI_COLOR_CYAN "=== EVIDENCIA: CONTENIDO DE (datos.txt) ===" ANSI_COLOR_RESET "\n");
    FILE* file = fopen(FILENAME, "r");
    if (!file) {
        printf(ANSI_COLOR_RED "No se pudo abrir el archivo de datos." ANSI_COLOR_RESET "\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("📄 %s", line);
    }
    fclose(file);
    printf(ANSI_COLOR_CYAN "===========================================" ANSI_COLOR_RESET "\n");
}

int main() {
    load_data();
    show_logs = true; // Activar logs visuales luego de la carga inicial
    
    int opcion;
    int key;
    Record temp_record;
    
    do {
        printf("\n--- ARBOL B+ (ID, Nombre, Ciudad, Saldo) ---\n");
        printf("1. Crear (Insertar Registro)\n");
        printf("2. Leer (Buscar por ID)\n");
        printf("3. Actualizar (Modificar Registro)\n");
        printf("4. Eliminar (Borrar por ID)\n");
        printf("5. Mostrar todos (Imprimir tabla)\n");
        printf("6. Visualizar Arbol B+ (Grafico)\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            clear_input_buffer();
            continue;
        }
        clear_input_buffer(); // Limpiar el newline dejado por scanf
        
        switch(opcion) {
            case 1:
                printf(ANSI_COLOR_CYAN "=== CREAR REGISTRO ===" ANSI_COLOR_RESET "\n");
                printf("Ingrese ID (numero entero): ");
                scanf("%d", &key);
                clear_input_buffer();
                
                if (search(key) != NULL) {
                    printf(ANSI_COLOR_RED "Error: El ID %d ya existe en la base de datos." ANSI_COLOR_RESET "\n", key);
                } else {
                    printf("Ingrese Nombre: ");
                    fgets(temp_record.nombre, 50, stdin);
                    temp_record.nombre[strcspn(temp_record.nombre, "\n")] = 0;
                    
                    printf("Ingrese Ciudad: ");
                    fgets(temp_record.ciudad, 50, stdin);
                    temp_record.ciudad[strcspn(temp_record.ciudad, "\n")] = 0;
                    
                    printf("Ingrese Saldo (decimal): ");
                    scanf("%f", &temp_record.saldo);
                    clear_input_buffer();
                    
                    insert(key, temp_record);
                    save_data();
                    printf(ANSI_COLOR_GREEN "=> Registro insertado con exito." ANSI_COLOR_RESET "\n");
                    print_tree(); // Mostrar el árbol luego de crear
                    print_file_contents(); // Mostrar evidencia en archivo
                }
                break;
            case 2:
                printf(ANSI_COLOR_CYAN "=== LEER REGISTRO ===" ANSI_COLOR_RESET "\n");
                printf("Ingrese ID a buscar: ");
                scanf("%d", &key);
                clear_input_buffer();
                
                Record* val = search(key);
                if (val) {
                    printf(ANSI_COLOR_GREEN "\n--- REGISTRO ENCONTRADO ---" ANSI_COLOR_RESET "\n");
                    printf("ID:     %d\n", key);
                    printf("Nombre: %s\n", val->nombre);
                    printf("Ciudad: %s\n", val->ciudad);
                    printf("Saldo:  $%.2f\n", val->saldo);
                    printf(ANSI_COLOR_GREEN "---------------------------" ANSI_COLOR_RESET "\n");
                } else {
                    printf(ANSI_COLOR_RED "=> ID no encontrado." ANSI_COLOR_RESET "\n");
                }
                break;
            case 3:
                printf(ANSI_COLOR_CYAN "=== ACTUALIZAR REGISTRO ===" ANSI_COLOR_RESET "\n");
                printf("Ingrese ID a actualizar: ");
                scanf("%d", &key);
                clear_input_buffer();
                
                if (search(key) == NULL) {
                    printf(ANSI_COLOR_RED "Error: ID no encontrado." ANSI_COLOR_RESET "\n");
                } else {
                    printf("Ingrese Nuevo Nombre: ");
                    fgets(temp_record.nombre, 50, stdin);
                    temp_record.nombre[strcspn(temp_record.nombre, "\n")] = 0;
                    
                    printf("Ingrese Nueva Ciudad: ");
                    fgets(temp_record.ciudad, 50, stdin);
                    temp_record.ciudad[strcspn(temp_record.ciudad, "\n")] = 0;
                    
                    printf("Ingrese Nuevo Saldo (decimal): ");
                    scanf("%f", &temp_record.saldo);
                    clear_input_buffer();
                    
                    update(key, temp_record);
                    save_data();
                    printf(ANSI_COLOR_GREEN "=> Registro actualizado con exito." ANSI_COLOR_RESET "\n");
                    print_tree(); // Mostrar el árbol luego de actualizar
                    print_file_contents(); // Mostrar evidencia en archivo
                }
                break;
            case 4:
                printf(ANSI_COLOR_CYAN "=== ELIMINAR REGISTRO ===" ANSI_COLOR_RESET "\n");
                printf("Ingrese ID a eliminar: ");
                scanf("%d", &key);
                clear_input_buffer();
                
                if (delete_key(key)) {
                    save_data();
                    printf(ANSI_COLOR_GREEN "=> Eliminado con exito." ANSI_COLOR_RESET "\n");
                    print_tree(); // Mostrar el árbol luego de eliminar
                    print_file_contents(); // Mostrar evidencia en archivo
                } else {
                    printf(ANSI_COLOR_RED "Error: ID no encontrado." ANSI_COLOR_RESET "\n");
                }
                break;
            case 5:
                display_all();
                break;
            case 6:
                print_tree();
                break;
            case 7:
                printf(ANSI_COLOR_MAGENTA "Saliendo y guardando datos de forma segura..." ANSI_COLOR_RESET "\n");
                break;
            default:
                printf(ANSI_COLOR_RED "Opcion no valida." ANSI_COLOR_RESET "\n");
        }
    } while (opcion != 7);
    
    return 0;
}
