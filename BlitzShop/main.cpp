#include <iostream>
#include <string>

#include "models.h"
#include "globals.h"
#include "lab1_catalog.h"
#include "display.h"
#include "lab6_bst.h"
#include "lab2_cart.h"
#include "lab4_undo_stack.h"
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

// Asks for a product ID until it exists (0 cancels). Returns nullptr if cancelled.
static Product* askForProduct(const string& prompt) {
    while (true) {
        int id = readInt(prompt, 0, INT_MAX);
        if (id == 0) return nullptr;
        BSTNode* result = searchBST(bstRoot, id);   // Lab 6: O(log n) lookup
        if (result) return result->product;
        cout << "Invalid input. Product ID not found. Try again.\n";
    }
}

// Rebuilds everything that depends on the catalog array (tree + brand banner)
static void rebuildIndexes(Product* catalog, int size) {
    destroyBST(bstRoot);
    bstRoot = buildBalancedBST(catalog, size);
    initBrandBanner(catalog, size);
}

// Admin: add a brand-new product. The catalog array grows automatically when it is full.
static Product* addNewProduct(Product* catalog, int& size, int& capacity) {
    Product item;
    item.id = 0;
    for (int i = 0; i < size; i++) if (catalog[i].id > item.id) item.id = catalog[i].id;
    item.id++;
    item.name = readText("Product name: ");
    item.category = readText("Category: ");
    string brand = readText("Brand: ");
    string existing = findBrand(catalog, size, brand);
    item.brand = existing.empty() ? brand : existing;
    item.price = readDouble("Price (PHP): ", 0.01);
    item.stockQuantity = readInt("Starting stock (0-10000): ", 0, 10000);

    catalog = addProductToCatalog(catalog, size, capacity, item);
    rebuildIndexes(catalog, size);   // the array may have moved, so the tree is rebuilt
    cout << "Added product #" << item.id << ":\n";
    displayProduct(&catalog[size - 1]);
    return catalog;
}

static void cartMenu() {
    while (true) {
        cout << "\n--- Shopping Cart ---\n1. Add item\n2. View cart\n3. Browse cart (Previous / Next)\n4. Back\n";
        int opt = readInt("Selection (1-4): ", 1, 4);
        if (opt == 1) {
            Product* p = askForProduct("Enter Product ID to add (0 to cancel): ");
            if (p) addToCart(p);
        } else if (opt == 2) {
            displayCart();
        } else if (opt == 3) {
            browseCart();
        } else {
            return;
        }
    }
}

static void checkoutMenu() {
    while (true) {
        cout << "\n--- Checkout Queue ---\n1. Place order (join the queue)\n2. Process next order (front of the queue)\n3. View queue\n4. Back\n";
        int opt = readInt("Selection (1-4): ", 1, 4);
        if (opt == 1) enqueueCheckout(cartHead);
        else if (opt == 2) processCheckoutQueue();
        else if (opt == 3) displayCheckoutQueue();
        else return;
    }
}

static Product* adminMenu(Product* catalog, int& size, int& capacity) {
    while (true) {
        cout << "\n--- Admin Actions ---\n1. Undo last action\n2. Redo last undone action\n3. Restock a product\n4. Add a new product (catalog resizes when full)\n5. Back\n";
        int opt = readInt("Selection (1-5): ", 1, 5);
        if (opt == 1) {
            undoLastAction();
        } else if (opt == 2) {
            redoLastAction();
        } else if (opt == 3) {
            Product* p = askForProduct("Enter Product ID to restock (0 to cancel): ");
            if (p) restockProduct(p, readInt("Units to add (1-1000): ", 1, 1000));
        } else if (opt == 4) {
            catalog = addNewProduct(catalog, size, capacity);
        } else {
            return catalog;
        }
    }
}

int main() {
    int catalogSize = 0, catalogCapacity = 0;
    Product* catalog = createCatalog(catalogSize, catalogCapacity);   // Lab 1
    rebuildIndexes(catalog, catalogSize);                              // Lab 6 + Lab 3

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
                cout << "Invalid brand. Available: " << brandListText() << ". Try again.\n";
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
            cartMenu();
        }
        else if (choice == 6) {
            checkoutMenu();
        }
        else if (choice == 7) {
            catalog = adminMenu(catalog, catalogSize, catalogCapacity);
        }
        else if (choice == 8) {
            showBrandBanner();
        }
        else if (choice == 9) {
            cout << "1. Sort by Price (Ascending)\n2. Sort by Stock (Descending)\n3. Sort by Brand (Alphabetical)\n";
            int sortOpt = readInt("Selection (1-3): ", 1, 3);
            // Sort a copy for display so cart/undo pointers still reference the real products
            Product* sorted = new Product[catalogSize];
            for (int i = 0; i < catalogSize; i++) sorted[i] = catalog[i];
            if (sortOpt == 1) quickSortByPrice(sorted, 0, catalogSize - 1);
            else if (sortOpt == 2) quickSortByStock(sorted, 0, catalogSize - 1);
            else if (sortOpt == 3) insertionSortByBrand(sorted, catalogSize);
            displayCatalog(sorted, catalogSize);
            delete[] sorted;
        }
    }

    // ---- Free every dynamic structure (zero memory leaks) ----
    destroyCart();            // Lab 2
    destroyCheckoutQueue();   // Lab 5
    clearUndo();              // Lab 4
    clearRedo();
    destroyBST(bstRoot);      // Lab 6
    bstRoot = nullptr;
    destroyBrandBanner();     // Lab 3
    delete[] catalog;         // Lab 1
    cout << "Thank you for shopping at BlitzShop. Goodbye!\n";
    return 0;
}
