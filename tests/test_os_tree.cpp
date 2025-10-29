#include "../src/os_tree.hpp"
#include "../src/dothtml.hpp"

#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <iostream>

// это чтобы посмотреть корректность построения дерева
void check_structure_with_dump() {
    OS_Tree::SearchTree tree;
    std::vector<std::string> svgFiles;
    int step = 0;

    std::vector<int> values = {10, 5, 1, 3, 7, 2, 18, 9, 12, 14};
    for (int val : values) {
        tree.insert(val);

        std::string dotFile = "../../data/dump/step_" + std::to_string(step) + ".dot";
        std::string svgFile = "../../data/dump/step_" + std::to_string(step) + ".svg";

        tree.writeDot(dotFile);
        generateSVG(dotFile, svgFile);

        svgFiles.push_back("step_" + std::to_string(step) + ".svg");

        step++;
    }

    generateHTML(svgFiles, "../../data/dump/avl_tree_visualization.html");

    std::cout << "HTML файл с визуализацией создан: data/dump/avl_tree_visualization.html\n";
}


void check_balancing_with_dump() {
    OS_Tree::SearchTree tree;
    std::vector<std::string> svgFiles;
    int step = 0;

    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 9, 10};
    for (int val : values) {
        tree.insert(val);

        std::string dotFile = "../../data/dump/bal_" + std::to_string(step) + ".dot";
        std::string svgFile = "../../data/dump/bal_" + std::to_string(step) + ".svg";

        tree.writeDot(dotFile);
        generateSVG(dotFile, svgFile);

        svgFiles.push_back("bal_" + std::to_string(step) + ".svg");

        step++;
    }

    generateHTML(svgFiles, "../../data/dump/balancing.html");

    std::cout << "HTML файл с визуализацией создан: data/dump/balancing.html\n";
}


TEST(OS_TreeTest, basic_test) {
    OS_Tree::SearchTree tree;

    tree.insert(10);
    tree.insert(20);

    EXPECT_EQ(tree.count_in_range(8, 31), 2);
    EXPECT_EQ(tree.count_in_range(6, 9), 0);

    tree.insert(30);
    tree.insert(40);

    EXPECT_EQ(tree.count_in_range(15, 40), 3);
}

TEST(OS_TreeTest, empty_tree) {
    OS_Tree::SearchTree tree;
    EXPECT_EQ(tree.count_in_range(0, 10), 0);
}


TEST(OS_TreeTest, rank_test) {
    OS_Tree::SearchTree tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    EXPECT_EQ(tree.rank(10), 2);    // 5 < 10
    EXPECT_EQ(tree.rank(16), 3);    // 5, 10, 15 < 16
}

int main(int argc, char* argv[]) {
    check_structure_with_dump();
    check_balancing_with_dump();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
