#include <iostream>
#include <string>

#include "models.h"
#include "globals.h"
#include "lab1_catalog.h"
#include "display.h"
#include "lab6_bst.h"
#include "lab2_cart.h"
#include "lab5_checkout_queue.h"
#include "lab3_brand_banner.h"
#include "lab8_sorting.h"
#include "lab7_searching.h"
#include "input_utils.h"

using namespace std;

// Case-insensitive compare, returns the catalog's official brand spelling ("" if none)
static string findBrand(Product* catalog, int size, const string& input) {
    auto lower = [](string s) { for (char& c : s) c = (char)tolower((unsigned char)c); return s; };
    for (int i = 0; i < size; i++)
        if (lower(catalog[i].brand) == lower(input)) return catalog[i].brand;
    return "";
}

int main() {
    int catalogSize = CATALOG_SIZE;
    Product* catalog = createCatalog();

    for (int i = 0; i < catalogSize; i++) {
        bstRoot = insertBST(bstRoot, &catalog[i]);
    }

    initBrandBanner();

    int choice = 0;
    while (choice != 10) {
        cout << "\n======================================================================\n";
        cout << "BRAND-AFFILIATED E-COMMERCE INVENTORY & SEARCH SYSTEM (CS201)\n";
        cout << "======================================================================\n";
        cout << "1. View Complete Catalog (Tabular View with Brand Affiliations)\n";
        cout << "2. Filter Products by Official Brand (e.g., Sony, Nintendo, Microsoft)\n";
        cout << "3. Search Products by Keyword (Linear Search)\n";
        cout << "4. Filter Products by Price Range (Binary Search)\n";
        cout << "5. Add Item to Shopping Cart (Doubly Linked List Navigation)\n";
        cout << "6. Process Customer Checkout Queue (FIFO Order Queue)\n";
        cout << "7. Perform Admin Undo Action (LIFO Stack Pop)\n";
        cout << "8. Display Partner Brand Banner Showcase (Circular List Cycle)\n";
        cout << "9. Sort Inventory Display (By Price, Stock, or Brand)\n";
        cout << "10. Exit\n";
        cout << "======================================================================\n";
        choice = readInt("Enter selection (1-10): ", 1, 10);

        if (choice == 1) {
            displayCatalog(catalog, catalogSize);
        } 
        else if (choice == 2) {
            string brandObj;
            while (true) {
                brandObj = findBrand(catalog, catalogSize, readText("Enter Brand Name: "));
                if (brandObj != "") break;
                cout << "Invalid brand. Available: Sony, Microsoft, Nintendo, The Pokemon Company, Epic Games, Roblox Corporation. Try again.\n";
            }
            cout << "\n--- Filtered by " << brandObj << " ---\n";
            for (int i = 0; i < catalogSize; i++) {
                if (catalog[i].brand == brandObj) displayProduct(&catalog[i]);
            }
        } 
        else if (choice == 3) {
            string kw = readText("Enter keyword: ");
            linearSearchKeyword(catalog, catalogSize, kw);
        } 
        else if (choice == 4) {
            double minP = readDouble("Enter minimum price (PHP): ", 0);
            double maxP;
            while (true) {
                maxP = readDouble("Enter maximum price (PHP): ", 0);
                if (maxP >= minP) break;
                cout << "Invalid input. Maximum price must be greater than or equal to the minimum. Try again.\n";
            }
            binarySearchPriceFilter(catalog, catalogSize, minP, maxP);
        } 
        else if (choice == 5) {
            while (true) {
                int id = readInt("Enter Product ID to add (0 to cancel): ", 0, INT_MAX);
                if (id == 0) break;
                BSTNode* result = searchBST(bstRoot, id);
                if (result) { addToCart(result->product); break; }
                cout << "Invalid input. Product ID not found. Try again.\n";
            }
        } 
        else if (choice == 6) {
            enqueueCheckout(cartHead);
            processCheckoutQueue();
        } 
        else if (choice == 7) {
            undoLastAction();
        } 
        else if (choice == 8) {
            showBrandBanner();
        } 
        else if (choice == 9) {
            cout << "1. Sort by Price (Ascending)\n2. Sort by Stock (Descending)\n3. Sort by Brand (Alphabetical)\n";
            int sortOpt = readInt("Selection (1-3): ", 1, 3);
            if (sortOpt == 1) quickSortByPrice(catalog, 0, catalogSize - 1);
            else if (sortOpt == 2) quickSortByStock(catalog, 0, catalogSize - 1);
            else if (sortOpt == 3) insertionSortByBrand(catalog, catalogSize);
            displayCatalog(catalog, catalogSize);
        }
    }

    delete[] catalog;
    return 0;
}