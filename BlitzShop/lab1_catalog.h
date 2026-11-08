// Lab 1 - Dynamic Allocation: product catalog with dynamic resizing
#pragma once
#include "models.h"

const int CATALOG_SIZE = 10;   // starting capacity

// Allocates the default catalog. size/capacity are filled in. Caller must delete[] it.
Product* createCatalog(int& size, int& capacity);

// Appends a product. When the array is full it is resized (capacity doubles), every pointer
// that referenced the old array is moved to the new one, and the old array is freed.
// Returns the (possibly new) catalog array.
Product* addProductToCatalog(Product* catalog, int& size, int& capacity, const Product& item);
