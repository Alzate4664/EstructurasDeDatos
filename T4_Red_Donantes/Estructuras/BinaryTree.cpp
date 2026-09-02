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
