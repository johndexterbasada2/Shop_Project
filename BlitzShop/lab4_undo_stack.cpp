// Lab 4 - Stack (LIFO): admin undo / redo of inventory actions
#include "lab4_undo_stack.h"
#include "lab2_cart.h"
#include "globals.h"
#include <iostream>

using namespace std;

static void push(StackNode*& head, ActionType type, Product* p, int amount) {
    head = new StackNode{type, p, amount, head};
}

static bool pop(StackNode*& head, StackNode& out) {
    if (!head) return false;
    StackNode* temp = head;
    out = *temp;
    head = head->next;
    delete temp;
    return true;
}

static void clearAll(StackNode*& head) {
    StackNode ignored;
    while (pop(head, ignored)) {}
}

void pushUndo(ActionType type, Product* p, int amount) { push(undoStackHead, type, p, amount); }
bool popUndo(StackNode& out) { return pop(undoStackHead, out); }
void pushRedo(ActionType type, Product* p, int amount) { push(redoStackHead, type, p, amount); }
bool popRedo(StackNode& out) { return pop(redoStackHead, out); }
void clearUndo() { clearAll(undoStackHead); }
void clearRedo() { clearAll(redoStackHead); }

void restockProduct(Product* p, int amount) {
    p->stockQuantity += amount;
    pushUndo(ACTION_RESTOCK, p, amount);
    clearRedo();
    cout << "Restocked " << p->name << " by " << amount << ". Stock is now " << p->stockQuantity << ".\n";
}

void undoLastAction() {
    StackNode action;
    if (!popUndo(action)) {
        cout << "No recent actions to undo.\n";
        return;
    }
    Product* p = action.product;

    if (action.type == ACTION_CART_ADD) {
        removeLastCartNode();
        p->stockQuantity += action.amount;
        cout << "Undo successful. Removed " << p->name << " from cart. Stock restored to "
             << p->stockQuantity << ".\n";
    } else {   // ACTION_RESTOCK
        if (p->stockQuantity < action.amount) {
            cout << "Cannot undo restock of " << p->name << ": those units are already in carts.\n";
            pushUndo(action.type, p, action.amount);   // keep the action on the stack
            return;
        }
        p->stockQuantity -= action.amount;
        cout << "Undo successful. Removed restock of " << action.amount << " from " << p->name
             << ". Stock is now " << p->stockQuantity << ".\n";
    }
    pushRedo(action.type, p, action.amount);
}

void redoLastAction() {
    StackNode action;
    if (!popRedo(action)) {
        cout << "No actions to redo.\n";
        return;
    }
    Product* p = action.product;

    if (action.type == ACTION_CART_ADD) {
        if (p->stockQuantity < action.amount) {
            cout << "Cannot redo: " << p->name << " is out of stock.\n";
            pushRedo(action.type, p, action.amount);
            return;
        }
        appendCartNode(p);
        p->stockQuantity -= action.amount;
        cout << "Redo successful. Added " << p->name << " back to cart. Stock is now "
             << p->stockQuantity << ".\n";
    } else {   // ACTION_RESTOCK
        p->stockQuantity += action.amount;
        cout << "Redo successful. Restocked " << p->name << " by " << action.amount
             << ". Stock is now " << p->stockQuantity << ".\n";
    }
    pushUndo(action.type, p, action.amount);   // redo-ed action can be undone again
}
