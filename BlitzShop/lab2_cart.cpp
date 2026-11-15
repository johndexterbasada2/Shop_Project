// Lab 2 - Doubly Linked List: shopping cart
#include "lab2_cart.h"
#include "globals.h"
#include "lab4_undo_stack.h"
#include "console_color.h"
#include "input_utils.h"
#include <iostream>
#include <iomanip>

using namespace std;

void appendCartNode(Product* p) {
    CartNode* newNode = new CartNode{p, nullptr, nullptr};
    if (cartHead == nullptr) {
        cartHead = cartTail = newNode;
    } else {
        cartTail->next = newNode;
        newNode->prev = cartTail;
        cartTail = newNode;
    }
}

bool removeLastCartNode() {
    if (!cartTail) return false;
    CartNode* temp = cartTail;
    if (cartTail->prev) {
        cartTail = cartTail->prev;
        cartTail->next = nullptr;
    } else {
        cartHead = cartTail = nullptr;
    }
    delete temp;
    return true;
}

void addToCart(Product* p) {
    if (p->stockQuantity <= 0) {
        cout << "Out of stock.\n";
        return;
    }
    appendCartNode(p);
    p->stockQuantity--;                 // reserve one unit: the catalog table shows it right away
    pushUndo(ACTION_CART_ADD, p, 1);
    clearRedo();                        // a new action ends the redo history
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
        cout << " | PHP " << fixed << setprecision(2) << curr->product->price << "\n";
        total += curr->product->price;
        curr = curr->next;
    }
    cout << "Total: PHP " << fixed << setprecision(2) << total << "\n";
}

// Doubly linked list navigation: next moves along ->next, previous moves along ->prev
void browseCart() {
    if (!cartHead) {
        cout << "Shopping cart is empty.\n";
        return;
    }
    int total = 0;
    for (CartNode* c = cartHead; c; c = c->next) total++;

    CartNode* curr = cartHead;
    int pos = 1;
    while (true) {
        cout << "\nItem " << pos << " of " << total << ": ";
        setBrandColor(curr->product->brand);
        cout << curr->product->name;
        resetColor();
        cout << " (" << curr->product->brand << ") | PHP "
             << fixed << setprecision(2) << curr->product->price << "\n";

        string cmd;
        while (true) {
            cmd = readText("[N]ext  [P]revious  [Q]uit: ");
            for (char& ch : cmd) ch = (char)tolower((unsigned char)ch);
            if (cmd == "n" || cmd == "p" || cmd == "q") break;
            cout << "Invalid input. Type N, P, or Q. Try again.\n";
        }

        if (cmd == "q") return;
        if (cmd == "n") {
            if (curr->next) { curr = curr->next; pos++; }
            else cout << "You are at the last item.\n";
        } else {
            if (curr->prev) { curr = curr->prev; pos--; }
            else cout << "You are at the first item.\n";
        }
    }
}

void destroyCart() {
    while (cartHead) {
        CartNode* next = cartHead->next;
        delete cartHead;
        cartHead = next;
    }
    cartTail = nullptr;
}
