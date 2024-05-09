#include <vector>
#include <string>
#include <unordered_map>
#include <cassert>

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

    tree result;

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

        result.root = preorder[0];
        result.nodes = nodes;

        return result;
    }
};