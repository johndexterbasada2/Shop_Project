// Lab 7 - Searching: linear keyword search and binary search price filter
#pragma once
#include <string>
#include "models.h"

using namespace std;

namespace searching {

void linearSearchKeyword(Product* catalog, int size, string keyword);
void binarySearchPriceFilter(Product* catalog, int size, double minPrice, double maxPrice);

}  // namespace searching
