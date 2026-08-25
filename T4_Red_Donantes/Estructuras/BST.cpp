#include "BST.h"

TreeNode::TreeNode(Person p) : data(p), left(nullptr), right(nullptr) {}

BST::BST() : root(nullptr) {}

BST::~BST() { 
    destruirArbol(root); // Limpieza recursiva
}

// Inserción recursiva: Encuentra el lugar adecuado bajando por el árbol
void BST::insertarNodo(TreeNode*& node, Person p) {
    // Si encontramos un lugar vacío, aquí va el nodo
    if (!node) { 
        node = new TreeNode(p); 
        return; 
    }
    
    // Si la edad del nuevo paciente es MENOR a la del nodo actual, va a la IZQUIERDA
    if (p.edad < node->data.edad) {
        insertarNodo(node->left, p);
    } 
    // Si es MAYOR o IGUAL, va a la DERECHA
    else {
        insertarNodo(node->right, p);
    }
}

// Función auxiliar para borrar ramas post-orden y no dejar basura en memoria
void BST::destruirArbol(TreeNode* node) {
    if (!node) return;
    destruirArbol(node->left);
    destruirArbol(node->right);
    delete node;
}

// Interfaz pública para insertar
void BST::insertar(Person p) { 
    insertarNodo(root, p); 
}