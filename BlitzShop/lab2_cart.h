// Lab 2 - Doubly Linked List: shopping cart
#pragma once
#include "models.h"

void addToCart(Product* p);
void displayCart();
void undoLastAction();   // pops the undo stack and removes the last cart item
