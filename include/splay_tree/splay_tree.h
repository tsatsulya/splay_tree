#pragma once

#include <unordered_map>

namespace splay_ {

enum status {OK = 0,
             WARN = 1};

enum sides { left = 'left', right = 'right' };

template<typename tag_t, typename value_t>
class splay_tree {
    status rotate(tag_t, sides);
    status right_rotate(tag_t);
    status left_rotate(tag_t);

    status zig_zag(tag_t);
    status zag_zig(tag_t);
    status zig(tag_t);

    struct node {
        tag_t tag;
        node* right = nullptr;
        node* left = nullptr;
        node* parent = nullptr;
        node(tag_t tag_) {
            this->tag = tag_;
        }
    };
    node *root;

    node *get_sub_node(node* root, sides side) {
        if (side == sides::left) return root->left;
        else return root->right;
    };

    status tie_node(node*, node*, sides);

    std::unordered_map<tag_t, node*> nodes;
    int depth;

public:
    splay_tree();
    value_t search(tag_t);
    status insert(tag_t, value_t);
    status remove(tag_t);
    status print();
    int get_depth();
};
}