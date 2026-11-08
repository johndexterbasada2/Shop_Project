// Lab 6 - Binary Search Tree: product lookup by ID
#pragma once
#include "models.h"

BSTNode* insertBST(BSTNode* root, Product* p);
BSTNode* searchBST(BSTNode* root, int id);

// Builds a BALANCED tree (middle ID becomes the root, recursively), so lookup is O(log n)
// even though IDs are naturally in ascending order.
BSTNode* buildBalancedBST(Product* catalog, int size);

void destroyBST(BSTNode* root);   // frees every node (post-order)
