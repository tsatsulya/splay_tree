#pragma once

#include <unordered_map>

namespace splay_ {

enum status {OK = 0,
             WARN = 1};

enum sides { left, right, no };

template<typename tag_t, typename value_t>
class splay_tree {
public:

    splay_tree();
    value_t search(tag_t);
    status insert(tag_t, value_t);
    status remove(tag_t);
    status print();

    struct node {
        tag_t tag;
        node* right = nullptr;
        node* left = nullptr;
        node* parent = nullptr;
        node(tag_t tag_) {
            this->tag = tag_;
        }
    };
    node *create_node(tag_t tag) {
        return new node(tag);
    }
private:

    std::unordered_map<tag_t, node*> nodes;
    int depth;
    int get_depth();
    status rotate(tag_t, sides);
    status right_rotate(tag_t);
    status left_rotate(tag_t);

    status zig_zag(tag_t tag) {
        tag_t parent_tag =  nodes[tag]->parent->tag;
        status st = right_rotate(parent_tag);
        if (st) return st;
        return left_rotate(parent_tag);
    };

    status zag_zig(tag_t tag) {
        tag_t parent_tag =  nodes[tag]->parent->tag;
        status st = left_rotate(parent_tag);
        if (st) return st;
        return right_rotate(parent_tag);
    }

    status zig_zig(tag_t tag) {
        tag_t parent_tag =  nodes[tag]->parent->tag;
        tag_t grparent_tag = nodes[tag]->parent->parent->tag;
        status st = right_rotate(grparent_tag);
        if (st) return st;
        return right_rotate(parent_tag);
    }

    status zag_zag(tag_t tag) {
        tag_t parent_tag =  nodes[tag]->parent->tag;
        tag_t grparent_tag = nodes[tag]->parent->parent->tag;
        status st = left_rotate(grparent_tag);
        if (st) return st;
        return left_rotate(parent_tag);
    };

    node *root;

    node *get_sub_node(node* root, sides side) {
        if (side == sides::left) return root->left;
        else return root->right;
    };

    sides get_node_side(node picked_node) {
        if (picked_node->left && picked_node->left == picked_node)
            return sides::left;
        if (picked_node->right && picked_node->right == picked_node)
            return sides::right;
        return sides::no;
    }

    status tie_node(node*, node*, sides);

    status splay(tag_t);
};
}