// Lab 7 - Searching: linear keyword search and binary search price filter
#include "lab7_searching.h"
#include "display.h"
#include "lab8_sorting.h"
#include <iostream>

using namespace std;

static string toLower(string s) {
    for (char& c : s) c = (char)tolower((unsigned char)c);
    return s;
}

// Linear search: checks every product's name, category AND brand (not case-sensitive)
void linearSearchKeyword(Product* catalog, int size, string keyword) {
    cout << "\n--- Search Results for '" << keyword << "' ---\n";
    string key = toLower(keyword);
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (toLower(catalog[i].name).find(key) != string::npos ||
            toLower(catalog[i].category).find(key) != string::npos ||
            toLower(catalog[i].brand).find(key) != string::npos) {
            displayProduct(&catalog[i]);
            found = true;
        }
    }
    if (!found) cout << "No products found.\n";
}

// Binary search: finds the first product >= minPrice in a price-sorted copy, then walks up to maxPrice
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

    cout << "\n--- Products between PHP " << minPrice << " and PHP " << maxPrice << " ---\n";
    int shown = 0;
    if (startIdx != -1) {
        for (int i = startIdx; i < size && tempCatalog[i].price <= maxPrice; i++) {
            displayProduct(&tempCatalog[i]);
            shown++;
        }
    }
    if (shown == 0) cout << "No products in this price range.\n";
    delete[] tempCatalog;
}
