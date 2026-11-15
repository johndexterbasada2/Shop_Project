// Lab 1 - Dynamic Allocation: product catalog with dynamic resizing
#pragma once
#include "models.h"

namespace inventory {

const int CATALOG_SIZE = 10;   // starting capacity

// Allocates the default catalog. size/capacity are filled in. Caller must delete[] it.
Product* createCatalog(int& size, int& capacity);

// Appends a product. When the array is full it is resized (capacity doubles), every pointer
// that referenced the old array is moved to the new one, and the old array is freed.
// Returns the (possibly new) catalog array.
Product* addProductToCatalog(Product* catalog, int& size, int& capacity, const Product& item);

// True when the product sits in the cart or in a waiting order, so it must not be removed.
bool isProductInUse(const Product* p);

// Rebuilds everything that depends on the catalog array (search tree + brand banner).
// Call it after the array moves or after a product is added or removed.
void rebuildIndexes(Product* catalog, int size);

}  // namespace inventory
