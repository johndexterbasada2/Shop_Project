// Lab 7 - Searching: linear keyword search and binary search price filter
#include "lab7_searching.h"
#include "display.h"
#include "lab8_sorting.h"
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

namespace searching {

// Splits text into lowercase letter/digit words: "Spider-Man 2 (PS5)" -> spider, man, 2, ps5
static vector<string> wordsOf(const string& text) {
    vector<string> words;
    string current;
    for (char ch : text) {
        if (isalnum((unsigned char)ch)) {
            current += (char)tolower((unsigned char)ch);
        } else if (!current.empty()) {
            words.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) words.push_back(current);
    return words;
}

static bool allDigits(const string& s) {
    for (char ch : s) if (!isdigit((unsigned char)ch)) return false;
    return !s.empty();
}

bool matchesKeyword(const string& text, const string& keyword) {
    vector<string> wanted = wordsOf(keyword);
    vector<string> words = wordsOf(text);
    if (wanted.empty()) return false;

    for (const string& w : wanted) {
        bool numeric = allDigits(w);
        bool found = false;
        for (const string& t : words) {
            if (numeric ? (t == w) : (t.compare(0, w.size(), w) == 0)) { found = true; break; }
        }
        if (!found) return false;
    }
    return true;
}

// Linear search: checks every product's name, category AND brand (not case-sensitive)
int findProducts(Product* catalog, int size, const string& keyword, Product** out) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        string text = catalog[i].name + " " + catalog[i].category + " " + catalog[i].brand;
        if (matchesKeyword(text, keyword)) out[count++] = &catalog[i];
    }
    return count;
}

int matchBrands(Product* catalog, int size, const string& keyword, string* out) {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (!matchesKeyword(catalog[i].brand, keyword)) continue;
        bool seen = false;
        for (int k = 0; k < count; k++) if (out[k] == catalog[i].brand) seen = true;
        if (!seen) out[count++] = catalog[i].brand;
    }
    return count;
}

void linearSearchKeyword(Product* catalog, int size, const string& keyword) {
    cout << "\n--- Search Results for '" << keyword << "' ---\n";
    vector<Product*> found(size > 0 ? size : 1);
    int count = findProducts(catalog, size, keyword, found.data());
    if (count == 0) {
        cout << "No products found.\n";
        return;
    }
    display::displayHeader();
    for (int i = 0; i < count; i++) display::displayProduct(found[i]);
    display::displayFooter();
}

// Binary search: finds the first product >= minPrice in a price-sorted copy, then walks up to maxPrice
void binarySearchPriceFilter(Product* catalog, int size, double minPrice, double maxPrice) {
    Product* tempCatalog = new Product[size];
    for (int i = 0; i < size; i++) tempCatalog[i] = catalog[i];
    sorting::quickSortByPrice(tempCatalog, 0, size - 1);

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

    cout << "\n--- Products between PHP " << fixed << setprecision(2) << minPrice
         << " and PHP " << maxPrice << " ---\n";
    int shown = 0;
    if (startIdx != -1) {
        for (int i = startIdx; i < size && tempCatalog[i].price <= maxPrice; i++) {
            if (shown == 0) display::displayHeader();
            display::displayProduct(&tempCatalog[i]);
            shown++;
        }
    }
    if (shown == 0) cout << "No products in this price range.\n";
    else display::displayFooter();
    delete[] tempCatalog;
}

}  // namespace searching
