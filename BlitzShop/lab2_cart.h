// Lab 2 - Doubly Linked List: shopping cart (one node per product, with a quantity)
#pragma once
#include "models.h"

using namespace std;

namespace cart {

void addToCart(Product* p, int qty);                // reserves qty units of stock and records the action for undo
void removeFromCart(CartNode* line, int qty);       // gives qty units back to stock and records the action for undo

// Raw list operations (also used by undo and redo, they do not touch stock or history)
void putUnits(Product* p, int qty);                 // adds to the product's line, or creates a new line at the tail
bool takeUnits(Product* p, int qty);                // removes units, deletes the line when it reaches 0

int lineCount();                                    // number of different products in the cart
CartNode* lineAt(int number);                       // 1-based position, nullptr if out of range
void displayCart(bool numbered = false);
void browseCart();                                  // walk the cart forward/backward using the prev/next links
void destroyCart();                                 // frees every cart node

}  // namespace cart
