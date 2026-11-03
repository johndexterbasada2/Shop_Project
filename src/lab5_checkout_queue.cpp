// Lab 5 - Queue (FIFO): checkout line
#include "lab5_checkout_queue.h"
#include "globals.h"
#include "console_color.h"
#include <iostream>

using namespace std;

void enqueueCheckout(CartNode* cart) {
    if (!cart) return;
    QueueNode* newNode = new QueueNode{cart, nullptr};
    if (!checkoutQueueTail) {
        checkoutQueueHead = checkoutQueueTail = newNode;
    } else {
        checkoutQueueTail->next = newNode;
        checkoutQueueTail = newNode;
    }
    cartHead = cartTail = nullptr; 
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
    cout << "\n--- Processing Order ---\n";
    while (curr) {
        if (curr->product->stockQuantity > 0) {
            curr->product->stockQuantity--;
            grandTotal += curr->product->price;
            cout << "Purchased: ";
            setBrandColor(curr->product->brand);
            cout << curr->product->name;
            resetColor();
            cout << " | Remaining Stock: " << curr->product->stockQuantity << "\n";
        } else {
            cout << "Failed to purchase (Out of Stock): " << curr->product->name << "\n";
        }
        CartNode* nextCart = curr->next;
        delete curr;
        curr = nextCart;
    }
    cout << "Order Complete. Grand Total: $" << grandTotal << "\n";
    delete temp;
}
