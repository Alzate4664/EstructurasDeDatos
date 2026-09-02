#include "Visualizer.h"
#include "Colors.h"
#include <iostream>
#include <iomanip>

namespace Visualizer {
    void printHeader(const std::string& title) {
        std::cout << "\n" << BOLD << GREEN << "========================================================\n" << RESET;
        std::cout << BOLD << YELLOW << "   " << title << "\n" << RESET;
        std::cout << BOLD << GREEN << "========================================================\n" << RESET;
    }

    void printPerson(const Person& p) {
        std::cout << BOLD << CYAN << "Paciente: " << RESET << p.nombre 
                  << " | " << YELLOW << "Edad: " << RESET << p.edad
                  << " | " << RED << "Sangre: " << RESET << p.tipoSangre 
                  << " | " << MAGENTA << "Órgano: " << RESET << p.organo << "\n";
        std::cout << "  -> Compatibles: ";
        if (p.compatibles.empty()) {
            std::cout << RED << "Ninguno" << RESET;
        } else {
            std::cout << GREEN;
            for (size_t i = 0; i < p.compatibles.size(); i++) {
                std::cout << p.compatibles[i] << (i < p.compatibles.size() - 1 ? ", " : "");
            }
            std::cout << RESET;
        }
        std::cout << "\n";
    }

    void printLinkedList(const LinkedList& list) {
        ListNode* temp = list.head;
        std::cout << CYAN;
        while (temp) {
            std::cout << "[" << temp->data.nombre << " Edad:" << temp->data.edad << "] -> ";
            temp = temp->next;
        }
        std::cout << RED << "NULL\n" << RESET;
    }

    void printDoublyLinkedList(const DoublyLinkedList& list) {
        DLLNode* temp = list.tail;
        std::cout << RED << "NULL" << RESET;
        while (temp) {
            std::cout << MAGENTA << " <-> [" << temp->data.nombre << "]";
            temp = temp->prev;
        }
        std::cout << MAGENTA << " <-> " << RED << "NULL\n" << RESET;
    }

    void printStack(const Stack& stack) {
        StackNode* temp = stack.topNode;
        std::cout << BLUE << "       |========================|\n";
        while (temp) {
            std::cout << "       | " << YELLOW << std::left << std::setw(22) << temp->data.shortInfo() << BLUE << " |\n";
            std::cout << "       |------------------------|\n";
            temp = temp->next;
        }
        std::cout << "       |        " << RED << "FONDO" << BLUE << "           |\n";
        std::cout << "       \\========================/\n" << RESET;
    }

    void printQueue(const Queue& queue) {
        QueueNode* temp = queue.frontNode;
        std::cout << BOLD << GREEN << "[FRENTE] " << RESET;
        while (temp) {
            std::cout << YELLOW << "=> (" << temp->data.nombre << ") ";
            temp = temp->next;
        }
        std::cout << BOLD << GREEN << "=> [FINAL]\n" << RESET;
    }

    void print2DUtil(TreeNode* root, int space) {
        if (root == nullptr) return;
        space += 8;
        print2DUtil(root->right, space);
        std::cout << "\n";
        for (int i = 8; i < space; i++) std::cout << " ";
        std::cout << GREEN << "---[" << YELLOW << root->data.nombre << " (" << root->data.edad << ")" << GREEN << "]\n" << RESET;
        print2DUtil(root->left, space);
    }

    void printBST(const BST& bst) {
        std::cout << CYAN << "Representación 2D del Árbol (Rotado 90°, Izquierda es Abajo):\n" << RESET;
        print2DUtil(bst.root, 0);
        std::cout << "\n";
    }

    void printBT2DUtil(BTNode* root, int space) {
        if (root == nullptr) return;
        space += 8;
        printBT2DUtil(root->right, space);
        std::cout << "\n";
        for (int i = 8; i < space; i++) std::cout << " ";
        std::cout << GREEN << "---[" << YELLOW << root->data.nombre << " (" << root->data.edad << ")" << GREEN << "]\n" << RESET;
        printBT2DUtil(root->left, space);
    }

    void printBinaryTree(const BinaryTree& bt) {
        std::cout << CYAN << "Representación 2D del Árbol Binario:\n" << RESET;
        printBT2DUtil(bt.root, 0);
        std::cout << "\n";
    }

    void printHashTable(const HashTable& ht) {
        for (int i = 0; i < ht.tableSize; i++) {
            std::cout << BOLD << BLUE << "[Bucket " << std::setw(2) << std::setfill('0') << i << "] " << RESET << "-> ";
            if (ht.table[i].empty()) {
                std::cout << RED << "Vacío\n" << RESET;
            } else {
                for (const auto& person : ht.table[i]) {
                    std::cout << YELLOW << "[" << person.nombre << "] " << CYAN << "--> " << RESET;
                }
                std::cout << RED << "NULL\n" << RESET;
            }
        }
    }

    void printGraph(const Graph& graph) {
        std::cout << CYAN << "========================================================\n" << RESET;
        std::cout << YELLOW << "   Representación del Grafo (Red Médica)\n" << RESET;
        std::cout << CYAN << "========================================================\n" << RESET;
        for (auto const& pair : graph.vertices) {
            GraphNode* node = pair.second;
            std::cout << BOLD << BLUE << " ┌─ Vértice: " << RESET << CYAN << node->data.nombre 
                      << YELLOW << " (" << node->data.tipoSangre << ", " << node->data.organo << ")\n" << RESET;
                      
            if (node->matches.empty()) {
                std::cout << BOLD << BLUE << " └─ Aristas: " << RED << "Ninguna conexión compatible.\n" << RESET;
            } else {
                std::cout << BOLD << BLUE << " └─ Aristas: " << RESET;
                for (size_t i = 0; i < node->matches.size(); i++) {
                    std::cout << GREEN << "──> [" << node->matches[i]->data.nombre << " | " << node->matches[i]->data.organo << "]" << RESET;
                    if (i < node->matches.size() - 1) std::cout << " ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    }
}