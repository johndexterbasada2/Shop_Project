// Lab 6 - Binary Search Tree: product lookup by ID
#include "lab6_bst.h"

BSTNode* insertBST(BSTNode* root, Product* p) {
    if (root == nullptr) {
        BSTNode* newNode = new BSTNode{p, nullptr, nullptr};
        return newNode;
    }
    if (p->id < root->product->id) root->left = insertBST(root->left, p);
    else if (p->id > root->product->id) root->right = insertBST(root->right, p);
    return root;
}

BSTNode* searchBST(BSTNode* root, int id) {
    if (root == nullptr || root->product->id == id) return root;
    if (root->product->id < id) return searchBST(root->right, id);
    return searchBST(root->left, id);
}
