// Lab 4 - Stack (LIFO): admin undo / redo of inventory actions
#include "lab4_undo_stack.h"
#include "lab1_catalog.h"
#include "lab2_cart.h"
#include "globals.h"
#include "console_color.h"
#include <iostream>

using namespace std;

namespace history {

static void push(StackNode*& head, ActionType type, Product* p, int amount, const Product& item) {
    head = new StackNode{type, p, amount, item, head};
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

void pushUndo(ActionType type, Product* p, int amount, const Product& item) { push(globals::undoStackHead, type, p, amount, item); }
bool popUndo(StackNode& out) { return pop(globals::undoStackHead, out); }
void pushRedo(ActionType type, Product* p, int amount, const Product& item) { push(globals::redoStackHead, type, p, amount, item); }
bool popRedo(StackNode& out) { return pop(globals::redoStackHead, out); }
void clearUndo() { clearAll(globals::undoStackHead); }
void clearRedo() { clearAll(globals::redoStackHead); }

void restockProduct(Product* p, int amount) {
    p->stockQuantity += amount;
    pushUndo(ACTION_RESTOCK, p, amount);
    clearRedo();
    color::setGreen();
    cout << "Restocked " << p->name << " by " << amount << ". Stock is now " << p->stockQuantity << ".";
    color::resetColor();
    cout << "\n";
}

void recordProductAdded(Product* p) {
    pushUndo(ACTION_ADD_PRODUCT, p, 0, *p);   // keeps a copy so redo can add it back
    clearRedo();
}

void undoLastAction(Product*& catalog, int& size, int& capacity) {
    (void)capacity;
    StackNode action;
    if (!popUndo(action)) {
        cout << "No recent actions to undo.\n";
        return;
    }
    Product* p = action.product;

    if (action.type == ACTION_CART_ADD) {
        if (!cart::takeUnits(p, action.amount)) {
            cout << "Cannot undo: " << p->name << " is no longer in the cart.\n";
            pushUndo(action.type, p, action.amount, action.item);
            return;
        }
        p->stockQuantity += action.amount;
        cout << "Undo successful. Removed " << action.amount << " x " << p->name
             << " from cart. Stock restored to " << p->stockQuantity << ".\n";
    } else if (action.type == ACTION_CART_REMOVE) {
        if (p->stockQuantity < action.amount) {
            cout << "Cannot undo: only " << p->stockQuantity << " of " << p->name << " left in stock.\n";
            pushUndo(action.type, p, action.amount, action.item);
            return;
        }
        cart::putUnits(p, action.amount);
        p->stockQuantity -= action.amount;
        cout << "Undo successful. Put " << action.amount << " x " << p->name
             << " back in the cart. Stock is now " << p->stockQuantity << ".\n";
    } else if (action.type == ACTION_RESTOCK) {
        if (p->stockQuantity < action.amount) {
            cout << "Cannot undo restock of " << p->name << ": those units are already in carts.\n";
            pushUndo(action.type, p, action.amount, action.item);   // keep the action on the stack
            return;
        }
        p->stockQuantity -= action.amount;
        cout << "Undo successful. Removed restock of " << action.amount << " from " << p->name
             << ". Stock is now " << p->stockQuantity << ".\n";
    } else {   // ACTION_ADD_PRODUCT: the new product is always the last one in the array
        if (size == 0 || p != &catalog[size - 1]) {
            cout << "Cannot undo: " << p->name << " is no longer the newest product.\n";
            pushUndo(action.type, p, action.amount, action.item);
            return;
        }
        string name = p->name;
        size--;
        inventory::rebuildIndexes(catalog, size);   // the search tree and brand banner must forget it
        cout << "Undo successful. Removed product '" << name << "' from the catalog.\n";
    }
    pushRedo(action.type, p, action.amount, action.item);
}

void redoLastAction(Product*& catalog, int& size, int& capacity) {
    StackNode action;
    if (!popRedo(action)) {
        cout << "No actions to redo.\n";
        return;
    }
    Product* p = action.product;

    if (action.type == ACTION_CART_ADD) {
        if (p->stockQuantity < action.amount) {
            cout << "Cannot redo: only " << p->stockQuantity << " of " << p->name << " left in stock.\n";
            pushRedo(action.type, p, action.amount, action.item);
            return;
        }
        cart::putUnits(p, action.amount);
        p->stockQuantity -= action.amount;
        cout << "Redo successful. Added " << action.amount << " x " << p->name
             << " back to cart. Stock is now " << p->stockQuantity << ".\n";
    } else if (action.type == ACTION_CART_REMOVE) {
        if (!cart::takeUnits(p, action.amount)) {
            cout << "Cannot redo: " << p->name << " is not in the cart.\n";
            pushRedo(action.type, p, action.amount, action.item);
            return;
        }
        p->stockQuantity += action.amount;
        cout << "Redo successful. Removed " << action.amount << " x " << p->name
             << " from cart again. Stock is now " << p->stockQuantity << ".\n";
    } else if (action.type == ACTION_RESTOCK) {
        p->stockQuantity += action.amount;
        color::setGreen();
        cout << "Redo successful. Restocked " << p->name << " by " << action.amount
             << ". Stock is now " << p->stockQuantity << ".";
        color::resetColor();
        cout << "\n";
    } else {   // ACTION_ADD_PRODUCT: add the saved copy back at the end of the array
        catalog = inventory::addProductToCatalog(catalog, size, capacity, action.item);
        inventory::rebuildIndexes(catalog, size);
        p = &catalog[size - 1];
        cout << "Redo successful. Added product '" << p->name << "' back to the catalog.\n";
    }
    pushUndo(action.type, p, action.amount, action.item);   // redo-ed action can be undone again
}

}  // namespace history
