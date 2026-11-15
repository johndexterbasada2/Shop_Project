// Lab 7 - Searching: linear keyword search and binary search price filter
#pragma once
#include <string>
#include "models.h"

using namespace std;

namespace searching {

// Word match (not "contains"): every word of the keyword must match the START of a word in the
// text. A number must match a whole word, so "2" finds "Spider-Man 2" but not "2800".
bool matchesKeyword(const string& text, const string& keyword);

// Fills `out` with the products whose name, category or brand matches. Returns how many.
// `out` must have room for `size` pointers.
int findProducts(Product* catalog, int size, const string& keyword, Product** out);

// Fills `out` with the distinct brands that match the keyword (catalog order). Returns how many.
int matchBrands(Product* catalog, int size, const string& keyword, string* out);

void linearSearchKeyword(Product* catalog, int size, const string& keyword);
void binarySearchPriceFilter(Product* catalog, int size, double minPrice, double maxPrice);

}  // namespace searching
