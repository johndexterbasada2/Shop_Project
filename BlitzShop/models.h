#pragma once
#include <string>

using namespace std;

// ---------- Data models used across the whole project ----------

struct Product {
    int id;
    string name;
    string category;
    string brand;
    double price;
    int stockQuantity;
};

// Doubly linked list node (shopping cart). One node per product, with a quantity.
struct CartNode {
    Product* product;
    int quantity;
    CartNode* prev;
    CartNode* next;
};

// Circular singly linked list node (partner brand banner)
struct BrandNode {
    string brandName;
    BrandNode* next;
};

// What kind of admin action an undo/redo stack entry records (customer cart actions are not on the stacks)
enum ActionType { ACTION_RESTOCK, ACTION_ADD_PRODUCT };

// Stack node (admin undo / redo, LIFO)
struct StackNode {
    ActionType type;
    Product* product;
    int amount;          // units restocked
    Product item;        // copy of the product (only used by ACTION_ADD_PRODUCT, so redo can add it back)
    StackNode* next;
};

// Queue node (checkout orders, FIFO)
struct QueueNode {
    int orderNo;
    string customer;
    CartNode* cartHead;
    QueueNode* next;
};

// Finished order, kept so earlier customers still show up in the queue screen
struct DoneNode {
    int orderNo;
    string customer;
    int units;
    double total;
    DoneNode* next;
};

// Binary search tree node (product lookup by ID)
struct BSTNode {
    Product* product;
    BSTNode* left;
    BSTNode* right;
};
