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
    // return rotate(tag, sides::left);
}