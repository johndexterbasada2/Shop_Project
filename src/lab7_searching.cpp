// Lab 7 - Searching: linear keyword search and binary search price filter
#include "lab7_searching.h"
#include "display.h"
#include "lab8_sorting.h"
#include <iostream>

using namespace std;

void linearSearchKeyword(Product* catalog, int size, string keyword) {
    cout << "\n--- Search Results for '" << keyword << "' ---\n";
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (catalog[i].name.find(keyword) != string::npos || catalog[i].category.find(keyword) != string::npos) {
            displayProduct(&catalog[i]);
            found = true;
        }
    }
    if (!found) cout << "No products found.\n";
}

void binarySearchPriceFilter(Product* catalog, int size, double minPrice, double maxPrice) {
    Product* tempCatalog = new Product[size];
    for (int i = 0; i < size; i++) tempCatalog[i] = catalog[i];
    quickSortByPrice(tempCatalog, 0, size - 1);

    int left = 0, right = size - 1, startIdx = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (tempCatalog[mid].price >= minPrice) {
            startIdx = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    cout << "\n--- Products between $" << minPrice << " and $" << maxPrice << " ---\n";
    if (startIdx != -1) {
        for (int i = startIdx; i < size && tempCatalog[i].price <= maxPrice; i++) {
            displayProduct(&tempCatalog[i]);
        }
    } else {
        cout << "No products in this price range.\n";
    }
    delete[] tempCatalog;
}
