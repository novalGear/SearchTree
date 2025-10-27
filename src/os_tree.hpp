#pragma once

#include <string>
#include <fstream>
#include <vector>

namespace OS_Tree {

class SearchTree {
private:
    struct Node {
        int key_;
        int height_;
        int subtree_size_;
        Node* left;
        Node* right;

        Node(int key) : key_(key), height_(1), subtree_size_(1), left(nullptr), right(nullptr) {}
    };

    Node* root_;

    // стандартные методы OS дерева
    Node* insert(Node* node, int key);
    void destroy(Node* node);

    int height(Node* node);
    int subtree_size(Node* node);
    void upd_height(Node* node);
    void upd_subtree_size(Node* node);
    void upd_node_ctx(Node* node);

    int getBalance(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);

    // специфичные для задачи методы
    int rank(Node* node, int x);

public:
    SearchTree();
    ~SearchTree();
    void insert(int key);
    void writeDot(const std::string& filename);
    int count_in_range(int a, int b);

};

}
