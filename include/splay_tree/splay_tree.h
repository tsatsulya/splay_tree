#pragma once

#include <unordered_map>
#include <iostream>
#include <assert.h>
#include <preorder_traversal/preorder_traversal.h>

namespace alg {

enum status {OK = 0,
             WARN = 1,
             SAME_NODES = 2,
             INCORRECT_TREES};

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
    splay_tree *join(splay_tree *other);
    status split(tag_t tag, splay_tree *new_left, splay_tree *new_right);

    std::vector<tag_t> get_in_preorder_traversal();


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
    tag_t delete_leaf(tag_t);
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
sides opp_side(sides side) {
    if (side == sides::left) return sides::right;
    else return sides::left;
}

sides same_side(sides side) {
    if (side == sides::left) return sides::left;
    else return sides::right;
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::tie_node(node* x, node* y, sides side) { // x y -> x.side = y
    if (!x ) return status::WARN;
    if ( side == sides::left ) x->left = y;
    else x->right = y;
    if (y) y->parent = x;

    return status::OK;
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::rotate(tag_t tag, sides direction) {
    std::cout << "\nrotate : " << ((direction == sides::left) ? "left, tag = " : "right, tag = ") << tag << std::endl;
    node *root_node = nodes[tag];
    node *parent = root_node->parent;

    node *x = get_sub_node(root_node, opp_side(direction));
    if (!x) return status::WARN;
    node *sub_node = get_sub_node(x, same_side(direction));

    sides parent_side;
    if (parent) {
        parent_side = get_node_side(root_node);
        tie_node(parent, x, parent_side);
    }
    else {
        x->parent = nullptr;
        root = x; // todo: verify
    }
    tie_node(root_node, sub_node, opp_side(direction));
    tie_node(x, root_node, same_side(direction));
    // if (sub_node)
        // tie_node(root_node, sub_node, opp_side(direction));
    if (nodes[tag]->right) std::cout << "\n RIGHT_NODE OF " << tag << "IS " << nodes[tag]->right->tag;
    return status::OK;
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::right_rotate(tag_t tag) {
    return rotate(tag, sides::right);
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::left_rotate(tag_t tag) {
    return rotate(tag, sides::left);
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::splay(tag_t tag) {
    std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA";

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
        if (grparent_side == sides::left && parent_side == sides::right) {
            std::cout << 555;
            zag_zig(tag);

        }
        if (grparent_side == sides::right && parent_side == sides::left) {
            std::cout << 444;
            zag_zig(tag);
        }
        if (grparent_side == sides::right && parent_side == sides::right) {
            std::cout << 777;
            zag_zag(tag);
        }
        if (grparent_side == sides::left && parent_side == sides::left) {
            std::cout << 888;
            zig_zig(tag);
        }
    }
    return status::OK;
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::simple_tree_insert(node *tie_to, node *tie) {
    std::cout << "\nsimple insert: " << tie_to->tag << tie->tag;
    int cmp_result = cmp(tie_to->tag, tie->tag);
    if (!cmp_result) return status::SAME_NODES;

    sides side = (cmp_result > 0) ? sides::left : sides::right;
    node *previous_child = (side == sides::left) ? tie_to->left : tie_to->right;

    if (!previous_child)
        return tie_node(tie_to, tie, side);
    else {
        tie_node(tie_to, tie, side);
        std::cout << 65656566565 << tie_to->right->tag;

        int child_cmp_result = cmp(previous_child->tag, tie->tag);

        if (!child_cmp_result) return status::SAME_NODES;
        else if (child_cmp_result > 0) return tie_node(tie, previous_child, sides::right);
        else return tie_node(tie, previous_child, sides::left);
    }
}

template<typename tag_t, typename value_t>
tag_t splay_tree<tag_t, value_t>::find_closest_tag(tag_t req_tag, node *picked_node) {
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
status splay_tree<tag_t, value_t>::insert(tag_t new_tag, value_t new_data) {
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
tag_t splay_tree<tag_t, value_t>::search_for_extremum(tag_t sub_root,
    desired desire) {
    tag_t child_tag = sub_root;
    node* sub_child = nodes[sub_root];

    while (sub_child != nullptr) {
        child_tag = sub_child->tag;
        if (desire == desired::maximum)
            sub_child = sub_child->right;
        else {
            sub_child = sub_child->left;
        }
        if (!sub_child) return child_tag;
    }
    return child_tag;
}

template<typename tag_t, typename value_t>
tag_t splay_tree<tag_t, value_t>::search_for_maximum(tag_t sub_root) {
    return search_for_extremum(sub_root, desired::maximum);
}

template<typename tag_t, typename value_t>
tag_t splay_tree<tag_t, value_t>::search_for_minimum(tag_t sub_root) {
    return search_for_extremum(sub_root, desired::minimum);
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::top_down_delete(tag_t picked) {
    splay(picked);
    node* left_root = root->left;
    splay_tree *left_tree = new splay_tree(cmp, nodes, left_root->tag);

    tag_t left_max = left_tree->search_for_maximum(root->tag);
    left_tree->splay(left_max);
    left_tree->tie_node(left_tree->root, left_tree->nodes[left_max], sides::right);
    left_tree->nodes.erase(picked);
    nodes = left_tree->nodes;

    delete left_tree;
    return status::OK;
}

template<typename tag_t, typename value_t>
tag_t splay_tree<tag_t, value_t>::delete_leaf(tag_t leaf) {
    std::cout << "DELETE" << leaf << std::endl;
    node *parent = nodes[leaf]->parent;
    if (!parent) {
        // std::cout << 1010010011;
        root = nullptr;
    }
    else {
        sides side = get_node_side(nodes[leaf]);
        if (side == sides::left) parent->left = nullptr;
        else parent->right = nullptr;
    }
    nodes.erase(leaf);
    std::cout << "DELETE" << parent->tag << std::endl;

    return parent->tag;
}

template<typename tag_t, typename value_t>
void splay_tree<tag_t, value_t>::swap_nodes(
    splay_tree<tag_t, value_t>::node *first,
    splay_tree<tag_t, value_t>::node *second) {

    sides side;
    std::cout << "\nswapswap " << first->tag << second->tag;
    if (first->left) first->left->parent = second;
    if (first->right) first->right->parent = second;

    if (second->left) second->left->parent = first;
    if (second->right) second->right->parent = first;

    if (first->parent) {
        side = get_node_side(first);
        if (side == sides::left) {first->parent->left = second;}
        if (side == sides::right) {first->parent->right = second;}
    }

    if (second->parent) {
        side = get_node_side(second);
        if (side == sides::left) {second->parent->left = first;}
        if (side == sides::right) {second->parent->right = first;}
    }


    if (root->tag == first->tag) {root = second;}
    else if (root->tag == second->tag) {root = first;}

    node *tmp = new node(first);
    first->parent = second->parent;
    first->right = second->right;
    first->left = second->left;
    second->parent = tmp->parent;
    second->right = tmp->right;
    second->left = tmp->left;

    // if (first_parent) {((first_side == sides::left) ? first_parent->left : first_parent->right) = second;}

    delete tmp;
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::bottom_up_delete(tag_t picked) {
    node *parent;
    std::cout << "maxmax" << picked << std::endl;
    if (is_leaf(picked)) {
        parent = nodes[delete_leaf(picked)];
    }
    else {
        parent = nodes[picked]->parent;
        if (nodes[picked]->right) {
            // if (!parent)
            tag_t right_minimum = search_for_minimum(nodes[picked]->right->tag);

            swap_nodes(nodes[picked], nodes[right_minimum]);
            delete_leaf(picked);
        }
        else if (nodes[picked]->left) {
            std::cout << 9999999;
            tag_t left_maximum = search_for_maximum(nodes[picked]->left->tag);
            std::cout << "\n" << left_maximum << 7777080808080808;
            swap_nodes(nodes[picked], nodes[left_maximum]);
            std::vector<int> p_t = get_in_preorder_traversal();
            delete_leaf(picked);
        }
    }
    std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAA";

    if (parent) splay(parent->tag);
    // else

    return status::OK;
}

template<typename tag_t, typename value_t>
splay_tree<tag_t, value_t>* splay_tree<tag_t, value_t>::join(splay_tree *other) {
    tag_t max_this = search_for_maximum(root->tag);
    tag_t max_other = search_for_maximum(other->root->tag);
    tag_t min_this = search_for_minimum(root->tag);
    tag_t min_other = search_for_minimum(other->root->tag);
    tag_t new_root;
    std::cout << max_this << min_this << max_other << min_other << std::endl;
    if (cmp(max_this, min_other) < 0) {
        splay(max_this);
        tie_node(root, other->root, sides::right);
        new_root = root->tag;
    }
    else if (cmp(min_this, max_other) > 0) {
        other->splay(max_other);
        other->tie_node(other->root, root, sides::right);
        new_root = other->root->tag;
    }
    else assert(0); // incorrect trees to join

    std::unordered_map<tag_t, node*> new_nodes = {};
    new_nodes.insert(other->nodes.begin(), other->nodes.end());
    new_nodes.insert(nodes.begin(), nodes.end());

    return new splay_tree(cmp, new_nodes, new_root);
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::split(tag_t tag, splay_tree *new_left, splay_tree* new_right) {
    splay(tag);
    std::unordered_map<tag_t, node*> left_nodes = {}, right_nodes = {};

    for (auto& node_ : nodes) {
        if (cmp(tag, node_.first) > 0)
            left_nodes[node_.first] = node_.second;
        if (cmp(tag, node_.first) < 0)
            right_nodes[node_.first] = node_.second;
    }
    *new_left = splay_tree(cmp, left_nodes, nodes[tag]->left->tag);
    *new_right = splay_tree(cmp, right_nodes, nodes[tag]->right->tag);
    return status::OK;
}

template<typename tag_t, typename value_t>
std::vector<tag_t> splay_tree<tag_t, value_t>::get_in_preorder_traversal() {
    graph_format::tree<tag_t> *format_tree = new graph_format::tree<tag_t>;
    std::cout << "6666" << std::endl;
    for (auto& n : nodes)
        format_tree->nodes[n.first] = graph_format::node(n.first);
    for (auto& n : nodes) {
        if (n.second->left)
            format_tree->nodes[n.first].left = &(format_tree->nodes[n.second->left->tag]);
        if (n.second->right)
            format_tree->nodes[n.first].right = &(format_tree->nodes[n.second->right->tag]);
    }
    format_tree->root = root->tag;

    graph_format::preorder_reformer<tag_t> reformer(format_tree);
    std::vector<tag_t> p_t = reformer.tree_to_preorder_traversal(root->tag);
    delete format_tree;
    return p_t;
}
}