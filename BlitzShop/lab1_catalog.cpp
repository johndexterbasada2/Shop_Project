// Lab 1 - Dynamic Allocation: product catalog with dynamic resizing
#include "lab1_catalog.h"
#include "globals.h"
#include <iostream>

using namespace std;

// All prices are in Philippine pesos (PHP).
Product* createCatalog(int& size, int& capacity) {
    Product* catalog = new Product[CATALOG_SIZE]{
        {101, "PlayStation 5 Console", "Consoles", "Sony", 31749.37, 10},
        {102, "Spider-Man 2 (PS5)", "Games", "Sony", 4444.37, 25},
        {103, "Xbox Series X Console", "Consoles", "Microsoft", 31749.37, 8},
        {104, "Halo Infinite (Xbox)", "Games", "Microsoft", 3809.37, 30},
        {105, "Nintendo Switch OLED", "Consoles", "Nintendo", 22224.37, 15},
        {106, "Mario Kart 8 Deluxe", "Games", "Nintendo", 3809.37, 40},
        {107, "Pokemon TCG: 151 ETB", "Pokemon Cards", "The Pokemon Company", 3174.37, 12},
        {108, "Pokemon Booster Bundle", "Pokemon Cards", "The Pokemon Company", 1713.87, 20},
        {109, "Fortnite 2800 V-Bucks", "Gift Cards", "Epic Games", 1269.37, 50},
        {110, "Roblox $10 Gift Card", "Gift Cards", "Roblox Corporation", 635.00, 50}
    };
    size = capacity = CATALOG_SIZE;
    return catalog;
}

// Cart, undo/redo stacks and checkout queue all point INTO the catalog array.
// When the array moves to new memory those pointers must follow it.
static Product* moved(Product* p, Product* oldBase, Product* newBase) {
    return newBase + (p - oldBase);
}

static void relocatePointers(Product* oldBase, Product* newBase) {
    for (CartNode* c = cartHead; c; c = c->next)
        c->product = moved(c->product, oldBase, newBase);
    for (StackNode* s = undoStackHead; s; s = s->next)
        s->product = moved(s->product, oldBase, newBase);
    for (StackNode* s = redoStackHead; s; s = s->next)
        s->product = moved(s->product, oldBase, newBase);
    for (QueueNode* q = checkoutQueueHead; q; q = q->next)
        for (CartNode* c = q->cartHead; c; c = c->next)
            c->product = moved(c->product, oldBase, newBase);
}

Product* addProductToCatalog(Product* catalog, int& size, int& capacity, const Product& item) {
    if (size == capacity) {
        int newCapacity = capacity * 2;
        Product* bigger = new Product[newCapacity];
        for (int i = 0; i < size; i++) bigger[i] = catalog[i];
        relocatePointers(catalog, bigger);   // must happen BEFORE the old array is freed
        delete[] catalog;
        cout << "Catalog was full: resized from " << capacity << " to " << newCapacity << " slots.\n";
        catalog = bigger;
        capacity = newCapacity;
    }
    catalog[size++] = item;
    return catalog;
}
