#pragma once
#include <string>

// ---------- Data models used across the whole project ----------

struct Product {
    int id;
    std::string name;
    std::string category;
    std::string brand;
    double price;
    int stockQuantity;
};

// Doubly linked list node (shopping cart)
struct CartNode {
    Product* product;
    CartNode* prev;
    CartNode* next;
};

// Circular singly linked list node (partner brand banner)
struct BrandNode {
    std::string brandName;
    BrandNode* next;
};

// What kind of action an undo/redo stack entry records
enum ActionType { ACTION_CART_ADD, ACTION_RESTOCK };

// Stack node (admin undo / redo, LIFO)
struct StackNode {
    ActionType type;
    Product* product;
    int amount;          // units added to cart (1) or restocked
    StackNode* next;
};

// Queue node (checkout orders, FIFO)
struct QueueNode {
    int orderNo;
    CartNode* cartHead;
    QueueNode* next;
};

// Binary search tree node (product lookup by ID)
struct BSTNode {
    Product* product;
    BSTNode* left;
    BSTNode* right;
};
