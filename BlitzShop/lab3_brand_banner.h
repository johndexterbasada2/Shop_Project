// Lab 3 - Circular Linked List: partner brand banner
#pragma once
#include <string>
#include "models.h"

using namespace std;

namespace banner {

void initBrandBanner(Product* catalog, int size);   // (re)builds the list from the catalog's brands
void showBrandBanner();
string brandListText();                        // "Sony, Microsoft, ..." read from the list
void destroyBrandBanner();                          // frees every node

}  // namespace banner
