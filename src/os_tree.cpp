#include "os_tree.hpp"
#include <vector>
#include <algorithm>

namespace OS_Tree {

int SearchTree::height(std::unique_ptr<Node>& node) {
    if (!node) { return 0; }
    return node->height_;
}

int SearchTree::subtree_size(std::unique_ptr<Node>& node) {
    if (!node) { return 0; }
    return node->subtree_size_;
}

void SearchTree::upd_height(std::unique_ptr<Node>& node) {
    if (!node) { return; }
    int left_height  = 0;
    int right_height = 0;

    if (node->left)  { left_height  = height(node->left);  }
    if (node->right) { right_height = height(node->right); }

    node->height_ = std::max(left_height, right_height) + 1;
}

void SearchTree::upd_subtree_size(std::unique_ptr<Node>& node) {
    if (!node) { return; }
    int left_subtree_size  = 0;
    int right_subtree_size = 0;

    if (node->left)  { left_subtree_size  = subtree_size(node->left);  }
    if (node->right) { right_subtree_size = subtree_size(node->right); }

    node->subtree_size_ = left_subtree_size + right_subtree_size + 1;
}

void SearchTree::upd_node_ctx(std::unique_ptr<Node>& node) {
    if (!node) { return; }
    upd_height(node);
    upd_subtree_size(node);
}

int SearchTree::get_balance(std::unique_ptr<Node>& node) {
    if (!node) { return 0; }
    return height(node->left) - height(node->right);
}

void SearchTree::right_rotate(std::unique_ptr<Node>& B) {
    if (!B || !(B->left)) return;

    auto A = std::move(B->left);
    auto C = std::move(A->right);

    A->right        = std::move(B);
    B               = std::move(A);     // хотим вернуть вершину получившегося поддерева, далее работаем через нее
    B->right->left  = std::move(C);     // eq A->right->left = C

    upd_node_ctx(B->right);
    upd_node_ctx(B);
}

void SearchTree::left_rotate(std::unique_ptr<Node>& A) {
    if (!A || !(A->right)) return;

    auto B = std::move(A->right);
    auto C = std::move(B->left);

    B->left         = std::move(A);
    A               = std::move(B);
    A->left->right  = std::move(C);

    upd_node_ctx(A->left);
    upd_node_ctx(A);
}

void SearchTree::insert(std::unique_ptr<Node>& node, int key) {
    if (!node) {
        node = std::make_unique<Node>(key);
        return;
    }

    if (key < node->key_) {
        insert(node->left, key);
        // upd_node_ctx(node);
    } else if (key > node->key_) {
        insert(node->right, key);
        // upd_node_ctx(node);
    } else {
        return; // дубликаты не допускаются
    }

    upd_node_ctx(node);
    int balance = get_balance(node);

    // Левый левый
    if (balance > 1 && key < node->left->key_) {
        right_rotate(node);
    }
    // Правый правый
    if (balance < -1 && key > node->right->key_) {
        left_rotate(node);
    }
    // Левый правый
    if (balance > 1 && key > node->left->key_) {
        left_rotate(node->left);
        right_rotate(node);
    }
    // Правый левый
    if (balance < -1 && key < node->right->key_) {
        right_rotate(node->right);
        left_rotate(node);
    }
}

void SearchTree::insert(int key) {
    insert(root_, key);
}

void SearchTree::writeDot(const std::string& filename) {
    std::ofstream file(filename);
    file << "digraph G {\n";
    file << "  rankdir=TB;\n";
    file << "  ordering=out;\n";
    file << "  node [shape=record];\n";

    if (root_) {
        std::vector<Node*> queue = {root_.get()};
        while (!queue.empty()) {
            Node* current = queue.back();
            queue.pop_back();

            // Используем ключ как уникальный ID
            std::string nodeId = "node_" + std::to_string(current->key_);

            file << "  " << nodeId << " [label=\"{k: " << current->key_
                 << "|h: " << current->height_ << "|sz: " << current->subtree_size_ << "}\"];\n";

            if (current->left) {
                file << "  " << nodeId << " -> node_" << current->left->key_ << ";\n";
                queue.push_back(current->left.get());
            }
            if (current->right) {
                file << "  " << nodeId << " -> node_" << current->right->key_ << ";\n";
                queue.push_back(current->right.get());
            }
        }
    }

    file << "}\n";
    file.close();
}

int SearchTree::node_rank(std::unique_ptr<Node>&  node, int x) {
    if (!node) { return 0; }

    int curr_key = node->key_;
    if (curr_key > x) {
        // только в левом поддереве могут найтись искомые узлы
        return node_rank(node->left, x);
    } else {
        // значит текущий узел и все в его левом поддереве подходят
        int matching = 1;
        if (node->left) { matching += subtree_size(node->left); }

        return node_rank(node->right, x) + matching;
    }
}

int SearchTree::rank(int x) {
    return node_rank(root_, x);
}

int SearchTree::count_in_range(int a, int b) {
    if (a > b) {return 0; }
    return rank(b) - rank(a - 1);
}

}
