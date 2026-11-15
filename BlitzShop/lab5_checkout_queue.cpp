// Lab 5 - Queue (FIFO): checkout line
#include "lab5_checkout_queue.h"
#include "globals.h"
#include "lab4_undo_stack.h"
#include "console_color.h"
#include <iostream>
#include <iomanip>

using namespace std;

static int nextOrderNo = 1;

void enqueueCheckout(CartNode* cart) {
    if (!cart) {
        cout << "Your cart is empty. Add items first.\n";
        return;
    }
    QueueNode* newNode = new QueueNode{nextOrderNo++, cart, nullptr};
    int position = 1;
    if (!checkoutQueueTail) {
        checkoutQueueHead = checkoutQueueTail = newNode;
    } else {
        for (QueueNode* q = checkoutQueueHead; q; q = q->next) position++;
        checkoutQueueTail->next = newNode;
        checkoutQueueTail = newNode;
    }
    cartHead = cartTail = nullptr;   // the cart now belongs to the order
    clearUndo();                     // a placed order is final, so its history is closed
    clearRedo();
    cout << "Order #" << newNode->orderNo << " placed. Position in line: " << position << ".\n";
}

void processCheckoutQueue() {
    if (!checkoutQueueHead) {
        cout << "Checkout queue is empty.\n";
        return;
    }
    QueueNode* temp = checkoutQueueHead;
    checkoutQueueHead = checkoutQueueHead->next;
    if (!checkoutQueueHead) checkoutQueueTail = nullptr;

    CartNode* curr = temp->cartHead;
    double grandTotal = 0;
    cout << "\n--- Processing Order #" << temp->orderNo << " ---\n";
    while (curr) {
        // Stock was already deducted when the item was added to the cart
        grandTotal += curr->product->price;
        cout << "Purchased: ";
        setBrandColor(curr->product->brand);
        cout << curr->product->name;
        resetColor();
        cout << " | Remaining Stock: " << curr->product->stockQuantity << "\n";
        CartNode* nextCart = curr->next;
        delete curr;
        curr = nextCart;
    }
    cout << "Order Complete. Grand Total: PHP " << fixed << setprecision(2) << grandTotal << "\n";
    delete temp;
}

void displayCheckoutQueue() {
    if (!checkoutQueueHead) {
        cout << "Checkout queue is empty.\n";
        return;
    }
    cout << "\n--- Checkout Queue (front to back) ---\n";
    int position = 1;
    for (QueueNode* q = checkoutQueueHead; q; q = q->next, position++) {
        int items = 0;
        double total = 0;
        for (CartNode* c = q->cartHead; c; c = c->next) { items++; total += c->product->price; }
        cout << position << ". Order #" << q->orderNo << " | " << items << " item(s) | PHP "
             << fixed << setprecision(2) << total << "\n";
    }
}

void destroyCheckoutQueue() {
    while (checkoutQueueHead) {
        QueueNode* nextOrder = checkoutQueueHead->next;
        CartNode* c = checkoutQueueHead->cartHead;
        while (c) {
            CartNode* nextCart = c->next;
            delete c;
            c = nextCart;
        }
        delete checkoutQueueHead;
        checkoutQueueHead = nextOrder;
    }
    checkoutQueueTail = nullptr;
}
