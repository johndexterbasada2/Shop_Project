// Lab 3 - Circular Linked List: partner brand banner
#pragma once
#include <string>
#include "models.h"

void initBrandBanner(Product* catalog, int size);   // (re)builds the list from the catalog's brands
void showBrandBanner();
std::string brandListText();                        // "Sony, Microsoft, ..." read from the list
void destroyBrandBanner();                          // frees every node
