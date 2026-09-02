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
#include "BinaryTree.h"
#include <queue>

BTNode::BTNode(Person p) : data(p), left(nullptr), right(nullptr) {}

BinaryTree::BinaryTree() : root(nullptr) {}

BinaryTree::~BinaryTree() {
    destruirArbol(root);
}

void BinaryTree::destruirArbol(BTNode* node) {
    if (!node) return;
    destruirArbol(node->left);
    destruirArbol(node->right);
    delete node;
}

void BinaryTree::insertar(Person p) {
    BTNode* newNode = new BTNode(p);
    if (!root) {
        root = newNode;
        return;
    }
    
    std::queue<BTNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        BTNode* temp = q.front();
        q.pop();
        
        if (!temp->left) {
            temp->left = newNode;
            return;
        } else {
            q.push(temp->left);
        }
        
        if (!temp->right) {
            temp->right = newNode;
            return;
        } else {
            q.push(temp->right);
        }
    }
}
