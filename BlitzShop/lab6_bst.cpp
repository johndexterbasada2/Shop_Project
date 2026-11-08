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

// Builds a balanced tree from IDs that are already sorted in pointers[lo..hi]
static BSTNode* buildFromSorted(Product** pointers, int lo, int hi) {
    if (lo > hi) return nullptr;
    int mid = lo + (hi - lo) / 2;
    BSTNode* node = new BSTNode{pointers[mid], nullptr, nullptr};
    node->left = buildFromSorted(pointers, lo, mid - 1);
    node->right = buildFromSorted(pointers, mid + 1, hi);
    return node;
}

BSTNode* buildBalancedBST(Product* catalog, int size) {
    Product** pointers = new Product*[size];
    for (int i = 0; i < size; i++) pointers[i] = &catalog[i];

    // insertion sort the pointers by ID
    for (int i = 1; i < size; i++) {
        Product* key = pointers[i];
        int j = i - 1;
        while (j >= 0 && pointers[j]->id > key->id) {
            pointers[j + 1] = pointers[j];
            j--;
        }
        pointers[j + 1] = key;
    }

    BSTNode* root = buildFromSorted(pointers, 0, size - 1);
    delete[] pointers;
    return root;
}

void destroyBST(BSTNode* root) {
    if (!root) return;
    destroyBST(root->left);
    destroyBST(root->right);
    delete root;
}
