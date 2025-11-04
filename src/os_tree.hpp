#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <stack>

namespace OS_Tree {

class SearchTree {
private:

    struct Node {
        int key_;
        int height_ = 1;            // высота поддерева
        int subtree_size_ = 1;      // высота поддерева ВКЛЮЧАЯ node

        int index_;                 // собственный индекс в nodes
        int left_index_   = -1;
        int right_index_  = -1;
        int parent_index_;

        bool is_active = true;      // помечаем что узел рабочий, false у sentinel node

        Node(int key, int self_index, int parent_index = -1) : key_(key), index_(self_index), parent_index_(parent_index) {}
    };

    std::vector<Node>   nodes_;             // массив узлов
    // std::stack<int>     free_indices_;
    int sentinel_index_ = -1;               // неприкасаемый мнимый корень
    int size_ = 0;                          // количество активных узлов

    // проверка валидности индекса узла
    bool is_node_active(int index) const;

    // получить индекс прямого потомка sentinel node, реальный корень дерева
    int real_root() const;
    // accessors
    int get_node_key(int node_index)     const;
    int height(int node_index)  const;
    // возвращает количество элементов в поддереве, ВКЛЮЧАЯ node
    int subtree_size(int node_index)    const;

    // обновление состояния
    void upd_height(int node_index);
    void upd_subtree_size(int node_index);
    // обновить всю информацию узла, используя информацию потомков
    // в данном случае height и subtree_size
    void upd_node_ctx(int node_index);

    // балансировка
    int get_balance(int node_index) const;
    // заменить потомка (обычно при повороте)
    void replace_child(int prev_child_index, int new_child_index, int parent_index);
    int right_rotate(int B);
    int left_rotate(int A);
    // балансирование узла с node_index (не рекурсивное!)
    int balance_node(int node_index, int key);

    // вставка ключа в поддерево с корнем в node_index
    void insert(int node_index, int key);
    // ТОЛЬКО добавляет узел к родителю и обновляет его состояние, остальное дерево еще нужно балансировать!
    void add_node(int parent_index, int key);
    // void remove(int key);

    // Подсчитывает число узлов в поддереве со значением key <= x
    int node_rank(int node_index, int x) const;
    void print_tree_structure(std::ostream& os, int node_index) const;

public:

    // класс получился сильно медленным, но очень уж хотелось обертку над node просто чтобы передвигаться по дереву
    class NodeNavigator {
    public:
        // к какому дереву относится
        const SearchTree* tree_ = nullptr;
        // node на которой сейчас находится navigator
        int current_index_ = -1;
        // последний посещенный node
        int last_visited_  = -1;
        bool is_current_index_valid() const;

        NodeNavigator (const SearchTree* tree, int index = -1): tree_(tree), current_index_(index) {}
        // передвижения
        bool go_left();
        bool go_right();
        bool go_parent();
        bool go_root();

        // перепрыгнуть на node с нужным индексом (обычно после поворота)
        bool set_index(int new_index);

        // проверки
        bool is_root()      const;
        bool has_left()     const;
        bool has_right()    const;
        bool has_parent()   const;

        // accessors к полям node, на которой сейчас находится navigator
        int get_parent()        const;
        int get_key()           const;
        int get_height()        const;
        int get_subtree_size()  const;
    };

    // SearchTree не управляет ресурсами вручную, только сразу добавляет sentinel node в дерево
    // поэтому Rule of Zero не нарушено
    SearchTree();
    SearchTree(SearchTree&& other) noexcept = default;
    SearchTree& operator=(SearchTree&& other) noexcept = default;
    SearchTree(const SearchTree& other) = default;
    SearchTree& operator=(const SearchTree& other) = default;
    ~SearchTree() = default;

    void insert(int key);

    NodeNavigator get_root_navigator() const;
    // просто создаст навигатор от соответствующего узла
    NodeNavigator get_navigator_by_index(int node_index) const;
    // ищет узел с key или место для вставки в поддереве с корнем в node_index
    NodeNavigator get_navigator_by_key(int node_index, int key) const;
    // NodeNavigator find_navigator(int key) const;

    // тоже, что и node_rank, только для всего дерева
    int rank(int x) const;
    // возвращает число узлов с key: key in (a, b]
    int count_in_range(int a, int b) const;

    // графическая отладка
    // Графический дамп через html, используется для тестирования структуры дерева
    void writeDot(const std::string& filename) const;
    // отображение дерева в виде скобочной записи preorder со значениями ключей в узлах
    void print_tree_structure(std::ostream& os) const;
};

}
