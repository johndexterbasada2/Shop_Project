// Lab 2 - Doubly Linked List: shopping cart
#pragma once
#include "models.h"

void addToCart(Product* p);       // reserves 1 unit of stock and records the action for undo
void appendCartNode(Product* p);  // adds a node at the tail (used by add and by redo)
bool removeLastCartNode();        // removes the tail node (used by undo)
void displayCart();
void browseCart();                // walk the cart forward/backward using the prev/next links
void destroyCart();               // frees every cart node
