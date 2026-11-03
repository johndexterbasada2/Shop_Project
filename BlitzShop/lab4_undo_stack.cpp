// Lab 4 - Stack (LIFO): admin undo
#include "lab4_undo_stack.h"
#include "globals.h"

void pushUndo(Product* p) {
    StackNode* newNode = new StackNode{p, undoStackHead};
    undoStackHead = newNode;
}

Product* popUndo() {
    if (!undoStackHead) return nullptr;
    StackNode* temp = undoStackHead;
    Product* p = temp->product;
    undoStackHead = undoStackHead->next;
    delete temp;
    return p;
}
