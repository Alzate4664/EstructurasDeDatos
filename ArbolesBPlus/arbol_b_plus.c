#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ORDER 4
#define MAX_KEYS (ORDER - 1)

// Estructura de datos requerida
typedef struct {
    int id;
    char nombre[50];
    char ciudad[50];
    float saldo;
} Record;

// Nodo del árbol B+
typedef struct Node {
    bool is_leaf;
    int num_keys;
    int keys[ORDER]; // Espacio extra para el overflow temporal (antes del split)
    struct Node* children[ORDER + 1]; // Punteros a hijos (hasta ORDER+1 temporalmente)
    Record* records[ORDER];    // Punteros a datos (para hojas)
    struct Node* next;            // Puntero al siguiente nodo hoja
} Node;

Node* root = NULL;

// Crea un nuevo registro
Record* create_record(int id, const char* nombre, const char* ciudad, float saldo) {
    Record* rec = (Record*)malloc(sizeof(Record));
    rec->id = id;
    strncpy(rec->nombre, nombre, 49);
    rec->nombre[49] = '\0';
    strncpy(rec->ciudad, ciudad, 49);
    rec->ciudad[49] = '\0';
    rec->saldo = saldo;
    return rec;
}

// Crea un nuevo nodo
Node* create_node(bool is_leaf) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    node->next = NULL;
    for (int i = 0; i < ORDER + 1; i++) {
        node->children[i] = NULL;
    }
    for (int i = 0; i < ORDER; i++) {
        node->records[i] = NULL;
    }
    return node;
}

// ---------------------------------------------------------
// BUSCAR (READ)
// ---------------------------------------------------------
Record* search(Node* node, int id) {
    if (node == NULL) return NULL;

    int i = 0;
    while (i < node->num_keys && id > node->keys[i]) {
        i++;
    }

    if (node->is_leaf) {
        if (i < node->num_keys && node->keys[i] == id) {
            return node->records[i];
        }
        return NULL;
    } else {
        return search(node->children[i], id);
    }
}

// ---------------------------------------------------------
// INSERTAR (CREATE)
// ---------------------------------------------------------
void insert_into_leaf(Node* leaf, Record* rec) {
    int i = leaf->num_keys - 1;
    while (i >= 0 && leaf->keys[i] > rec->id) {
        leaf->keys[i + 1] = leaf->keys[i];
        leaf->records[i + 1] = leaf->records[i];
        i--;
    }
    leaf->keys[i + 1] = rec->id;
    leaf->records[i + 1] = rec;
    leaf->num_keys++;
}

Node* split_leaf(Node* leaf) {
    Node* new_leaf = create_node(true);
    int split_point = (MAX_KEYS + 1) / 2;

    new_leaf->num_keys = leaf->num_keys - split_point;
    for (int i = 0; i < new_leaf->num_keys; i++) {
        new_leaf->keys[i] = leaf->keys[i + split_point];
        new_leaf->records[i] = leaf->records[i + split_point];
    }
    leaf->num_keys = split_point;
    
    new_leaf->next = leaf->next;
    leaf->next = new_leaf;

    return new_leaf;
}

void insert_into_internal(Node* parent, int key, Node* right_child) {
    int i = parent->num_keys - 1;
    while (i >= 0 && parent->keys[i] > key) {
        parent->keys[i + 1] = parent->keys[i];
        parent->children[i + 2] = parent->children[i + 1];
        i--;
    }
    parent->keys[i + 1] = key;
    parent->children[i + 2] = right_child;
    parent->num_keys++;
}

Node* split_internal(Node* node, int* up_key) {
    Node* new_node = create_node(false);
    int split_point = MAX_KEYS / 2;
    *up_key = node->keys[split_point];

    new_node->num_keys = node->num_keys - split_point - 1;
    for (int i = 0; i < new_node->num_keys; i++) {
        new_node->keys[i] = node->keys[i + split_point + 1];
        new_node->children[i] = node->children[i + split_point + 1];
    }
    new_node->children[new_node->num_keys] = node->children[node->num_keys];
    
    node->num_keys = split_point;
    return new_node;
}

// Función recursiva para insertar y manejar divisiones
Node* insert_recursive(Node* node, Record* rec, int* up_key) {
    if (node->is_leaf) {
        insert_into_leaf(node, rec);
        if (node->num_keys > MAX_KEYS) {
            Node* new_leaf = split_leaf(node);
            *up_key = new_leaf->keys[0];
            return new_leaf;
        }
        return NULL;
    } else {
        int i = 0;
        while (i < node->num_keys && rec->id > node->keys[i]) {
            i++;
        }
        int child_up_key;
        Node* new_child = insert_recursive(node->children[i], rec, &child_up_key);
        
        if (new_child != NULL) {
            insert_into_internal(node, child_up_key, new_child);
            if (node->num_keys > MAX_KEYS) {
                return split_internal(node, up_key);
            }
        }
        return NULL;
    }
}

void insert(Record* rec) {
    if (root == NULL) {
        root = create_node(true);
        root->keys[0] = rec->id;
        root->records[0] = rec;
        root->num_keys = 1;
        return;
    }

    if (search(root, rec->id) != NULL) {
        printf("Error: El registro con ID %d ya existe.\n", rec->id);
        free(rec);
        return;
    }

    int up_key;
    Node* new_node = insert_recursive(root, rec, &up_key);
    if (new_node != NULL) {
        Node* new_root = create_node(false);
        new_root->keys[0] = up_key;
        new_root->children[0] = root;
        new_root->children[1] = new_node;
        new_root->num_keys = 1;
        root = new_root;
    }
    printf("Registro con ID %d insertado exitosamente.\n", rec->id);
}

// ---------------------------------------------------------
// ACTUALIZAR (UPDATE)
// ---------------------------------------------------------
bool update(int id, const char* nombre, const char* ciudad, float saldo) {
    Record* rec = search(root, id);
    if (rec != NULL) {
        if (nombre != NULL && strlen(nombre) > 0) {
            strncpy(rec->nombre, nombre, 49);
            rec->nombre[49] = '\0';
        }
        if (ciudad != NULL && strlen(ciudad) > 0) {
            strncpy(rec->ciudad, ciudad, 49);
            rec->ciudad[49] = '\0';
        }
        if (saldo >= 0) {
            rec->saldo = saldo;
        }
        return true;
    }
    return false;
}

// ---------------------------------------------------------
// ELIMINAR (DELETE) - Simplificado
// ---------------------------------------------------------
bool delete_record(int id) {
    if (root == NULL) return false;

    Node* node = root;
    while (!node->is_leaf) {
        int i = 0;
        while (i < node->num_keys && id > node->keys[i]) i++;
        node = node->children[i];
    }

    for (int i = 0; i < node->num_keys; i++) {
        if (node->keys[i] == id) {
            free(node->records[i]);
            // Desplazar elementos a la izquierda
            for (int j = i; j < node->num_keys - 1; j++) {
                node->keys[j] = node->keys[j + 1];
                node->records[j] = node->records[j + 1];
            }
            node->num_keys--;
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------
// VER ESTRUCTURA
// ---------------------------------------------------------
void print_tree_recursive(Node* node, char* prefix, bool is_last, const char* branch_condition, bool is_root) {
    if (node == NULL) return;
    
    if (!is_root) {
        printf("%s", prefix);
        printf("%s", is_last ? "+-- " : "|-- ");
    }
    
    if (branch_condition != NULL) {
        printf("[%s] ", branch_condition);
    }
    
    char new_prefix[256] = "";
    if (!is_root) {
        snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "|   ");
    }
    
    if (node->is_leaf) {
        printf("\033[1;35mNodo Hoja:\033[0m\n");
        for (int i = 0; i < node->num_keys; i++) {
            printf("%s", new_prefix);
            bool is_last_record = (i == node->num_keys - 1);
            printf("%s", is_last_record ? "+-- " : "|-- ");
            Record* r = node->records[i];
            printf("{ID: \033[1;36m%d\033[0m, Nombre: '\033[1;32m%s\033[0m', Ciudad: '\033[1;33m%s\033[0m', Saldo: \033[1;31m%.2f\033[0m}\n", r->id, r->nombre, r->ciudad, r->saldo);
        }
    } else {
        printf("\033[1;34mNodo Interno:\033[0m Claves = [");
        for (int i = 0; i < node->num_keys; i++) {
            printf("\033[1;36m%d\033[0m", node->keys[i]);
            if (i < node->num_keys - 1) printf(", ");
        }
        printf("]\n");
        
        int num_children = node->num_keys + 1;
        for (int i = 0; i < num_children; i++) {
            char condition[100];
            if (i == 0) {
                sprintf(condition, "IDs \033[1;31m<\033[0m %d", node->keys[0]);
            } else if (i == num_children - 1) {
                sprintf(condition, "IDs \033[1;32m>=\033[0m %d", node->keys[i - 1]);
            } else {
                sprintf(condition, "%d \033[1;33m<=\033[0m IDs \033[1;31m<\033[0m %d", node->keys[i - 1], node->keys[i]);
            }
            
            bool is_last_child = (i == num_children - 1);
            print_tree_recursive(node->children[i], new_prefix, is_last_child, condition, false);
        }
    }
}

void print_tree_structure() {
    printf("\n--- ESTRUCTURA DEL ARBOL B+ (ENRUTAMIENTO) ---\n");
    if (root == NULL) {
        printf("El arbol esta vacio.\n");
    } else {
        print_tree_recursive(root, "", true, "Raiz", true);
    }
    printf("----------------------------------------------\n");
}

// ---------------------------------------------------------
// MOSTRAR TODOS
// ---------------------------------------------------------
void print_all() {
    if (root == NULL) {
        printf("El árbol está vacío.\n");
        return;
    }
    
    // Ir a la primera hoja
    Node* node = root;
    while (!node->is_leaf) {
        node = node->children[0];
    }
    
    printf("\n--- REGISTROS EN EL ARBOL ---\n");
    int count = 0;
    while (node != NULL) {
        for (int i = 0; i < node->num_keys; i++) {
            Record* r = node->records[i];
            printf("ID: %d | Nombre: %s | Ciudad: %s | Saldo: %.2f\n", r->id, r->nombre, r->ciudad, r->saldo);
            count++;
        }
        node = node->next;
    }
    if (count == 0) printf("No hay registros.\n");
    printf("-----------------------------\n");
}

// ---------------------------------------------------------
// MENÚ PRINCIPAL
// ---------------------------------------------------------
void clean_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int opcion;
    int id;
    char nombre[50];
    char ciudad[50];
    float saldo;
    char input_buffer[100];

    while (1) {
        printf("\n\033[1;36m=== SISTEMA CRUD: ARBOL B+ (C) ===\033[0m\n");
        printf("\033[1;32m1.\033[0m CREATE (Insertar registro)\n");
        printf("\033[1;32m2.\033[0m READ (Buscar por ID)\n");
        printf("\033[1;32m3.\033[0m UPDATE (Actualizar registro)\n");
        printf("\033[1;32m4.\033[0m DELETE (Eliminar registro)\n");
        printf("\033[1;32m5.\033[0m MOSTRAR TODOS\n");
        printf("\033[1;31m6.\033[0m SALIR\n");
        printf("\033[1;36m==================================\033[0m\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            clean_stdin();
            continue;
        }
        clean_stdin();

        switch (opcion) {
            case 1:
                printf("Ingrese ID (entero): ");
                scanf("%d", &id);
                clean_stdin();
                
                printf("Ingrese Nombre: ");
                fgets(nombre, 50, stdin);
                nombre[strcspn(nombre, "\n")] = 0;
                
                printf("Ingrese Ciudad: ");
                fgets(ciudad, 50, stdin);
                ciudad[strcspn(ciudad, "\n")] = 0;
                
                printf("Ingrese Saldo: ");
                scanf("%f", &saldo);
                clean_stdin();
                
                insert(create_record(id, nombre, ciudad, saldo));
                break;
                
            case 2:
                printf("Ingrese ID a buscar: ");
                scanf("%d", &id);
                clean_stdin();
                
                Record* rec = search(root, id);
                if (rec != NULL) {
                    printf("\nRegistro encontrado:\n");
                    printf("ID: %d | Nombre: %s | Ciudad: %s | Saldo: %.2f\n", rec->id, rec->nombre, rec->ciudad, rec->saldo);
                } else {
                    printf("Error: Registro no encontrado.\n");
                }
                break;
                
            case 3:
                printf("Ingrese ID a actualizar: ");
                scanf("%d", &id);
                clean_stdin();
                
                if (search(root, id) != NULL) {
                    printf("Nuevo Nombre (presione Enter para mantener el actual): ");
                    fgets(nombre, 50, stdin);
                    nombre[strcspn(nombre, "\n")] = 0;
                    
                    printf("Nueva Ciudad (presione Enter para mantener el actual): ");
                    fgets(ciudad, 50, stdin);
                    ciudad[strcspn(ciudad, "\n")] = 0;
                    
                    printf("Nuevo Saldo (ingrese -1 para mantener el actual): ");
                    fgets(input_buffer, 100, stdin);
                    if (input_buffer[0] == '\n') {
                        saldo = -1;
                    } else {
                        sscanf(input_buffer, "%f", &saldo);
                    }
                    
                    if (update(id, nombre, ciudad, saldo)) {
                        printf("Registro actualizado con exito.\n");
                    }
                } else {
                    printf("Error: Registro no encontrado.\n");
                }
                break;
                
            case 4:
                printf("Ingrese ID a eliminar: ");
                scanf("%d", &id);
                clean_stdin();
                
                if (delete_record(id)) {
                    printf("Registro eliminado con exito.\n");
                } else {
                    printf("Error: Registro no encontrado.\n");
                }
                break;
                
            case 5:
                print_all();
                break;
                
            case 6:
                printf("Saliendo...\n");
                return 0;
                
            default:
                printf("Opcion no valida.\n");
        }
        
        if (opcion >= 1 && opcion <= 4) {
            print_tree_structure();
        }
    }
    
    return 0;
}
