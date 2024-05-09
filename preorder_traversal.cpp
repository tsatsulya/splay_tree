#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>
#include <cmath>
// node_tags should be given in preorder traversal format
// example:
//     1 2 3 . . 4 . . 5 . 6 . .

void vector_output(std::vector<int> &vect, size_t vector_size) {
    std::cout << "\n";
    for (size_t i = 0; i < vector_size; ++i) {
        std::cout << vect[i] << '\t';
    }
    std::cout << "\n";
}

template <typename tag_t>
struct node {
    tag_t tag;
    node *left;
    node *right;
    node() : left(nullptr), right(nullptr) {}
    node(tag_t tag_) : tag(tag_), left(nullptr), right(nullptr) {}
    node(tag_t tag_, node *left, node *right) : tag(tag_), left(left), right(right) {}
};

enum sides { left = 'l', right = 'r' };

template<typename tag_t>
class tree_reformer {
public:

    typedef std::vector<tag_t> tag_set_t;
    typedef std::unordered_map<tag_t, node<tag_t>> nodes_set_t;


    tag_set_t preorder;
    int (*user_cmp)(tag_t, tag_t);
    nodes_set_t nodes;
    tag_t leaf;
    tag_t left_leaf, right_leaf;

    struct tree {
        tag_t root;
        nodes_set_t nodes;
    };

    tree result_tree;

    tree_reformer(tag_set_t &preorder_, int (*user_cmp_)(tag_t, tag_t)):
        preorder(preorder_), user_cmp(user_cmp_) {
            for (auto& tag : preorder_) nodes[tag] = node<tag_t>(tag);
            // pls verify
        }

    int safe_cmp(tag_t a, tag_t b) {
        int result = user_cmp(a, b);
        assert(result != 0); // user cmp function returned 0. please check documentation
        return result;
    }

    tag_set_t derive_to_side(tag_set_t &preorder, sides side, tag_t tag) {
        tag_set_t preorder_sub = {};
        size_t preorder_size = preorder.size();
        tag_t side_node;

        for (size_t i = 1; i < preorder_size; ++i) {
            side_node = preorder[i];
            if (side == sides::left && safe_cmp(side_node, tag) < 0) preorder_sub.push_back(side_node);
            if (side == sides::right && safe_cmp(side_node, tag) > 0) preorder_sub.push_back(side_node);
        }
        return preorder_sub;
    }
    tag_set_t derive_left_side(tag_set_t &preorder_sub, tag_t tag) {
        return derive_to_side(preorder_sub, sides::left, tag);
    }
    tag_set_t derive_right_side(tag_set_t &preorder_sub, tag_t tag) {
        return derive_to_side(preorder_sub, sides::right, tag);
    }

    void set_side_nodes(tag_set_t preorder_sub, tag_t tag, int *depth) {
        if (preorder_sub.size() == 1) {
            leaf = preorder_sub[0];

            if (tag == leaf) return;
            if (safe_cmp(tag, leaf) > 0) {
                nodes[tag].left = &nodes[leaf];
            }
            if (safe_cmp(tag, leaf) < 0) {
                nodes[tag].right = &nodes[leaf];
            }
            return;
        }
        tag_set_t left_side = derive_left_side(preorder_sub, tag);

        if (left_side.size() > 0) {
            tag_t left_leaf = left_side[0];
            nodes[tag].left = &(nodes[left_leaf]);
            set_side_nodes(left_side, left_leaf, depth);
        }
        tag_set_t right_side = derive_right_side(preorder_sub, tag);

        if (right_side.size() > 0) {
            tag_t right_leaf = right_side[0];
            nodes[tag].right = &(nodes[right_leaf]);
            set_side_nodes(right_side, right_leaf, depth);
        }
    }

    tree reform_tree() {
        int depth;
        set_side_nodes(preorder, preorder[0], &depth);

        result_tree.root = preorder[0];
        result_tree.nodes = nodes;

        return result_tree;
    }
};


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


// std::string empt = " ", undln = "_", obliqln = "/", bobliqln = "\\";

// void console_draw_print(std::unordered_map<int, node<int>> nodes, int root, int depth) {
//     int half_output_width = std::pow(2, depth);
//     std::string stick  = (undln)*(half_output_width-1);
//     std::cout << " " << stick << root << stick << " \n";
//     std::cout << obliqln << empt * (2 * half_output_width - 1) << bobliqln;
// }

int main() {
    std::vector<int> i = {8, 3, 1, 6, 4, 7, 10, 14, 13};
    tree_reformer<int> tree_(i, &cmp);
    tree_.reform_tree();
    int depth = calculate_depth(tree_.nodes, 8);
    return 0;
}