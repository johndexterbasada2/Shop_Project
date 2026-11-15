// Lab 4 - Stack (LIFO): admin undo / redo of inventory actions
#pragma once
#include "models.h"

using namespace std;

namespace history {

// Raw stack operations. `item` is only used by ACTION_ADD_PRODUCT.
void pushUndo(ActionType type, Product* p, int amount, const Product& item = Product());
bool popUndo(StackNode& out);       // false when the stack is empty
void pushRedo(ActionType type, Product* p, int amount, const Product& item = Product());
bool popRedo(StackNode& out);
void clearUndo();                   // frees every node
void clearRedo();

// Admin actions built on the stacks
void restockProduct(Product* p, int amount);   // adds stock, can be undone
void recordProductAdded(Product* p);           // call after a new product was added to the catalog

// Undo and redo can change the catalog (a product is removed or added back), so they get the
// catalog array, its size and its capacity.
void undoLastAction(Product*& catalog, int& size, int& capacity);   // reverses the latest action
void redoLastAction(Product*& catalog, int& size, int& capacity);   // re-applies the latest undone action

}  // namespace history
