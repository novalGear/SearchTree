#include "os_tree.hpp"
#include <vector>
#include <algorithm>

namespace OS_Tree {

int SearchTree::height(Node* node) {
    if (!node) { return 0; }
    return node->height_;
}

int SearchTree::subtree_size(Node* node) {
    if (!node) { return 0; }
    return node->subtree_size_;
}

void SearchTree::upd_height(Node* node) {
    if (!node) { return; }
    int left_height  = 0;
    int right_height = 0;

    if (node->left)  { left_height  = height(node->left);  }
    if (node->right) { right_height = height(node->right); }

    node->height_ = std::max(left_height, right_height) + 1;
}

void SearchTree::upd_subtree_size(Node* node) {
    if (!node) { return; }
    int left_subtree_size  = 0;
    int right_subtree_size = 0;

    if (node->left)  { left_subtree_size  = subtree_size(node->left);  }
    if (node->right) { right_subtree_size = subtree_size(node->right); }

    node->subtree_size_ = left_subtree_size + right_subtree_size + 1;
}

void SearchTree::upd_node_ctx(Node* node) {
    if (!node) { return; }
    upd_height(node);
    upd_subtree_size(node);
}

int SearchTree::getBalance(Node* node) {
    if (!node) { return 0; }
    return height(node->left) - height(node->right);
}

SearchTree::Node* SearchTree::rightRotate(Node* B) {
    Node* A = B->left;
    Node* C = A->right;

    A->right = B;
    B->left = C;

    upd_node_ctx(B);
    upd_node_ctx(A);
    return A;
}

SearchTree::Node* SearchTree::leftRotate(Node* A) {
    Node* B = A->right;
    Node* C = B->left;

    B->left = A;
    A->right = C;

    upd_node_ctx(A);
    upd_node_ctx(B);
    return B;
}

SearchTree::Node* SearchTree::insert(Node* node, int key) {
    if (!node) return new Node(key);

    if (key < node->key_) {
        node->left = insert(node->left, key);
        upd_node_ctx(node);
    } else if (key > node->key_) {
        node->right = insert(node->right, key);
        upd_node_ctx(node);
    } else {
        return node; // дубликаты не допускаются
    }

    node->height_ = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Левый левый
    if (balance > 1 && key < node->left->key_) {
        return rightRotate(node);
    }

    // Правый правый
    if (balance < -1 && key > node->right->key_) {
        return leftRotate(node);
    }

    // Левый правый
    if (balance > 1 && key > node->left->key_) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Правый левый
    if (balance < -1 && key < node->right->key_) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void SearchTree::insert(int key) {
    root_ = insert(root_, key);
}
void SearchTree::writeDot(const std::string& filename) {
    std::ofstream file(filename);
    file << "digraph G {\n";
    file << "  rankdir=TB;\n";
    file << "  ordering=out;\n";
    file << "  node [shape=record];\n";

    if (root_) {
        std::vector<Node*> queue = {root_};
        while (!queue.empty()) {
            Node* current = queue.back();
            queue.pop_back();

            // Используем ключ как уникальный ID
            std::string nodeId = "node_" + std::to_string(current->key_);

            file << "  " << nodeId << " [label=\"{k: " << current->key_
                 << "|h: " << current->height_ << "|sz: " << current->subtree_size_ << "}\"];\n";

            if (current->left) {
                file << "  " << nodeId << " -> node_" << current->left->key_ << ";\n";
                queue.push_back(current->left);
            }
            if (current->right) {
                file << "  " << nodeId << " -> node_" << current->right->key_ << ";\n";
                queue.push_back(current->right);
            }
        }
    }

    file << "}\n";
    file.close();
}

int SearchTree::rank(Node* node, int x) {
    if (!node) { return 0; }

    int curr_key = node->key_;
    if (curr_key > x) {
        // только в левом поддереве могут найтись искомые узлы
        return rank(node->left, x);
    } else {
        // значит текущий узел и все в его левом поддереве подходят
        int matching = 1;
        if (node->left) { matching += subtree_size(node->left); }

        return rank(node->right, x) + matching;
    }
}

int SearchTree::count_in_range(int a, int b) {
    if (a >= b) {return 0; }
    return rank(root_, b) - rank(root_, a);
}

SearchTree::SearchTree() : root_(nullptr) {}

void SearchTree::destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

SearchTree::~SearchTree() {
    destroy(root_);
}

}
