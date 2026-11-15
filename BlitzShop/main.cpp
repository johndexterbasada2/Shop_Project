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
#include "terminal.h"

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
        int id = input::readInt(prompt, 0, INT_MAX);
        if (id == 0) return nullptr;
        BSTNode* result = bst::searchBST(globals::bstRoot, id);   // Lab 6: O(log n) lookup
        if (result) return result->product;
        cout << "Invalid input. Product ID not found. Try again.\n";
    }
}

// Rebuilds everything that depends on the catalog array (tree + brand banner)
static void rebuildIndexes(Product* catalog, int size) {
    bst::destroyBST(globals::bstRoot);
    globals::bstRoot = bst::buildBalancedBST(catalog, size);
    banner::initBrandBanner(catalog, size);
}

// Admin: add a brand-new product. The catalog array grows automatically when it is full.
static Product* addNewProduct(Product* catalog, int& size, int& capacity) {
    Product item;
    item.id = 0;
    for (int i = 0; i < size; i++) if (catalog[i].id > item.id) item.id = catalog[i].id;
    item.id++;
    item.name = input::readText("Product name: ");
    item.category = input::readText("Category: ");
    string brand = input::readText("Brand: ");
    string existing = findBrand(catalog, size, brand);
    item.brand = existing.empty() ? brand : existing;
    item.price = input::readDouble("Price (PHP): ", 0.01);
    item.stockQuantity = input::readInt("Starting stock (0-10000): ", 0, 10000);

    catalog = inventory::addProductToCatalog(catalog, size, capacity, item);
    rebuildIndexes(catalog, size);   // the array may have moved, so the tree is rebuilt
    cout << "Added product #" << item.id << ":\n";
    display::displayProduct(&catalog[size - 1]);
    return catalog;
}

static void cartMenu(Product* catalog, int size) {
    while (true) {
        term::newScreen(46);
        cout << term::centered("--- Shopping Cart ---") << "\n";
        cout << "1. Add item\n2. View cart\n3. Browse cart (Previous / Next)\n4. Back\n";
        char key = term::menuKey("Selection (1-4): ", "1234");
        if (key == '4') return;

        if (key == '1') {
            term::newScreen(100);
            display::displayCatalog(catalog, size);   // the catalog stays on screen so the IDs can be seen
            cout << "\n";
            Product* p = askForProduct("Enter Product ID to add (0 to cancel): ");
            if (!p) continue;
            cart::addToCart(p);
        } else if (key == '2') {
            term::newScreen(70);
            cart::displayCart();
        } else {
            cart::browseCart();   // draws its own screens
            continue;
        }
        term::pause();
    }
}

static void checkoutMenu() {
    while (true) {
        term::newScreen(50);
        cout << term::centered("--- Checkout Queue ---") << "\n";
        cout << "1. Place order (join the queue)\n2. Process next order (front of the queue)\n3. View queue\n4. Back\n";
        char key = term::menuKey("Selection (1-4): ", "1234");
        if (key == '4') return;

        if (key == '1') {
            term::newScreen(0);
            checkout::enqueueCheckout(globals::cartHead);
        } else if (key == '2') {
            term::newScreen(70);
            checkout::processCheckoutQueue();
        } else {
            term::newScreen(70);
            checkout::displayCheckoutQueue();
        }
        term::pause();
    }
}

static Product* adminMenu(Product* catalog, int& size, int& capacity) {
    while (true) {
        term::newScreen(52);
        cout << term::centered("--- Admin Actions ---") << "\n";
        cout << "1. Undo last action\n2. Redo last undone action\n3. Restock a product\n4. Add a new product (catalog resizes when full)\n5. Back\n";
        char key = term::menuKey("Selection (1-5): ", "12345");
        if (key == '5') return catalog;

        if (key == '1') {
            term::newScreen(0);
            history::undoLastAction();
        } else if (key == '2') {
            term::newScreen(0);
            history::redoLastAction();
        } else if (key == '3') {
            term::newScreen(100);
            display::displayCatalog(catalog, size);
            cout << "\n";
            Product* p = askForProduct("Enter Product ID to restock (0 to cancel): ");
            if (!p) continue;
            history::restockProduct(p, input::readInt("Units to add (1-1000): ", 1, 1000));
        } else {
            term::newScreen(100);
            cout << term::centered("--- Add a New Product ---") << "\n\n";
            catalog = addNewProduct(catalog, size, capacity);
        }
        term::pause();
    }
}

static void showMainMenu() {
    term::newScreen(70);
    cout << "======================================================================\n";
    cout << term::centered("BRAND-AFFILIATED E-COMMERCE INVENTORY & SEARCH SYSTEM (CS201)") << "\n";
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
    cout << "0. Exit\n";
    cout << "======================================================================\n";
}

int main() {
    term::init();

    int catalogSize = 0, catalogCapacity = 0;
    Product* catalog = inventory::createCatalog(catalogSize, catalogCapacity);   // Lab 1
    rebuildIndexes(catalog, catalogSize);                              // Lab 6 + Lab 3

    while (true) {
        showMainMenu();
        char key = term::menuKey("Enter selection (0-9): ", "0123456789");
        if (key == '0') break;
        int choice = key - '0';

        // 5, 6 and 7 open their own menus and manage their own screens
        if (choice == 5) { cartMenu(catalog, catalogSize); continue; }
        if (choice == 6) { checkoutMenu(); continue; }
        if (choice == 7) { catalog = adminMenu(catalog, catalogSize, catalogCapacity); continue; }

        if (choice == 9) {
            term::newScreen(44);
            cout << term::centered("--- Sort Inventory ---") << "\n";
            cout << "1. Sort by Price (Ascending)\n2. Sort by Stock (Descending)\n3. Sort by Brand (Alphabetical)\n";
            char sortKey = term::menuKey("Selection (1-3): ", "123");
            term::newScreen(100);
            // Sort a copy for display so cart/undo pointers still reference the real products
            Product* sorted = new Product[catalogSize];
            for (int i = 0; i < catalogSize; i++) sorted[i] = catalog[i];
            if (sortKey == '1') sorting::quickSortByPrice(sorted, 0, catalogSize - 1);
            else if (sortKey == '2') sorting::quickSortByStock(sorted, 0, catalogSize - 1);
            else sorting::insertionSortByBrand(sorted, catalogSize);
            display::displayCatalog(sorted, catalogSize);
            delete[] sorted;
            term::pause();
            continue;
        }

        if (choice == 8) term::newScreen(0);
        else term::newScreen(100);

        if (choice == 1) {
            display::displayCatalog(catalog, catalogSize);
        }
        else if (choice == 2) {
            cout << "Available brands: " << banner::brandListText() << "\n\n";
            string brandObj;
            while (true) {
                brandObj = findBrand(catalog, catalogSize, input::readText("Enter Brand Name: "));
                if (brandObj != "") break;
                cout << "Invalid brand. Available: " << banner::brandListText() << ". Try again.\n";
            }
            cout << "\n--- Filtered by " << brandObj << " ---\n";
            for (int i = 0; i < catalogSize; i++) {
                if (catalog[i].brand == brandObj) display::displayProduct(&catalog[i]);
            }
        }
        else if (choice == 3) {
            string kw = input::readText("Enter keyword: ");
            searching::linearSearchKeyword(catalog, catalogSize, kw);
        }
        else if (choice == 4) {
            double minP = input::readDouble("Enter minimum price (PHP): ", 0);
            double maxP;
            while (true) {
                maxP = input::readDouble("Enter maximum price (PHP): ", 0);
                if (maxP >= minP) break;
                cout << "Invalid input. Maximum price must be greater than or equal to the minimum. Try again.\n";
            }
            searching::binarySearchPriceFilter(catalog, catalogSize, minP, maxP);
        }
        else if (choice == 8) {
            banner::showBrandBanner();
        }
        term::pause();
    }

    // ---- Free every dynamic structure (zero memory leaks) ----
    cart::destroyCart();            // Lab 2
    checkout::destroyCheckoutQueue();   // Lab 5
    history::clearUndo();              // Lab 4
    history::clearRedo();
    bst::destroyBST(globals::bstRoot);      // Lab 6
    globals::bstRoot = nullptr;
    banner::destroyBrandBanner();     // Lab 3
    delete[] catalog;         // Lab 1
    term::newScreen(0);
    cout << "Thank you for shopping at BlitzShop. Goodbye!\n\n";
    term::shutdown();
    return 0;
}
