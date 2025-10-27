#include "os_tree.hpp"
#include "dothtml.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <cstdlib>

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
