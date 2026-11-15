// Lab 5 - Queue (FIFO): checkout line
#include "lab5_checkout_queue.h"
#include "globals.h"
#include "lab4_undo_stack.h"
#include "console_color.h"
#include "input_utils.h"
#include <iostream>
#include <iomanip>

using namespace std;

namespace checkout {

static int nextOrderNo = 1;

// Finished orders stay in a small list so earlier customers are still visible
static DoneNode* doneHead = nullptr;
static DoneNode* doneTail = nullptr;

static int unitsIn(CartNode* items) {
    int units = 0;
    for (CartNode* c = items; c; c = c->next) units += c->quantity;
    return units;
}

static double totalOf(CartNode* items) {
    double total = 0;
    for (CartNode* c = items; c; c = c->next) total += c->product->price * c->quantity;
    return total;
}

void enqueueCheckout(CartNode* items) {
    if (!items) {
        cout << "Your cart is empty. Add items first.\n";
        return;
    }
    string customer = input::readText("Customer name: ");   // asked first: nothing changes if it fails

    QueueNode* newNode = new QueueNode{nextOrderNo++, customer, items, nullptr};
    int position = 1;
    if (!globals::checkoutQueueTail) {
        globals::checkoutQueueHead = globals::checkoutQueueTail = newNode;
    } else {
        for (QueueNode* q = globals::checkoutQueueHead; q; q = q->next) position++;
        globals::checkoutQueueTail->next = newNode;
        globals::checkoutQueueTail = newNode;
    }
    globals::cartHead = globals::cartTail = nullptr;   // the cart now belongs to the order
    history::clearUndo();                              // a placed order is final, so its history is closed
    history::clearRedo();
    cout << "Order #" << newNode->orderNo << " placed for " << customer
         << ". Position in line: " << position << ".\n";
}

void processCheckoutQueue() {
    if (!globals::checkoutQueueHead) {
        cout << "Checkout queue is empty.\n";
        return;
    }
    QueueNode* temp = globals::checkoutQueueHead;
    globals::checkoutQueueHead = globals::checkoutQueueHead->next;
    if (!globals::checkoutQueueHead) globals::checkoutQueueTail = nullptr;

    int units = unitsIn(temp->cartHead);
    double grandTotal = totalOf(temp->cartHead);
    cout << "--- Processing Order #" << temp->orderNo << " (" << temp->customer << ") ---\n";
    CartNode* curr = temp->cartHead;
    while (curr) {
        // Stock was already deducted when the item was added to the cart
        cout << "Purchased: ";
        color::setBrandColor(curr->product->brand);
        cout << curr->product->name;
        color::resetColor();
        cout << " x" << curr->quantity << " | Remaining Stock: " << curr->product->stockQuantity << "\n";
        CartNode* nextCart = curr->next;
        delete curr;
        curr = nextCart;
    }
    cout << "Order Complete. Grand Total: PHP " << fixed << setprecision(2) << grandTotal << "\n";

    DoneNode* done = new DoneNode{temp->orderNo, temp->customer, units, grandTotal, nullptr};
    if (doneTail) doneTail->next = done;
    else doneHead = done;
    doneTail = done;
    delete temp;
}

void displayCheckoutQueue() {
    cout << "--- Waiting Orders (front to back) ---\n";
    if (!globals::checkoutQueueHead) {
        cout << "No orders are waiting.\n";
    }
    int position = 1;
    for (QueueNode* q = globals::checkoutQueueHead; q; q = q->next, position++) {
        cout << position << ". Order #" << q->orderNo << " | " << q->customer << " | "
             << unitsIn(q->cartHead) << " item(s) | PHP " << fixed << setprecision(2)
             << totalOf(q->cartHead) << "\n";
        for (CartNode* c = q->cartHead; c; c = c->next) {
            cout << "     - ";
            color::setBrandColor(c->product->brand);
            cout << c->product->name;
            color::resetColor();
            cout << " x" << c->quantity << "\n";
        }
    }

    cout << "\n--- Completed Orders ---\n";
    if (!doneHead) {
        cout << "No orders have been completed yet.\n";
        return;
    }
    for (DoneNode* d = doneHead; d; d = d->next) {
        cout << "Order #" << d->orderNo << " | " << d->customer << " | " << d->units
             << " item(s) | PHP " << fixed << setprecision(2) << d->total << "\n";
    }
}

void destroyCheckoutQueue() {
    while (globals::checkoutQueueHead) {
        QueueNode* nextOrder = globals::checkoutQueueHead->next;
        CartNode* c = globals::checkoutQueueHead->cartHead;
        while (c) {
            CartNode* nextCart = c->next;
            delete c;
            c = nextCart;
        }
        delete globals::checkoutQueueHead;
        globals::checkoutQueueHead = nextOrder;
    }
    globals::checkoutQueueTail = nullptr;

    while (doneHead) {
        DoneNode* next = doneHead->next;
        delete doneHead;
        doneHead = next;
    }
    doneTail = nullptr;
}

}  // namespace checkout
