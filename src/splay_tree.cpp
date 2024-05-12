#include "splay_tree/splay_tree.h"
#include "preorder_traversal.h"

#define grandp(a) a->parent->parent

using namespace splay_;

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
    // todo: verify?
    if ( side == sides::left ) x->left = y;
    else x->right = y;
    y->parent = x;

    return status::OK;
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::rotate(tag_t tag, sides direction) {
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
    else root = x; // todo: verify

    tie_node(x, root_node, same_side(direction));
    tie_node(root_node, sub_node, opp_side(direction));

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
    node *picked_node = nodes[tag];
    while (root != picked_node) {
        node *parent = picked_node->parent;
        if (!parent) return status::OK;

        sides parent_side = get_node_side(picked_node);

        node *grparent = nodes[tag]->parent->parent;

        if (!grparent)
            if (parent_side == sides::left)
                right_rotate(parent->tag);
            else
                left_rotate(parent->tag);

        sides grparent_side = get_node_side(parent);

        if (grparent_side = sides::left && parent_side == sides::right)
            zag_zig(tag);
        if (grparent_side = sides::right && parent_side == sides::left)
            zag_zig(tag);
        if (grparent_side = sides::right && parent_side == sides::right)
            zag_zag(tag);
        if (grparent_side = sides::left && parent_side == sides::left)
            zig_zig(tag);
    }
    return status::OK;
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::simple_tree_insert(node *tie_to, node *tie) {
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
tag_t splay_tree<tag_t, value_t>::find_closest_tag(tag_t req_tag, node *picked_node) {
    int cmp_result = cmp(picked_node->tag, req_tag);
    if (!cmp_result) return picked_node->tag;

    if (cmp_result > 0) {
        if (!picked_node->left) return picked_node->tag;

        int child_cmp_result = cmp(picked_node->left->tag, req_tag);
        if (child_cmp_result > 0)
            return find_closest_in_tag(req_tag, picked_node->left);
        else
            return picked_node->tag;
    }
    else {
        if (!picked_node->right) return picked_node->tag;

        int child_cmp_result = cmp(picked_node->right->tag, req_tag);
        if (child_cmp_result < 0)
            return find_closest_in_tag(req_tag, picked_node->right);
        else return picked_node->tag;
    }
}

template<typename tag_t, typename value_t>
status splay_tree<tag_t, value_t>::insert(tag_t new_tag, value_t &new_data) {
    node *new_node = new node(new_tag);
    data[new_tag] = new_data;
    nodes[new_tag] = new_node;

    if (root == nullptr) {
        root = new_node;
        return status::OK;
    }
    if (root->tag == new_tag) return status::OK;

    tag_t closest_tag = find_closest_tag(new_tag, root);
    if (closest_tag != new_tag)
        simple_tree_insert(nodes[closest_tag], nodes[new_tag]);

    splay(new_tag);

}
