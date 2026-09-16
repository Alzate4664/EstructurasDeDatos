/* 
 * ============================================================================
 * LIBRERÍAS ESTÁNDAR DE C UTILIZADAS
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* 
 * ============================================================================
 * CONSTANTES FUNDAMENTALES DEL ÁRBOL B+
 * ============================================================================
 */
#define ORDER 4
#define MAX_KEYS (ORDER - 1)

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
typedef struct {
    char nombre[50];
    char ciudad[50];
    float saldo;
} Record;

typedef struct Node {
    bool is_leaf;
    int num_keys;
    int keys[MAX_KEYS];
    Record values[MAX_KEYS];
    struct Node* children[ORDER];
    struct Node* next;
} Node;

// NUEVO: Objeto Árbol B+ que encapsula el estado. Permite instanciar múltiples árboles.
typedef struct {
    Node* root;
    char filename[100];
    bool show_logs;
} BPlusTree;

/* 
 * ============================================================================
 * PROTOTIPOS DE FUNCIONES (Enfoque Orientado a Objetos en C)
 * ============================================================================
 */
BPlusTree* bpt_create_tree(const char* filename);
Node* bpt_create_node(bool is_leaf);
void bpt_insert(BPlusTree* tree, int key, Record record);
void bpt_insert_non_full(BPlusTree* tree, Node* node, int key, Record record);
void bpt_split_child(BPlusTree* tree, Node* parent, int i, Node* child);
Record* bpt_search(BPlusTree* tree, int key);
bool bpt_update(BPlusTree* tree, int key, Record new_record);
bool bpt_delete_key(BPlusTree* tree, int key);
void bpt_save_data(BPlusTree* tree);
void bpt_load_data(BPlusTree* tree);
void bpt_display_all(BPlusTree* tree);
void bpt_print_tree(BPlusTree* tree);
void bpt_print_tree_recursive(Node* node, int level);
void bpt_print_file_contents(BPlusTree* tree);
void clear_input_buffer();

// Implementación de funciones

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

BPlusTree* bpt_create_tree(const char* filename) {
    BPlusTree* tree = (BPlusTree*)malloc(sizeof(BPlusTree));
    tree->root = NULL;
    tree->show_logs = false;
    strncpy(tree->filename, filename, sizeof(tree->filename) - 1);
    tree->filename[sizeof(tree->filename) - 1] = '\0';
    return tree;
}

Node* bpt_create_node(bool is_leaf) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->is_leaf = is_leaf;
    new_node->num_keys = 0;
    new_node->next = NULL;
    for (int i = 0; i < ORDER; i++) {
        new_node->children[i] = NULL;
    }
    return new_node;
}

Record* bpt_search(BPlusTree* tree, int key) {
    if (tree->root == NULL) return NULL;
    
    Node* current = tree->root;
    while (!current->is_leaf) {
        int i = 0;
        while (i < current->num_keys && key >= current->keys[i]) i++;
        if (tree->show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG %s] Búsqueda: Descendiendo al hijo en la ruta %d..." ANSI_COLOR_RESET "\n", tree->filename, i);
        current = current->children[i];
    }
    
    if (tree->show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG %s] Búsqueda: Escaneando hoja en busca del ID %d..." ANSI_COLOR_RESET "\n", tree->filename, key);
    for (int i = 0; i < current->num_keys; i++) {
        if (current->keys[i] == key) {
            if (tree->show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG %s] Búsqueda: ID %d encontrado." ANSI_COLOR_RESET "\n", tree->filename, key);
            return &(current->values[i]);
        }
    }
    if (tree->show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG %s] Búsqueda: ID %d no existe." ANSI_COLOR_RESET "\n", tree->filename, key);
    return NULL;
}

void bpt_insert(BPlusTree* tree, int key, Record record) {
    if (tree->root == NULL) {
        tree->root = bpt_create_node(true);
        tree->root->keys[0] = key;
        tree->root->values[0] = record;
        tree->root->num_keys = 1;
        return;
    }
    
    if (bpt_search(tree, key) != NULL) {
        bpt_update(tree, key, record);
        return;
    }

    if (tree->root->num_keys == MAX_KEYS) {
        Node* new_root = bpt_create_node(false);
        new_root->children[0] = tree->root;
        bpt_split_child(tree, new_root, 0, tree->root);
        tree->root = new_root;
    }
    
    bpt_insert_non_full(tree, tree->root, key, record);
}

void bpt_split_child(BPlusTree* tree, Node* parent, int i, Node* child) {
    if (tree->show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG %s] SPLIT de Nodo..." ANSI_COLOR_RESET "\n", tree->filename);
    Node* new_node = bpt_create_node(child->is_leaf);
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
    }
}

void bpt_insert_non_full(BPlusTree* tree, Node* node, int key, Record record) {
    int i = node->num_keys - 1;
    
    if (node->is_leaf) {
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = record;
        node->num_keys++;
        if (tree->show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG %s] Inserción finalizada en hoja." ANSI_COLOR_RESET "\n", tree->filename);
    } else {
        while (i >= 0 && node->keys[i] > key) i--;
        i++;
        if (node->children[i]->num_keys == MAX_KEYS) {
            bpt_split_child(tree, node, i, node->children[i]);
            if (key > node->keys[i]) i++;
        }
        bpt_insert_non_full(tree, node->children[i], key, record);
    }
}

bool bpt_update(BPlusTree* tree, int key, Record new_record) {
    if (tree->root == NULL) return false;
    Node* current = tree->root;
    while (!current->is_leaf) {
        int i = 0;
        while (i < current->num_keys && key >= current->keys[i]) i++;
        current = current->children[i];
    }
    for (int i = 0; i < current->num_keys; i++) {
        if (current->keys[i] == key) {
            if (tree->show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG %s] Sobrescribiendo campos..." ANSI_COLOR_RESET "\n", tree->filename);
            current->values[i] = new_record;
            return true;
        }
    }
    return false;
}

bool bpt_delete_key(BPlusTree* tree, int key) {
    if (tree->root == NULL) return false;
    Node* current = tree->root;
    while (!current->is_leaf) {
        int i = 0;
        while (i < current->num_keys && key >= current->keys[i]) i++;
        current = current->children[i];
    }
    for (int i = 0; i < current->num_keys; i++) {
        if (current->keys[i] == key) {
            if (tree->show_logs) printf(ANSI_COLOR_MAGENTA "  [LOG %s] Desplazando llaves izq para borrar..." ANSI_COLOR_RESET "\n", tree->filename);
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

void bpt_save_data(BPlusTree* tree) {
    FILE* file = fopen(tree->filename, "w");
    if (!file) {
        printf("Error al abrir %s para guardar.\n", tree->filename);
        return;
    }
    if (tree->root == NULL) {
        fclose(file);
        return;
    }
    Node* current = tree->root;
    while (!current->is_leaf) current = current->children[0];
    while (current != NULL) {
        for (int i = 0; i < current->num_keys; i++) {
            fprintf(file, "%d,%s,%s,%.2f\n", 
                current->keys[i], current->values[i].nombre,
                current->values[i].ciudad, current->values[i].saldo);
        }
        current = current->next;
    }
    fclose(file);
}

void bpt_load_data(BPlusTree* tree) {
    FILE* file = fopen(tree->filename, "r");
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
            bpt_insert(tree, key, r);
        }
    }
    fclose(file);
}

void bpt_display_all(BPlusTree* tree) {
    if (tree->root == NULL) {
        printf("Arbol %s vacio.\n", tree->filename);
        return;
    }
    Node* current = tree->root;
    while (!current->is_leaf) current = current->children[0];
    printf("\n--- Todos los Registros de [%s] ---\n", tree->filename);
    printf("%-5s | %-20s | %-15s | %-10s\n", "ID", "Nombre", "Ciudad", "Saldo");
    printf("------------------------------------------------------------\n");
    while (current != NULL) {
        for (int i = 0; i < current->num_keys; i++) {
            printf("%-5d | %-20s | %-15s | $%-9.2f\n", 
                current->keys[i], current->values[i].nombre,
                current->values[i].ciudad, current->values[i].saldo);
        }
        current = current->next;
    }
    printf("------------------------------------------------------------\n");
}

void bpt_print_tree_recursive(Node* node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) printf("    ");
    if (node->is_leaf) {
        printf(ANSI_COLOR_GREEN "🌿 [HOJA]: " ANSI_COLOR_RESET "| ");
        for (int i = 0; i < node->num_keys; i++) {
            printf(ANSI_COLOR_YELLOW "%d " ANSI_COLOR_RESET "(%s - $%.2f) | ", 
                node->keys[i], node->values[i].nombre, node->values[i].saldo);
        }
        printf("\n");
    } else {
        printf(ANSI_COLOR_BLUE "🔵 [NODO INTERNO]: " ANSI_COLOR_RESET "| ");
        for (int i = 0; i < node->num_keys; i++) {
            printf(ANSI_COLOR_YELLOW "%d " ANSI_COLOR_RESET "| ", node->keys[i]);
        }
        printf("\n");
    }
    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            bpt_print_tree_recursive(node->children[i], level + 1);
        }
    }
}

void bpt_print_tree(BPlusTree* tree) {
    printf("\n" ANSI_COLOR_CYAN "=== ESTRUCTURA DEL ARBOL [%s] ===" ANSI_COLOR_RESET "\n", tree->filename);
    if (tree->root == NULL) printf("El arbol esta vacio.\n");
    else bpt_print_tree_recursive(tree->root, 0);
    printf(ANSI_COLOR_CYAN "=================================================" ANSI_COLOR_RESET "\n");
}

void bpt_print_file_contents(BPlusTree* tree) {
    printf("\n" ANSI_COLOR_CYAN "=== CONTENIDO FISICO DE [%s] ===" ANSI_COLOR_RESET "\n", tree->filename);
    FILE* file = fopen(tree->filename, "r");
    if (!file) {
        printf(ANSI_COLOR_RED "No se pudo abrir el archivo." ANSI_COLOR_RESET "\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) printf("📄 %s", line);
    fclose(file);
    printf(ANSI_COLOR_CYAN "=================================================" ANSI_COLOR_RESET "\n");
}

/* ============================================================================
 * INTERFAZ PRINCIPAL CON SOPORTE MULTI-ARBOL
 * ============================================================================ */

int main() {
    // Permite manejar MÚLTIPLES ÁRBOLES SIMULTÁNEAMENTE instanciando objetos BPlusTree
    
    char filename[100];
    printf(ANSI_COLOR_GREEN "Bienvenido al Gestor de Arboles B+ (Multi-Instancia)" ANSI_COLOR_RESET "\n");
    printf("Ingrese el nombre del archivo de datos a cargar (ej: 'datos.txt' o 'empleados.txt'): ");
    scanf("%s", filename);
    clear_input_buffer();
    
    // Instanciamos el árbol B+ pasándole el nombre del archivo
    BPlusTree* active_tree = bpt_create_tree(filename);
    bpt_load_data(active_tree);
    active_tree->show_logs = true;
    
    int opcion;
    int key;
    Record temp_record;
    
    do {
        printf("\n--- GESTOR DE ÁRBOL B+ | Base Activa: [%s] ---\n", active_tree->filename);
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
        clear_input_buffer();
        
        switch(opcion) {
            case 1:
                printf(ANSI_COLOR_CYAN "=== CREAR REGISTRO ===" ANSI_COLOR_RESET "\n");
                printf("Ingrese ID (numero entero): ");
                scanf("%d", &key);
                clear_input_buffer();
                
                if (bpt_search(active_tree, key) != NULL) {
                    printf(ANSI_COLOR_RED "Error: El ID %d ya existe." ANSI_COLOR_RESET "\n", key);
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
                    
                    bpt_insert(active_tree, key, temp_record);
                    bpt_save_data(active_tree);
                    printf(ANSI_COLOR_GREEN "=> Registro insertado." ANSI_COLOR_RESET "\n");
                    bpt_print_tree(active_tree);
                    bpt_print_file_contents(active_tree);
                }
                break;
            case 2:
                printf("Ingrese ID a buscar: ");
                scanf("%d", &key);
                clear_input_buffer();
                Record* val = bpt_search(active_tree, key);
                if (val) {
                    printf(ANSI_COLOR_GREEN "\n--- ENCONTRADO ---" ANSI_COLOR_RESET "\n");
                    printf("ID: %d | %s | %s | $%.2f\n", key, val->nombre, val->ciudad, val->saldo);
                } else {
                    printf(ANSI_COLOR_RED "=> ID no encontrado." ANSI_COLOR_RESET "\n");
                }
                break;
            case 3:
                printf("Ingrese ID a actualizar: ");
                scanf("%d", &key);
                clear_input_buffer();
                if (bpt_search(active_tree, key) == NULL) {
                    printf(ANSI_COLOR_RED "Error: ID no encontrado." ANSI_COLOR_RESET "\n");
                } else {
                    printf("Nuevo Nombre: ");
                    fgets(temp_record.nombre, 50, stdin);
                    temp_record.nombre[strcspn(temp_record.nombre, "\n")] = 0;
                    printf("Nueva Ciudad: ");
                    fgets(temp_record.ciudad, 50, stdin);
                    temp_record.ciudad[strcspn(temp_record.ciudad, "\n")] = 0;
                    printf("Nuevo Saldo: ");
                    scanf("%f", &temp_record.saldo);
                    clear_input_buffer();
                    
                    bpt_update(active_tree, key, temp_record);
                    bpt_save_data(active_tree);
                    printf(ANSI_COLOR_GREEN "=> Actualizado." ANSI_COLOR_RESET "\n");
                    bpt_print_tree(active_tree);
                    bpt_print_file_contents(active_tree);
                }
                break;
            case 4:
                printf("Ingrese ID a eliminar: ");
                scanf("%d", &key);
                clear_input_buffer();
                if (bpt_delete_key(active_tree, key)) {
                    bpt_save_data(active_tree);
                    printf(ANSI_COLOR_GREEN "=> Eliminado." ANSI_COLOR_RESET "\n");
                    bpt_print_tree(active_tree);
                    bpt_print_file_contents(active_tree);
                } else {
                    printf(ANSI_COLOR_RED "Error: ID no encontrado." ANSI_COLOR_RESET "\n");
                }
                break;
            case 5:
                bpt_display_all(active_tree);
                break;
            case 6:
                bpt_print_tree(active_tree);
                break;
            case 7:
                printf(ANSI_COLOR_MAGENTA "Cerrando Gestor y liberando..." ANSI_COLOR_RESET "\n");
                // TODO: Free memory function
                break;
            default:
                printf(ANSI_COLOR_RED "Opcion no valida." ANSI_COLOR_RESET "\n");
        }
    } while (opcion != 7);
    
    return 0;
}
