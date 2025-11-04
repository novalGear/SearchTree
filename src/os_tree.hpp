#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

namespace OS_Tree {

const int POISON_INDEX = -1;

class SearchTree {
private:

    struct Node {
        int key_;
        int height_ = 1;
        int subtree_size_ = 1;

        int index_;
        int left_index_   = -1;
        int right_index_  = -1;
        int parent_index_;

        bool is_active = true;

        Node(int key, int self_index, int parent_index = -1) : key_(key), index_(self_index), parent_index_(parent_index) {}
    };

    std::vector<Node>   nodes_;
    // std::stack<int>     free_indices_;
    int sentinel_index_ = -1;
    int size_ = 0;                          // количество активных узлов

    bool is_node_active(int index) const;
    int real_root() const;
    // accessors
    int get_node_key(int node_index)     const;
    int height(int node_index)  const;
    // возвращает количество элементов в поддереве, включая node
    int subtree_size(int node_index)    const;

    // обновление состояния
    void upd_height(int node_index);
    void upd_subtree_size(int node_index);
    void upd_node_ctx(int node_index);

    // балансировка
    int get_balance(int node_index) const;
    void replace_child(int prev_child_index, int new_child_index, int parent_index);
    int right_rotate(int B);
    int left_rotate(int A);
    int balance_node(int node_index, int key);

    // вставка
    void insert(int node_index, int key);
    void add_node(int parent_index, int key);
    // void remove(int key);

    // Подсчитывает число узлов в поддереве со значением key <= x
    int node_rank(int node_index, int x) const;
    void print_tree_structure(std::ostream& os, int node_index) const;

public:


    class NodeNavigator {
    public:
        const SearchTree* tree_ = nullptr;
        int current_index_ = -1;
        int last_visited_  = -1;
        bool is_current_index_valid() const;

        NodeNavigator (const SearchTree* tree, int index = -1): tree_(tree), current_index_(index) {}
        bool go_left();
        bool go_right();
        bool go_parent();
        bool go_root();

        bool set_index(int new_index);

        bool is_valid()     const;
        bool is_root()      const;
        bool has_left()     const;
        bool has_right()    const;
        bool has_parent()   const;

        int get_parent()        const;
        // int get_index()         const;
        int get_key()           const;
        int get_height()        const;
        int get_subtree_size()  const;

    //
    //     bool operator==(const iterator& other) const;
    //     bool operator!=(const iterator& other) const;
    };

    SearchTree();
    SearchTree(SearchTree&& other) noexcept = default;
    SearchTree& operator=(SearchTree&& other) noexcept = default;
    SearchTree(const SearchTree& other) = delete;
    SearchTree& operator=(const SearchTree& other) = delete;
    ~SearchTree() = default;

    void insert(int key);

    NodeNavigator get_root_navigator() const;
    NodeNavigator get_navigator_by_index(int node_index) const;
    NodeNavigator get_navigator_by_key(int node_index, int key) const;
    // NodeNavigator find_navigator(int key) const;

    // тоже, что и node_rank, только для всего дерева
    int rank(int x) const;
    // возвращает число узлов с key: key in (a, b]
    int count_in_range(int a, int b) const;

    // графическая отладка
    void writeDot(const std::string& filename) const;
    void print_tree_structure(std::ostream& os) const;

};

}
