#pragma once

#include <unordered_map>
#include <iostream>

namespace alg {

enum status {OK = 0,
             WARN = 1,
             SAME_NODES = 2};

enum sides { left, right, no };
enum desired { minimum, maximum, nn };


template<typename tag_t, typename value_t>
class splay_tree {
public:

    struct node {
        tag_t tag;
        node* right = nullptr;
        node* left = nullptr;
        node* parent = nullptr;
        node(tag_t tag_) {
            this->tag = tag_;
        }
        node(node *another) {
            this->tag = another->tag;
            this->right = another->right;
            this->left = another->left;
            this->parent = another->parent;
        }
    };

    splay_tree(int (*user_cmp_)(tag_t, tag_t)){
        cmp = user_cmp_;
        nodes = {}; data = {};
        root = nullptr;
    };

    splay_tree(int (*user_cmp_)(tag_t, tag_t),
        std::unordered_map<tag_t, node*> &nodes_, tag_t root_){
        cmp = user_cmp_;
        nodes = nodes_;
        data = {};
        root = nodes[root_];
        nodes[root_]->parent = nullptr;
    };

    value_t search(tag_t);
    status insert(tag_t, value_t);
    status top_down_delete(tag_t);
    status bottom_up_delete(tag_t);
    status print();


    node *create_node(tag_t tag) {
        return new node(tag);
    }
private:

    std::unordered_map<tag_t, node*> nodes;
    std::unordered_map<tag_t, value_t*> data;
    int (*cmp)(tag_t, tag_t);
    int depth;
    int get_depth();
    node *root;

    bool is_leaf(tag_t tag) {
        return (!nodes[tag]->left && !nodes[tag]->right);
    }

    tag_t search_for_extremum(tag_t sub_root, desired desire);
    tag_t search_for_maximum(tag_t sub_root);
    tag_t search_for_minimum(tag_t sub_root);

    status rotate(tag_t, sides);
    status right_rotate(tag_t);
    status left_rotate(tag_t);

    status simple_tree_insert(node*, node*);
    node *delete_leaf(tag_t);
    void swap_nodes(node*, node*);

    tag_t find_closest_tag(tag_t, node*);

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


    node *get_sub_node(node* root, sides side) {
        if (side == sides::left) return root->left;
        else return root->right;
    };

    sides get_node_side(node *picked_node) {
        std::cout << "\nget_node_side : " << picked_node->tag << std::endl;
        node *parent = picked_node->parent;

        if (!parent)
            return sides::no;

        if (parent->left && parent->left->tag == picked_node->tag) {
            return sides::left;
            }
        if (parent->right && parent->right->tag == picked_node->tag)
            return sides::right;

        return sides::no;
    }

    status tie_node(node*, node*, sides);

    status splay(tag_t);
};

template class splay_tree<int, int>;
alg::sides opp_side(alg::sides side) {
    if (side == alg::sides::left) return alg::sides::right;
    else return alg::sides::left;
}

alg::sides same_side(alg::sides side) {
    if (side == alg::sides::left) return alg::sides::left;
    else return alg::sides::right;
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::tie_node(node* x, node* y, sides side) { // x y -> x.side = y
    // todo: verify?
    if ( side == sides::left ) x->left = y;
    else x->right = y;
    y->parent = x;

    return status::OK;
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::rotate(tag_t tag, sides direction) {

    node *root_node = nodes[tag];
    node *parent = root_node->parent;

    node *x = get_sub_node(root_node, opp_side(direction));
    if (!x) return status::WARN;
    node *sub_node = get_sub_node(x, same_side(direction));

    sides parent_side;
    if (parent) {
        if (parent->left == root_node) parent_side = sides::left;
        else parent_side = sides::right;
        tie_node(parent, x, parent_side);
    }
    else {
        x->parent = nullptr;
        root = x; // todo: verify
    }
    tie_node(x, root_node, same_side(direction));
    if (sub_node)
        tie_node(root_node, sub_node, opp_side(direction));

    return status::OK;
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::right_rotate(tag_t tag) {
    return rotate(tag, sides::right);
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::left_rotate(tag_t tag) {
    return rotate(tag, sides::left);
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::splay(tag_t tag) {
    node *picked_node = nodes[tag];
    while (root != picked_node) {
        node *parent = picked_node->parent;
        if (!parent) return status::OK;

        sides parent_side = get_node_side(picked_node);

        node *grparent = nodes[tag]->parent->parent;

        if (!grparent) {
            if (parent_side == sides::left) {
                right_rotate(parent->tag);
            }
            else {
                left_rotate(parent->tag);
                std::cout << 9;
            }
            return status::OK;
        }
        sides grparent_side = get_node_side(parent);
        if (grparent_side == sides::left && parent_side == sides::right)
            zag_zig(tag);
        if (grparent_side == sides::right && parent_side == sides::left)
            zag_zig(tag);
        if (grparent_side == sides::right && parent_side == sides::right)
            zag_zag(tag);
        if (grparent_side == sides::left && parent_side == sides::left)
            zig_zig(tag);
    }
    return status::OK;
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::simple_tree_insert(node *tie_to, node *tie) {
    int cmp_result = cmp(tie_to->tag, tie->tag);
    if (!cmp_result) return status::SAME_NODES;

    sides side = (cmp_result > 0) ? sides::left : sides::right;
    node *previous_child = (side == sides::left) ? tie_to->left : tie_to->right;

    if (!previous_child)
        return tie_node(tie_to, tie, side);
    else {
        tie_node(tie_to, tie, side);
        int child_cmp_result = cmp(previous_child->tag, tie->tag);

        if (!child_cmp_result) return status::SAME_NODES;
        else if (child_cmp_result > 0) return tie_node(tie, previous_child, sides::right);
        else return tie_node(tie, previous_child, sides::left);
    }
}

template<typename tag_t, typename value_t>
tag_t alg::splay_tree<tag_t, value_t>::find_closest_tag(tag_t req_tag, node *picked_node) {
    int cmp_result = cmp(picked_node->tag, req_tag);
    if (!cmp_result) return picked_node->tag;

    if (cmp_result > 0) {
        if (!picked_node->left) return picked_node->tag;

        int child_cmp_result = cmp(picked_node->left->tag, req_tag);
        if (child_cmp_result > 0)
            return find_closest_tag(req_tag, picked_node->left);
        else
            return picked_node->tag;
    }
    else {
        if (!picked_node->right) return picked_node->tag;

        int child_cmp_result = cmp(picked_node->right->tag, req_tag);
        if (child_cmp_result < 0)
            return find_closest_tag(req_tag, picked_node->right);
        else return picked_node->tag;
    }
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::insert(tag_t new_tag, value_t new_data) {
    std::cout << "\ninsert: " << new_tag << std::endl;
    node *new_node = new node(new_tag);
    data[new_tag] = &new_data;
    nodes[new_tag] = new_node;

    if (root == nullptr) {
        root = new_node;
        return status::OK;
    }
    if (root->tag == new_tag) return status::OK;

    tag_t closest_tag = find_closest_tag(new_tag, root);
    std::cout << closest_tag;
    if (closest_tag != new_tag)
        simple_tree_insert(nodes[closest_tag], nodes[new_tag]);

    splay(new_tag);
    std::cout << "\nroot: " << root->tag << std::endl;
    return status::OK;
}

template<typename tag_t, typename value_t>
tag_t alg::splay_tree<tag_t, value_t>::search_for_extremum(tag_t sub_root,
    alg::desired desire) {

    node* sub_child; tag_t child_tag;
    while (sub_child != nullptr) {
        sub_child = (desire == alg::desired::maximum) ? sub_child->right : sub_child->left;
        if (sub_child) child_tag = sub_child->tag;
    }
    return child_tag;
}

template<typename tag_t, typename value_t>
tag_t alg::splay_tree<tag_t, value_t>::search_for_maximum(tag_t sub_root) {
    return search_for_extremum(sub_root, alg::desired::maximum);
}

template<typename tag_t, typename value_t>
tag_t alg::splay_tree<tag_t, value_t>::search_for_minimum(tag_t sub_root) {
    return search_for_extremum(sub_root, alg::desired::minimum);
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::top_down_delete(tag_t picked) {
    splay(picked);
    node* left_root = root->left;
    splay_tree *left_tree = new splay_tree(cmp, nodes, left_root->tag);

    tag_t left_max = left_tree->search_for_maximum(root->tag);
    left_tree->splay(left_max);
    left_tree->tie_node(left_tree->root, left_tree->nodes[left_max], alg::sides::right);
    left_tree->nodes.erase(picked);
    nodes = left_tree->nodes;

    delete left_tree;
    return alg::status::OK;
}

template<typename tag_t, typename value_t>
alg::splay_tree<tag_t, value_t>::node *alg::splay_tree<tag_t, value_t>::delete_leaf(tag_t leaf) {
    node *parent = nodes[leaf]->parent;
    if (!parent) {
        root = nullptr;
    }
    else {
        alg::sides side = get_node_side(leaf);
        if (side == alg::sides::left) parent->left = nullptr;
        else parent->right = nullptr;
    }
    nodes.erase(leaf);
    return parent;
}

template<typename tag_t, typename value_t>
void alg::splay_tree<tag_t, value_t>::swap_nodes(
    alg::splay_tree<tag_t, value_t>::node *first,
    alg::splay_tree<tag_t, value_t>::node *second) {

    node *tmp = new node(first);
    first->parent = second->parent;
    first->right = second->right;
    first->left = second->left;
    second->parent = tmp->parent;
    second->right = tmp->right;
    second->left = tmp->left;
    delete tmp;
}

template<typename tag_t, typename value_t>
alg::status alg::splay_tree<tag_t, value_t>::bottom_up_delete(tag_t picked) {
    node *parent;
    if (is_leaf(picked)) {
        parent = delete_leaf(picked);
    }
    else {
        if (nodes[picked]->right) {
            parent = nodes[picked];
            tag_t right_minimum = search_for_minimum(nodes[picked]->right->tag);
            swap_nodes(nodes[picked], nodes[right_minimum]);
            delete_leaf(picked);
        }
    }

    if (parent) splay(parent->tag);

    return alg::status::OK;
}
}