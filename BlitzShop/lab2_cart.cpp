// Lab 2 - Doubly Linked List: shopping cart
#include "lab2_cart.h"
#include "globals.h"
#include "lab4_undo_stack.h"
#include "console_color.h"
#include "terminal.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace cart {

void appendCartNode(Product* p) {
    CartNode* newNode = new CartNode{p, nullptr, nullptr};
    if (globals::cartHead == nullptr) {
        globals::cartHead = globals::cartTail = newNode;
    } else {
        globals::cartTail->next = newNode;
        newNode->prev = globals::cartTail;
        globals::cartTail = newNode;
    }
}

bool removeLastCartNode() {
    if (!globals::cartTail) return false;
    CartNode* temp = globals::cartTail;
    if (globals::cartTail->prev) {
        globals::cartTail = globals::cartTail->prev;
        globals::cartTail->next = nullptr;
    } else {
        globals::cartHead = globals::cartTail = nullptr;
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
    history::pushUndo(ACTION_CART_ADD, p, 1);
    history::clearRedo();                        // a new action ends the redo history
    cout << "Added to cart: ";
    color::setBrandColor(p->brand);
    cout << p->name;
    color::resetColor();
    cout << "\n";
}

void displayCart() {
    if (!globals::cartHead) {
        cout << "Shopping cart is empty.\n";
        return;
    }
    cout << "\n--- Shopping Cart ---\n";
    CartNode* curr = globals::cartHead;
    double total = 0;
    while (curr) {
        cout << "- ";
        color::setBrandColor(curr->product->brand);
        cout << curr->product->name;
        color::resetColor();
        cout << " | PHP " << fixed << setprecision(2) << curr->product->price << "\n";
        total += curr->product->price;
        curr = curr->next;
    }
    cout << "Total: PHP " << fixed << setprecision(2) << total << "\n";
}

// Doubly linked list navigation: next moves along ->next, previous moves along ->prev
// One key press per move (no Enter). The screen is redrawn for every item.
void browseCart() {
    if (!globals::cartHead) {
        term::newScreen(70);
        cout << "Shopping cart is empty.\n";
        term::pause();
        return;
    }
    int total = 0;
    for (CartNode* c = globals::cartHead; c; c = c->next) total++;

    CartNode* curr = globals::cartHead;
    int pos = 1;
    string note;
    while (true) {
        term::newScreen(70);
        cout << term::centered("--- Browse Cart ---") << "\n\n";
        cout << "Item " << pos << " of " << total << ": ";
        color::setBrandColor(curr->product->brand);
        cout << curr->product->name;
        color::resetColor();
        cout << " (" << curr->product->brand << ") | PHP "
             << fixed << setprecision(2) << curr->product->price << "\n";
        if (!note.empty()) cout << "\n" << note << "\n";
        note.clear();

        char cmd = term::menuKey("\n[N]ext  [P]revious  [Q]uit: ", "npq");
        if (cmd == 'q') return;
        if (cmd == 'n') {
            if (curr->next) { curr = curr->next; pos++; }
            else note = "You are at the last item.";
        } else {
            if (curr->prev) { curr = curr->prev; pos--; }
            else note = "You are at the first item.";
        }
    }
}

void destroyCart() {
    while (globals::cartHead) {
        CartNode* next = globals::cartHead->next;
        delete globals::cartHead;
        globals::cartHead = next;
    }
    globals::cartTail = nullptr;
}

}  // namespace cart
