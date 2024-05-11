#include "preorder_traversal.h"
#include <iostream>
#include <cmath>
#include "strings/my_strings.h"

using namespace my_strings;

int cmp(int a, int b) {
    // std::cout << "\n( " << a << " > " << b << " ) ? : 1 : -1\n";
    return (a > b) ? 1 : (a < b) ? -1 : 0;
}

int calculate_depth(std::unordered_map<int, node<int>> nodes, int root) {
    int left_depth = 0, right_depth = 0;
    // std::cout << std::endl;
    if (nodes[root].left) {
        // std::cout << "left node of " << root << ": " << nodes[root].left->tag << std::endl;
        left_depth = calculate_depth(nodes, nodes[root].left->tag) + 1;
    }

    if (nodes[root].right) {
        // std::cout << "right node of " << root << ": " << nodes[root].right->tag << std::endl;
        right_depth = calculate_depth(nodes, nodes[root].right->tag) + 1;
    }

    return (left_depth < right_depth) ? right_depth : left_depth;
}


std::string empt = " ", undln = "_", obliqln = "/", bobliqln = "\\";

void console_draw_print(std::unordered_map<int, node<int>> nodes, int root, int depth) {
    int half_output_width = std::pow(2, depth);
    std::string stick  = (undln)*(half_output_width-1);
    std::cout << " " << stick << root << stick << " \n";
    std::cout << obliqln << empt * (2 * half_output_width - 1) << bobliqln;
}

int main() {
    std::vector<int> i = {8, 3, 1, 6, 4, 7, 10, 14, 13};
    tree_reformer<int> tree_(i, &cmp);
    tree_.reform_tree();
    // int depth = calculate_depth(tree_.nodes, 8);
    return 0;
}