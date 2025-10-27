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
    // возвращает количество элементов в поддереве, включая node
    int subtree_size(Node* node);

    void upd_height(Node* node);
    void upd_subtree_size(Node* node);
    void upd_node_ctx(Node* node);

    int getBalance(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);


    // Подсчитывает число узлов в поддереве со значением key <= x
    int node_rank(Node* node, int x);

public:
    SearchTree();
    ~SearchTree();
    void insert(int key);

    // графическая отладка
    void writeDot(const std::string& filename);

    // тоже, что и node_rank, только для всего дерева
    int rank(int x);

    // возвращает число узлов с key: key in (a, b]
    int count_in_range(int a, int b);
};

}
