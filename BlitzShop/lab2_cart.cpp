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

static CartNode* findLine(Product* p) {
    for (CartNode* c = globals::cartHead; c; c = c->next)
        if (c->product == p) return c;
    return nullptr;
}

static void unlinkLine(CartNode* line) {
    if (line->prev) line->prev->next = line->next;
    else globals::cartHead = line->next;
    if (line->next) line->next->prev = line->prev;
    else globals::cartTail = line->prev;
    delete line;
}

void putUnits(Product* p, int qty) {
    CartNode* line = findLine(p);
    if (line) {
        line->quantity += qty;
        return;
    }
    CartNode* newNode = new CartNode{p, qty, globals::cartTail, nullptr};
    if (globals::cartHead == nullptr) {
        globals::cartHead = globals::cartTail = newNode;
    } else {
        globals::cartTail->next = newNode;
        globals::cartTail = newNode;
    }
}

bool takeUnits(Product* p, int qty) {
    CartNode* line = findLine(p);
    if (!line || line->quantity < qty) return false;
    line->quantity -= qty;
    if (line->quantity == 0) unlinkLine(line);
    return true;
}

void addToCart(Product* p, int qty) {
    if (qty < 1) return;
    if (p->stockQuantity < qty) {
        cout << "Only " << p->stockQuantity << " in stock.\n";
        return;
    }
    putUnits(p, qty);
    p->stockQuantity -= qty;            // reserve the units: the catalog table shows it right away
    history::pushUndo(ACTION_CART_ADD, p, qty);
    history::clearRedo();               // a new action ends the redo history
    cout << "Added to cart: " << qty << " x ";
    color::setBrandColor(p->brand);
    cout << p->name;
    color::resetColor();
    cout << "\n";
}

void removeFromCart(CartNode* line, int qty) {
    Product* p = line->product;
    if (qty < 1 || qty > line->quantity) return;
    takeUnits(p, qty);                  // the line may be deleted here, so do not use it afterwards
    p->stockQuantity += qty;
    history::pushUndo(ACTION_CART_REMOVE, p, qty);
    history::clearRedo();
    cout << "Removed from cart: " << qty << " x ";
    color::setBrandColor(p->brand);
    cout << p->name;
    color::resetColor();
    cout << ". Stock is now " << p->stockQuantity << ".\n";
}

int lineCount() {
    int n = 0;
    for (CartNode* c = globals::cartHead; c; c = c->next) n++;
    return n;
}

CartNode* lineAt(int number) {
    CartNode* c = globals::cartHead;
    for (int i = 1; c && i < number; i++) c = c->next;
    return (number >= 1) ? c : nullptr;
}

void displayCart(bool numbered) {
    if (!globals::cartHead) {
        cout << "Shopping cart is empty.\n";
        return;
    }
    cout << "--- Shopping Cart ---\n";
    CartNode* curr = globals::cartHead;
    double total = 0;
    int number = 1;
    while (curr) {
        double subtotal = curr->product->price * curr->quantity;
        if (numbered) cout << number << ". ";
        else cout << "- ";
        color::setBrandColor(curr->product->brand);
        cout << curr->product->name;
        color::resetColor();
        cout << " x" << curr->quantity << " | PHP " << fixed << setprecision(2)
             << curr->product->price << " each | PHP " << subtotal << "\n";
        total += subtotal;
        curr = curr->next;
        number++;
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
    int total = lineCount();

    CartNode* curr = globals::cartHead;
    int pos = 1;
    string note;
    while (true) {
        term::newScreen(80);
        cout << term::centered("--- Browse Cart ---") << "\n\n";
        cout << "Item " << pos << " of " << total << ": ";
        color::setBrandColor(curr->product->brand);
        cout << curr->product->name;
        color::resetColor();
        cout << " (" << curr->product->brand << ") x" << curr->quantity << " | PHP "
             << fixed << setprecision(2) << curr->product->price << " each\n";
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
