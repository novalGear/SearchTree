#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <vector>

namespace OS_Tree {

class SearchTree {
private:
    struct Node {
        int key_;
        int height_ = 1;
        int subtree_size_ = 1;
        std::unique_ptr<Node> left  = nullptr;
        std::unique_ptr<Node> right = nullptr;

        Node(int key) : key_(key) {}
    };

    std::unique_ptr<Node> root_;

    // стандартные методы OS дерева
    void insert(std::unique_ptr<Node>& node, int key);

    int height(std::unique_ptr<Node>& node);
    // возвращает количество элементов в поддереве, включая node
    int subtree_size(std::unique_ptr<Node>& node);

    void upd_height(std::unique_ptr<Node>& node);
    void upd_subtree_size(std::unique_ptr<Node>& node);
    void upd_node_ctx(std::unique_ptr<Node>& node);

    int get_balance(std::unique_ptr<Node>& node);
    void right_rotate(std::unique_ptr<Node>& y);
    void left_rotate(std::unique_ptr<Node>& x);


    // Подсчитывает число узлов в поддереве со значением key <= x
    int node_rank(std::unique_ptr<Node>& node, int x);

public:
    SearchTree() = default;
    SearchTree(SearchTree&& other) noexcept = default;
    SearchTree& operator=(SearchTree&& other) noexcept = default;
    SearchTree(const SearchTree& other) = delete;
    SearchTree& operator=(const SearchTree& other) = delete;
    ~SearchTree() = default;

    void insert(int key);

    // графическая отладка
    void writeDot(const std::string& filename);

    // тоже, что и node_rank, только для всего дерева
    int rank(int x);

    // возвращает число узлов с key: key in (a, b]
    int count_in_range(int a, int b);
};

}
