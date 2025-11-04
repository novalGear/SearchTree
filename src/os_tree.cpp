#include "os_tree.hpp"
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <queue>

namespace OS_Tree {

#ifdef DEBUG
#define DBG_PRINT(...) printf("%s:%d    ", __func__, __LINE__);  \
                       printf(__VA_ARGS__)
#else
#define DBG_PRINT(...)
#endif


    // SearchTree() = default;
    // SearchTree(SearchTree&& other) noexcept = default;
    // SearchTree& operator=(SearchTree&& other) noexcept = default;
    // SearchTree(const SearchTree& other) = delete;
    // SearchTree& operator=(const SearchTree& other) = delete;
    // ~SearchTree() = default;

SearchTree::SearchTree() {
    sentinel_index_ = nodes_.size();
    nodes_.emplace_back(-999, sentinel_index_, -1);
}

int SearchTree::real_root() const {
    return nodes_[sentinel_index_].left_index_;
}

// проверки =====================================================================================================================//

bool SearchTree::is_node_active(int index) const {
    return index >= 0 && index < nodes_.size() && nodes_[index].is_active;
}

// accessors ====================================================================================================================//

int SearchTree::get_node_key(int node_index) const {
    if (!is_node_active(node_index)) {
        return 0;
    }
    return nodes_[node_index].key_;
}

int SearchTree::height(int node_index) const {
    if (!is_node_active(node_index)) {
        return 0;
    }
    return nodes_[node_index].height_;
}

int SearchTree::subtree_size(int node_index) const {
    if (!is_node_active(node_index)) {
        return 0;
    }
    return nodes_[node_index].subtree_size_;
}

// обновление состояния узла ====================================================================================================//

void SearchTree::upd_height(int node_index) {
    if (!is_node_active(node_index)) return;

    int left_height  = height(nodes_[node_index].left_index_);
    int right_height = height(nodes_[node_index].right_index_);

    nodes_[node_index].height_ = std::max(left_height, right_height) + 1;
}

void SearchTree::upd_subtree_size(int node_index) {
    if (!is_node_active(node_index)) return;

    int left_subtree_size  = subtree_size(nodes_[node_index].left_index_);
    int right_subtree_size = subtree_size(nodes_[node_index].right_index_);

    nodes_[node_index].subtree_size_ = left_subtree_size + right_subtree_size + 1;
}

void SearchTree::upd_node_ctx(int node_index) {
    if (!is_node_active(node_index)) return;

    upd_height(node_index);
    upd_subtree_size(node_index);
}

// вспомогательные методы для балансировки ======================================================================================//

int SearchTree::get_balance(int node_index) const {
    if (!is_node_active(node_index)) {
        return 0;
    }
    return height(nodes_[node_index].left_index_) - height(nodes_[node_index].right_index_);
}

void SearchTree::replace_child(int prev_child_index, int new_child_index, int parent_index) {
    if (!is_node_active(new_child_index)) return;
    if (!is_node_active(parent_index))    return;

    if (nodes_[parent_index].left_index_ == prev_child_index) {
        nodes_[parent_index].left_index_ = new_child_index;
    } else if (nodes_[parent_index].right_index_ == prev_child_index) {
        nodes_[parent_index].right_index_ = new_child_index;
    } else {
        throw std::invalid_argument("Trying to replace non-existing child");
    }
}

// балансирование и вставка элемента ============================================================================================//

void SearchTree::insert(int key) {
    insert(real_root(), key);
}

void SearchTree::insert(int node_index, int key) {
    DBG_PRINT("node: %d, key: %d\n", node_index, key);
    NodeNavigator node_navi = get_navigator_by_key(node_index, key);

    if (!is_node_active(node_navi.current_index_)) {
        if (size_ == 0) {                                // дерево пустое
            add_node(node_navi.last_visited_, key);
            return;
        } else {
            throw std::invalid_argument("insert: got navigator to invalid node in non-empty tree");
        }
    } else {
        if (node_navi.get_key() == key) return;          // значит узел с таким ключом уже есть в дереве
        add_node(node_navi.current_index_, key);          // значит нашли место для вставки
    }

    DBG_PRINT("balancing\n");
    // проходим по пройденному пути вверх до корня, балансируя поддеревья на каждом шаге
    while (node_navi.go_parent() && node_navi.current_index_ != sentinel_index_) {
        DBG_PRINT("current node before balancing: %d\n", node_navi.current_index_);
        int new_local_root_index = balance_node(node_navi.current_index_, key);     // балансируем поддерево
        node_navi.set_index(new_local_root_index);                                  // устанавливаем навигатор в вершину сбалансированного поддерева
        DBG_PRINT("current node after balancing:  %d\n", node_navi.current_index_);
    }
}

void SearchTree::add_node(int parent_index, int key) {
    DBG_PRINT("parent: %d, key: %d\n", parent_index, key);
    if (!is_node_active(parent_index)) {
        if (size_ > 0) {
            throw std::invalid_argument("add_node: parent is invalid");
        }
        // значит дерево пустое
        int real_root_index = nodes_.size();
        nodes_.emplace_back(key, real_root_index, sentinel_index_); // создаем real root
        nodes_[sentinel_index_].left_index_ = real_root_index;
        size_++;
        DBG_PRINT("real root created\n");
        return;
    }

    bool should_be_left_child = key < get_node_key(parent_index);
    DBG_PRINT("before:\n");
    DBG_PRINT("left:  %d\n", nodes_[parent_index].left_index_);
    DBG_PRINT("right: %d\n", nodes_[parent_index].right_index_);
    if (should_be_left_child) {
        if (nodes_[parent_index].left_index_ != -1) throw std::invalid_argument("add_node: target child slot in parent is not empty");

        int new_node_index = nodes_.size();
        nodes_.emplace_back(key, new_node_index, parent_index); // добавляем узел в массив узлов
        nodes_[parent_index].left_index_ = new_node_index;      // связываем parent с новым узлом
        size_++;

        DBG_PRINT("after:\n");
        DBG_PRINT("left:  %d\n", nodes_[parent_index].left_index_);
        DBG_PRINT("right: %d\n", nodes_[parent_index].right_index_);
    } else {
        if (nodes_[parent_index].right_index_ != -1) throw std::invalid_argument("add_node: target child slot in parent is not empty");

        int new_node_index = nodes_.size();
        nodes_.emplace_back(key, new_node_index, parent_index); // добавляем узел в массив узлов
        nodes_[parent_index].right_index_ = new_node_index;      // связываем parent с новым узлом
        size_++;

        DBG_PRINT("after:\n");
        DBG_PRINT("left:  %d\n", nodes_[parent_index].left_index_);
        DBG_PRINT("right: %d\n", nodes_[parent_index].right_index_);
    }
    upd_node_ctx(parent_index);
    //
    // int& parent_child_ref = should_be_left_child ? nodes_[parent_index].left_index_ : nodes_[parent_index].right_index_;
    // DBG_PRINT("left:  %d\n", nodes_[parent_index].left_index_);
    // DBG_PRINT("right: %d\n", nodes_[parent_index].right_index_);
    // DBG_PRINT("child: %d\n", parent_child_ref);
    // if (parent_child_ref != -1) {
    //     throw std::invalid_argument("add_node: target child slot in parent is not empty");
    // }
    // int new_node_index = nodes_.size();
    // nodes_.emplace_back(key, new_node_index, parent_index); // добавляем узел в массив узлов
    // parent_child_ref = new_node_index;                      // связываем parent с новым узлом
    // DBG_PRINT("left:  %d\n", nodes_[parent_index].left_index_);
    // DBG_PRINT("right: %d\n", nodes_[parent_index].right_index_);
    // DBG_PRINT("child: %d\n", parent_child_ref);
    // size_++;                                                // обновляем количество активных узлов
}

int SearchTree::balance_node(int node_index, int key) {
    if (node_index == sentinel_index_) {
        throw std::invalid_argument("balance_node: sentinel node violation");
    }

    DBG_PRINT("node: %d, key: %d\n", node_index, key);
    upd_node_ctx(node_index);
    int balance = get_balance(node_index);
    DBG_PRINT("balance: %d\n", balance);
    // Левый левый
    if (balance > 1 && key < get_node_key(nodes_[node_index].left_index_)) {
        DBG_PRINT("LL\n");
        node_index = right_rotate(node_index);
    }
    // Правый правый
    if (balance < -1 && key > get_node_key(nodes_[node_index].right_index_)) {
        DBG_PRINT("RR\n");
        node_index = left_rotate(node_index);
    }
    // Левый правый
    if (balance > 1 && key > get_node_key(nodes_[node_index].left_index_)) {
        DBG_PRINT("LR\n");
        left_rotate(nodes_[node_index].left_index_);
        node_index = right_rotate(node_index);
    }
    // Правый левый
    if (balance < -1 && key < get_node_key(nodes_[node_index].right_index_)) {
        DBG_PRINT("RL\n");
        right_rotate(nodes_[node_index].right_index_);
        node_index = left_rotate(node_index);
    }
    return node_index;
}

int SearchTree::right_rotate(int B) {
    DBG_PRINT("node: %d\n", B);
    if (!is_node_active(B)) {
        throw std::invalid_argument("right_rotate: local root is inactive");
        // return -1;
    }
    int A = nodes_[B].left_index_;
    if (!is_node_active(A)) {
        throw std::invalid_argument("right_rotate: left child of local root is inactive");
        // return -1;
    }
    int C = nodes_[A].right_index_;

    nodes_[A].right_index_  = B;                            // A.right  = B
    nodes_[A].parent_index_ = nodes_[B].parent_index_;      // A.parent = B.parent

    nodes_[B].left_index_   = C;                            // B.left   = C
    nodes_[B].parent_index_ = A;                            // B.parent = A

    if (is_node_active(C)) {
        nodes_[C].parent_index_   = B;                      // C.parent = B
    }

    replace_child(B, A, nodes_[A].parent_index_);           // подвязать A к старому родителю B
    upd_node_ctx(B);
    upd_node_ctx(A);

    return A;
}

int SearchTree::left_rotate(int A) {
    DBG_PRINT("node: %d\n", A);

    if (!is_node_active(A)) {
        throw std::invalid_argument("left_rotate: local root is inactive");
        //return -1;
    }
    int B = nodes_[A].right_index_;
    if (!is_node_active(B)) {
        throw std::invalid_argument("right_rotate: right child of local root is inactive");
        // return -1;
    }
    int C = nodes_[B].left_index_;

    nodes_[B].left_index_   = A;                         // B.left   = A
    nodes_[B].parent_index_ = nodes_[A].parent_index_;   // B.parent = A.parent

    nodes_[A].right_index_  = C;                         // A.right  = C
    nodes_[A].parent_index_ = B;                         // A.parent = B

    if (is_node_active(C)) {
        nodes_[C].parent_index_ = A;                     // C.parent = A
    }

    replace_child(A, B, nodes_[B].parent_index_);        // подвязать B к старому родителю A
    upd_node_ctx(A);
    upd_node_ctx(B);

    return B;
}

// методы для нахождения количества ключей на отрезке ===========================================================================//

int SearchTree::node_rank(int node_index, int x) const {
    if (!is_node_active(node_index)) { return 0; }

    int curr_key = get_node_key(node_index);
    if (curr_key > x) {
        // только в левом поддереве могут найтись искомые узлы
        return node_rank(nodes_[node_index].left_index_, x);
    } else {
        // значит текущий узел и все в его левом поддереве подходят
        int matching = 1;
        if (is_node_active(nodes_[node_index].left_index_)) {
            matching += subtree_size(nodes_[node_index].left_index_);
        }
        return node_rank(nodes_[node_index].right_index_, x) + matching;
    }
}

int SearchTree::rank(int x) const {
    return node_rank(real_root(), x);
}

int SearchTree::count_in_range(int a, int b) const {
    if (a > b) {return 0; }
    return rank(b) - rank(a - 1);
}

// для отладки
void SearchTree::writeDot(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        // Лучше выбросить исключение или обработать ошибку по-другому
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << "digraph G {\n";
    file << "  rankdir=TB;\n";
    file << "  ordering=out;\n";
    file << "  node [shape=record];\n";

    int root_index = real_root();
    if (size_ > 0 && is_node_active(root_index)) {
        // Используем очередь для обхода в ширину (BFS)
        std::queue<int> q;
        q.push(root_index);

        while (!q.empty()) {
            int current_index = q.front();
            q.pop();

            if (!is_node_active(current_index)) {
                // Если узел стал неактивным между моментом добавления в очередь и обработкой,
                // это может указывать на проблему в логике дерева или на изменение структуры
                // во время выполнения этой функции (например, если она вызывается из другого потока).
                // Для отладки можно бросить исключение.
                throw std::invalid_argument("writeDot: encountered a non-active node in queue.");
                // Или просто continue; чтобы пропустить и продолжить.
                // continue;
            }

            const Node& current = nodes_[current_index];

            // Используем индекс как уникальный идентификатор узла в DOT
            std::string nodeId = "node_" + std::to_string(current_index);

            // Формируем метку узла
            file << "  " << nodeId << " [label=\"{k: " << current.key_
                 << "|h: " << current.height_ << "|sz: " << current.subtree_size_ << "}\"];\n";

            // Обработка левого потомка
            if (current.left_index_ != -1 && is_node_active(current.left_index_)) {
                std::string childId = "node_" + std::to_string(current.left_index_);
                file << "  " << nodeId << " -> " << childId << ";\n";
                q.push(current.left_index_);
            }

            // Обработка правого потомка
            if (current.right_index_ != -1 && is_node_active(current.right_index_)) {
                std::string childId = "node_" + std::to_string(current.right_index_);
                file << "  " << nodeId << " -> " << childId << ";\n";
                q.push(current.right_index_);
            }
        }
    }

    file << "}\n";
    // Деструктор ofstream автоматически закроет файл
}


void SearchTree::print_tree_structure(std::ostream& os, int node_index) const {
    if (node_index == -1 || !is_node_active(node_index)) {
        os << "()"; // Пустое поддерево
        return;
    }

    const Node& current = nodes_[node_index];

    os << "(" << current.key_ << " "; // Открывающая скобка и ключ текущего узла

    // Рекурсивный вызов для левого поддерева
    print_tree_structure(os, current.left_index_);

    os << " "; // Разделитель между левым и правым поддеревом

    // Рекурсивный вызов для правого поддерева
    print_tree_structure(os, current.right_index_);

    os << ")"; // Закрывающая скобка
}

// Публичный метод, который начинает обход от корня
void SearchTree::print_tree_structure(std::ostream& os) const {
    print_tree_structure(os, real_root());
    os << std::endl; // Добавим перевод строки в конце для красоты
}

// NAVIGATOR ====================================================================================================================//

bool SearchTree::NodeNavigator::is_current_index_valid() const {
    return  tree_ &&
            current_index_ >= 0 &&
            current_index_ < tree_->nodes_.size() &&
            tree_->nodes_[current_index_].is_active;
}

bool SearchTree::NodeNavigator::has_left() const {
    return  is_current_index_valid() && tree_->is_node_active(tree_->nodes_[current_index_].left_index_);
}

bool SearchTree::NodeNavigator::has_right() const {
    return  is_current_index_valid() && tree_->is_node_active(tree_->nodes_[current_index_].right_index_);
}

bool SearchTree::NodeNavigator::has_parent() const {
    return  is_current_index_valid() && tree_->is_node_active(tree_->nodes_[current_index_].parent_index_);
}

bool SearchTree::NodeNavigator::go_left() {
    if (!has_left()) return false;
    last_visited_ = current_index_;
    current_index_ = tree_->nodes_[current_index_].left_index_;
    return true;
}

bool SearchTree::NodeNavigator::go_right() {
    if (!has_right()) return false;
    last_visited_ = current_index_;
    current_index_ = tree_->nodes_[current_index_].right_index_;
    return true;
}

bool SearchTree::NodeNavigator::go_parent() {
    int parent_index = get_parent();
    if (parent_index == -1) return false;
    last_visited_ = current_index_;
    current_index_ = parent_index;
    return true;
}

bool SearchTree::NodeNavigator::go_root() {
    if (!tree_ || !tree_->is_node_active(tree_->real_root())) return false;
    last_visited_ = current_index_;
    current_index_ = tree_->real_root();
    return true;
}

bool SearchTree::NodeNavigator::set_index(int new_index) {
    if (!tree_ || !tree_->is_node_active(new_index)) return false;
    last_visited_ = current_index_;
    current_index_ = new_index;
    return true;
}


bool SearchTree::NodeNavigator::is_root() const {
    return is_current_index_valid() && current_index_ == tree_->real_root();
}

int SearchTree::NodeNavigator::get_parent() const {
    if (!has_parent()) return  -1;
    return tree_->nodes_[current_index_].parent_index_;
}

int SearchTree::NodeNavigator::get_key() const {
    if (!is_current_index_valid()) {
        throw std::out_of_range("get_key: Navigator points to invalid zone");
    }
    return tree_->nodes_[current_index_].key_;
}
//
// int SearchTree::NodeNavigator::get_index() const {
//     return current_index_;
// }

int SearchTree::NodeNavigator::get_height() const {
    if (!is_current_index_valid()) {
        throw std::out_of_range("get_height: Navigator points to invalid zone");
    }
    return tree_->nodes_[current_index_].height_;
}

int SearchTree::NodeNavigator::get_subtree_size() const {
    if (!is_current_index_valid()) {
        throw std::out_of_range("get_subtree_size: Navigator points to invalid zone");
    }
    return tree_->nodes_[current_index_].subtree_size_;
}
//
// bool NodeNavigator::operator==(const NodeNavigator& other) const {
//     return tree_ == other.tree_ && current_index_ == other.current_index_;
// }
//
// bool NodeNavigator::operator!=(const NodeNavigator& other) const {
//     return !(*this == other);
// }

SearchTree::NodeNavigator SearchTree::get_root_navigator() const {
    return NodeNavigator(this, real_root());
}

SearchTree::NodeNavigator SearchTree::get_navigator_by_index(int node_index) const {
    if (!is_node_active(node_index)) {
        std::invalid_argument("Trying to create navigator from dead node");
    }
    return NodeNavigator(this, node_index);
}

SearchTree::NodeNavigator SearchTree::get_navigator_by_key(int node_index, int key) const {
    DBG_PRINT("node: %d, key: %d\n", node_index, key);
    NodeNavigator node_navi = get_navigator_by_index(node_index);
    // либо найдем место для вставки, либо узел с данным ключом
    while(node_navi.is_current_index_valid()) {
        // DBG_PRINT(".\n");
        int current_key = node_navi.get_key();
        if (key == current_key) {
            break;
        } else if (key < current_key) {
            if (!node_navi.go_left()) break;
        } else {
            if (!node_navi.go_right()) break;
        }
    }

    return node_navi;
}

}
