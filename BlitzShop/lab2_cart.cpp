// Lab 2 - Doubly Linked List: shopping cart
#include "lab2_cart.h"
#include "globals.h"
#include "console_color.h"
#include "terminal.h"
#include "display.h"
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

// Every cart item in ONE table. `highlight` (1-based) marks a row with ">" for browsing.
void displayCart(int highlight) {
    if (!globals::cartHead) {
        cout << "Shopping cart is empty.\n";
        return;
    }
    const int W_NO = 5, W_ID = 9, W_NAME = 30, W_BRAND = 20, W_QTY = 6, W_PRICE = 15, W_SUB = 15;
    int width = W_NO + W_ID + W_NAME + W_BRAND + W_QTY + W_PRICE + W_SUB;

    cout << string(width, '=') << "\n" << left
         << setw(W_NO) << "No." << setw(W_ID) << "Prod_ID" << setw(W_NAME) << "Name"
         << setw(W_BRAND) << "Brand" << setw(W_QTY) << "Qty" << setw(W_PRICE) << "Unit Price"
         << "Subtotal\n" << string(width, '=') << "\n";

    double total = 0;
    int number = 1;
    for (CartNode* c = globals::cartHead; c; c = c->next, number++) {
        Product* p = c->product;
        double subtotal = p->price * c->quantity;
        string mark = (number == highlight ? "> " : "") + to_string(number) + ".";
        cout << setw(W_NO) << mark << setw(W_ID) << p->id;     // number and Prod_ID stay plain
        color::setBrandColor(p->brand);
        cout << setw(W_NAME) << display::fitText(p->name, W_NAME)
             << setw(W_BRAND) << display::fitText(p->brand, W_BRAND)
             << setw(W_QTY) << c->quantity
             << setw(W_PRICE) << display::money(p->price)
             << display::money(subtotal) << "\n";
        color::resetColor();
        total += subtotal;
    }
    cout << string(width, '=') << "\n"
         << right << setw(width) << ("Total: " + display::money(total)) << left << "\n";
}

// Doubly linked list navigation: next moves along ->next, previous moves along ->prev
// The whole cart is shown in one table and the > marker moves with each key press (no Enter).
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
        term::newScreen(display::tableWidth());
        cout << term::centered("--- Browse Cart ---") << "\n\n";
        displayCart(pos);
        cout << "\nItem " << pos << " of " << total << ": " << curr->product->name << "\n";
        if (!note.empty()) cout << note << "\n";
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
