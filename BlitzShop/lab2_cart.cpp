// Lab 2 - Doubly Linked List: shopping cart
#include "lab2_cart.h"
#include "globals.h"
#include "lab4_undo_stack.h"
#include "console_color.h"
#include <iostream>

using namespace std;

void addToCart(Product* p) {
    if (p->stockQuantity <= 0) {
        cout << "Out of stock.\n";
        return;
    }
    CartNode* newNode = new CartNode{p, nullptr, nullptr};
    if (cartHead == nullptr) {
        cartHead = cartTail = newNode;
    } else {
        cartTail->next = newNode;
        newNode->prev = cartTail;
        cartTail = newNode;
    }
    pushUndo(p);
    cout << "Added to cart: ";
    setBrandColor(p->brand);
    cout << p->name;
    resetColor();
    cout << "\n";
}

void displayCart() {
    if (!cartHead) {
        cout << "Shopping cart is empty.\n";
        return;
    }
    cout << "\n--- Shopping Cart ---\n";
    CartNode* curr = cartHead;
    double total = 0;
    while (curr) {
        cout << "- ";
        setBrandColor(curr->product->brand);
        cout << curr->product->name;
        resetColor();
        cout << " | $" << curr->product->price << "\n";
        total += curr->product->price;
        curr = curr->next;
    }
    cout << "Total: $" << total << "\n";
}

// Menu option 7 (moved out of main.cpp, logic unchanged)
void undoLastAction() {
    Product* p = popUndo();
    if (p) {
        if (cartTail) {
            CartNode* temp = cartTail;
            if (cartTail->prev) {
                cartTail = cartTail->prev;
                cartTail->next = nullptr;
            } else {
                cartHead = cartTail = nullptr;
            }
            delete temp;
        }
        cout << "Undo successful. Removed " << p->name << " from cart.\n";
    } else {
        cout << "No recent actions to undo.\n";
    }
}
