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

using namespace std;

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
        cout << "Enter selection: ";
        cin >> choice;

        if (choice == 1) {
            displayCatalog(catalog, catalogSize);
        } 
        else if (choice == 2) {
            string brandObj;
            cout << "Enter Brand Name: ";
            cin.ignore();
            getline(cin, brandObj);
            cout << "\n--- Filtered by " << brandObj << " ---\n";
            for (int i = 0; i < catalogSize; i++) {
                if (catalog[i].brand == brandObj) displayProduct(&catalog[i]);
            }
        } 
        else if (choice == 3) {
            string kw;
            cout << "Enter keyword: ";
            cin >> kw;
            linearSearchKeyword(catalog, catalogSize, kw);
        } 
        else if (choice == 4) {
            double minP, maxP;
            cout << "Enter minimum price (PHP): ";
            cin >> minP;
            cout << "Enter maximum price (PHP): ";
            cin >> maxP;
            binarySearchPriceFilter(catalog, catalogSize, minP, maxP);
        } 
        else if (choice == 5) {
            int id;
            cout << "Enter Product ID to add: ";
            cin >> id;
            BSTNode* result = searchBST(bstRoot, id);
            if (result) addToCart(result->product);
            else cout << "Product ID not found.\n";
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
            int sortOpt;
            cout << "1. Sort by Price (Ascending)\n2. Sort by Stock (Descending)\n3. Sort by Brand (Alphabetical)\nSelection: ";
            cin >> sortOpt;
            if (sortOpt == 1) quickSortByPrice(catalog, 0, catalogSize - 1);
            else if (sortOpt == 2) quickSortByStock(catalog, 0, catalogSize - 1);
            else if (sortOpt == 3) insertionSortByBrand(catalog, catalogSize);
            displayCatalog(catalog, catalogSize);
        }
    }

    delete[] catalog;
    return 0;
}