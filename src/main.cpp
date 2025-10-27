#include "os_tree.hpp"
#include "dothtml.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>

void test_basic() {
    OS_Tree::SearchTree tree;
    std::vector<std::string> svgFiles;
    int step = 0;

    std::vector<int> values = {10, 5, 1, 3, 7, 2, 18, 9, 12, 14};
    for (int val : values) {
        tree.insert(val);

        std::string dotFile = "./data/step_" + std::to_string(step) + ".dot";
        std::string svgFile = "./data/step_" + std::to_string(step) + ".svg";

        tree.writeDot(dotFile);
        generateSVG(dotFile, svgFile);

        // В HTML указываем только имя файла, если HTML тоже в ./data/
        svgFiles.push_back("step_" + std::to_string(step) + ".svg");

        step++;
    }

    generateHTML(svgFiles, "./data/avl_tree_visualization.html");

    std::cout << "HTML файл с визуализацией создан: ./data/avl_tree_visualization.html\n";
}

int main() {

    OS_Tree::SearchTree tree;
    std::string line;

    std::getline(std::cin, line);
    std::istringstream iss(line);

    std::string cmd;
    int key, a, b;

    while (iss >> cmd) {
        if (cmd == "k") {
            iss >> key;
            tree.insert(key);
        } else if (cmd == "q") {
            iss >> a >> b;
            int count = tree.count_in_range(a, b);
            std::cout << count << std::endl;
        }
    }

    return 0;
}
