// Lab 1 - Dynamic Allocation: product catalog
#pragma once
#include "models.h"

const int CATALOG_SIZE = 10;

// Allocates and returns the default product catalog (caller must delete[]).
Product* createCatalog();