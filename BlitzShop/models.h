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

// Stack node (admin undo, LIFO)
struct StackNode {
    Product* product;
    StackNode* next;
};

// Queue node (checkout orders, FIFO)
struct QueueNode {
    CartNode* cartHead;
    QueueNode* next;
};

// Binary search tree node (product lookup by ID)
struct BSTNode {
    Product* product;
    BSTNode* left;
    BSTNode* right;
};
