// Lab 4 - Stack (LIFO): admin undo / redo of inventory actions
#pragma once
#include "models.h"

// Raw stack operations
void pushUndo(ActionType type, Product* p, int amount);
bool popUndo(StackNode& out);       // false when the stack is empty
void pushRedo(ActionType type, Product* p, int amount);
bool popRedo(StackNode& out);
void clearUndo();                   // frees every node
void clearRedo();

// Admin actions built on the stacks
void restockProduct(Product* p, int amount);   // adds stock, can be undone
void undoLastAction();                         // reverses the latest cart-add or restock
void redoLastAction();                         // re-applies the latest undone action
