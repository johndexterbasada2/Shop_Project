#include <iostream>
#include <string>
#include <vector>

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
#include "console_color.h"
#include "input_utils.h"
#include "terminal.h"

using namespace std;

// Runs one menu action. After 3 wrong answers in a row the action is cancelled and the
// program goes back to the menu it came from.
template <typename Action>
static void guarded(Action action) {
    try {
        action();
    } catch (const input::TooManyAttempts&) {
        cout << "\nToo many invalid attempts. Returning to the menu.\n";
        term::pause();
    }
}

// Case-insensitive compare, returns the catalog's official brand spelling ("" if none)
static string findBrand(Product* catalog, int size, const string& text) {
    auto lower = [](string s) { for (char& c : s) c = (char)tolower((unsigned char)c); return s; };
    for (int i = 0; i < size; i++)
        if (lower(catalog[i].brand) == lower(text)) return catalog[i].brand;
    return "";
}

// Admin only: asks for a product ID until it exists (0 cancels). Returns nullptr if cancelled.
// Wrong IDs and wrong typing share one counter, so it is 3 failed tries in total.
static Product* askForProduct(const string& prompt) {
    int attempts = 0;
    while (true) {
        int id = input::readInt(prompt, 0, INT_MAX, attempts);
        if (id == 0) return nullptr;
        BSTNode* result = bst::searchBST(globals::bstRoot, id);   // Lab 6: O(log n) lookup
        if (result) return result->product;
        input::invalidEntry("Invalid input. Product ID not found. Try again.", attempts);
    }
}

// Admin: add a brand-new product. The catalog array grows automatically when it is full.
// Typing 0 at the first question goes back (returns false, nothing is added).
static bool addNewProduct(Product*& catalog, int& size, int& capacity) {
    Product item;
    item.id = 0;
    for (int i = 0; i < size; i++) if (catalog[i].id > item.id) item.id = catalog[i].id;
    item.id++;
    item.name = input::readText("Product name (0 to go back): ");
    if (item.name == "0") return false;
    item.category = input::readText("Category: ");
    string brand = input::readText("Brand: ");
    string existing = findBrand(catalog, size, brand);
    item.brand = existing.empty() ? brand : existing;
    item.price = input::readDouble("Price (PHP): ", 0.01);
    item.stockQuantity = input::readInt("Starting stock (0-10000): ", 0, 10000);

    catalog = inventory::addProductToCatalog(catalog, size, capacity, item);
    inventory::rebuildIndexes(catalog, size);   // the array may have moved, so the tree is rebuilt
    history::recordProductAdded(&catalog[size - 1]);   // so Undo can take it out again
    cout << "Added product #" << item.id << ":\n";
    display::displayHeader();
    display::displayProduct(&catalog[size - 1]);
    display::displayFooter();
    return true;
}

// ---------------------------------------------------------------- customer screens

static void viewCatalog(Product* catalog, int size) {
    term::newScreen(display::tableWidth());
    display::displayCatalog(catalog, size);
    display::displayStockLegend();
    term::pause();
}

// Filter by brand: the brands are shown in a table and the customer types just a keyword
static void brandFilter(Product* catalog, int size) {
    term::newScreen(display::tableWidth());
    cout << term::centered("--- Filter by Brand ---") << "\n\n";
    banner::showBrandTable();
    cout << "\n";

    vector<string> matched(size);
    int count = 0;
    int attempts = 0;
    while (true) {
        string keyword = input::readText(term::centered("Enter a brand keyword (0 to go back): "), attempts);
        if (keyword == "0") return;
        count = searching::matchBrands(catalog, size, keyword, matched.data());
        if (count > 0) break;
        input::invalidEntry("No brand matches '" + keyword + "'. Pick a keyword from the table. Try again.", attempts);
    }

    string names;
    for (int i = 0; i < count; i++) names += (i ? ", " : "") + matched[i];
    cout << "\n--- Filtered by " << names << " ---\n";
    display::displayHeader();
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < count; k++) {
            if (catalog[i].brand == matched[k]) { display::displayProduct(&catalog[i]); break; }
        }
    }
    display::displayFooter();
    term::pause();
}

static void keywordSearch(Product* catalog, int size) {
    term::newScreen(display::tableWidth());
    cout << term::centered("--- Search Products ---") << "\n\n";
    string keyword = input::readText("Enter keyword (0 to go back): ");
    if (keyword == "0") return;
    searching::linearSearchKeyword(catalog, size, keyword);
    term::pause();
}

static void priceFilter(Product* catalog, int size) {
    term::newScreen(display::tableWidth());
    cout << term::centered("--- Filter by Price Range ---") << "\n\n";
    bool back = false;
    double minP = input::readDoubleOrBack("Enter minimum price in PHP (B to go back): ", 0, back);
    if (back) return;
    double maxP;
    int attempts = 0;
    while (true) {
        maxP = input::readDoubleOrBack("Enter maximum price in PHP (B to go back): ", 0, back, attempts);
        if (back) return;
        if (maxP >= minP) break;
        input::invalidEntry("Invalid input. Maximum price must be greater than or equal to the minimum. Try again.", attempts);
    }
    searching::binarySearchPriceFilter(catalog, size, minP, maxP);
    term::pause();
}

static void sortMenu(Product* catalog, int size) {
    while (true) {
        term::newScreen(44);
        cout << term::centered("--- Sort Inventory ---") << "\n";
        cout << "1. Sort by Price (Ascending)\n2. Sort by Stock (Descending)\n3. Sort by Brand (Alphabetical)\n4. Back\n";
        char key = term::menuKey("Selection (1-4): ", "1234");
        if (key == '4') return;

        term::newScreen(display::tableWidth());
        // Sort a copy for display so cart/undo pointers still reference the real products
        Product* sorted = new Product[size];
        for (int i = 0; i < size; i++) sorted[i] = catalog[i];
        if (key == '1') sorting::quickSortByPrice(sorted, 0, size - 1);
        else if (key == '2') sorting::quickSortByStock(sorted, 0, size - 1);
        else sorting::insertionSortByBrand(sorted, size);
        display::displayCatalog(sorted, size);
        display::displayStockLegend();
        delete[] sorted;
        term::pause();   // then back to the sort menu
    }
}

static void bannerScreen() {
    term::newScreen(0);
    banner::showBrandBanner();
    term::pause();
}

// ---------------------------------------------------------------- cart

// The customer types a keyword, picks the product if several match, then chooses a quantity.
// "Add another item?" keeps the customer here, so many items can be added in one go.
static void addToCartFlow(Product* catalog, int size) {
    while (true) {
        term::newScreen(display::pickListWidth());
        cout << term::centered("--- Add Items to Cart ---") << "\n\n";
        display::displayCatalog(catalog, size);
        cout << "\n";

        vector<Product*> matches(size);
        int count = 0;
        int attempts = 0;
        while (true) {
            string keyword = input::readText("Enter a product keyword (0 to go back): ", attempts);
            if (keyword == "0") return;
            count = searching::findProducts(catalog, size, keyword, matches.data());
            if (count > 0) break;
            input::invalidEntry("No product matches '" + keyword + "'. Try again.", attempts);
        }

        Product* chosen = matches[0];
        if (count > 1) {
            cout << "\n";
            display::displayPickList(matches.data(), count);
            cout << "\n";
            int pick = input::readInt("Enter the item number (0 to go back): ", 0, count);
            if (pick == 0) return;
            chosen = matches[pick - 1];
        }

        cout << "\nSelected: ";
        color::setBrandColor(chosen->brand);
        cout << chosen->name;
        color::resetColor();
        cout << " (" << chosen->stockQuantity << " in stock)\n";

        if (chosen->stockQuantity <= 0) {
            cout << "Out of stock.\n";
        } else {
            int qty = input::readInt("Quantity (1-" + to_string(chosen->stockQuantity) + ", 0 to go back): ", 0, chosen->stockQuantity);
            if (qty == 0) return;
            cart::addToCart(chosen, qty);
        }

        cout << "\n";
        if (term::menuKey("Add another item? [Y]es / [N]o: ", "yn") == 'n') return;
    }
}

static void removeFromCartFlow() {
    term::newScreen(display::tableWidth());
    cout << term::centered("--- Remove Item from Cart ---") << "\n\n";
    if (!globals::cartHead) {
        cout << "Shopping cart is empty.\n";
        term::pause();
        return;
    }
    cart::displayCart();
    cout << "\n";
    int number = input::readInt("Enter the item number to remove (0 to go back): ", 0, cart::lineCount());
    if (number == 0) return;

    CartNode* line = cart::lineAt(number);
    int qty = line->quantity;
    if (qty > 1) {
        qty = input::readInt("How many to remove (1-" + to_string(qty) + ", 0 to go back): ", 0, qty);
        if (qty == 0) return;
    }
    cart::removeFromCart(line, qty);
    term::pause();
}

static void cartMenu(Product* catalog, int size) {
    while (true) {
        term::newScreen(46);
        cout << term::centered("--- Shopping Cart ---") << "\n";
        cout << "1. Add items\n2. Remove an item\n3. View cart\n4. Browse cart (Previous / Next)\n5. Back\n";
        char key = term::menuKey("Selection (1-5): ", "12345");
        if (key == '5') return;

        guarded([&] {
            if (key == '1') {
                addToCartFlow(catalog, size);
            } else if (key == '2') {
                removeFromCartFlow();
            } else if (key == '3') {
                term::newScreen(display::tableWidth());
                cout << term::centered("--- Shopping Cart ---") << "\n\n";
                cart::displayCart();
                term::pause();
            } else {
                cart::browseCart();   // draws its own screens
            }
        });
    }
}

// ---------------------------------------------------------------- checkout

static void checkoutMenu() {
    while (true) {
        term::newScreen(50);
        cout << term::centered("--- Checkout Queue ---") << "\n";
        cout << "1. Place order (join the queue)\n2. Process next order (front of the queue)\n3. View queue\n4. Back\n";
        char key = term::menuKey("Selection (1-4): ", "1234");
        if (key == '4') return;

        guarded([&] {
            term::newScreen(display::tableWidth());
            if (key == '1') {
                cout << term::centered("--- Place Order ---") << "\n\n";
                if (globals::cartHead) {
                    cart::displayCart();
                    cout << "\n";
                }
                QueueNode* lastBefore = globals::checkoutQueueTail;
                checkout::enqueueCheckout(globals::cartHead);
                if (globals::checkoutQueueTail != lastBefore) {   // a new order was really placed
                    cout << "\n";
                    checkout::displayCheckoutQueue();   // the new order shows up behind the earlier ones
                }
            } else if (key == '2') {
                checkout::processCheckoutQueue();
            } else {
                checkout::displayCheckoutQueue();
            }
            term::pause();
        });
    }
}

// ---------------------------------------------------------------- admin

static Product* adminMenu(Product* catalog, int& size, int& capacity) {
    while (true) {
        term::newScreen(52);
        cout << term::centered("--- Admin Actions ---") << "\n";
        cout << "1. Undo last action\n2. Redo last undone action\n3. Restock a product\n4. Add a new product (catalog resizes when full)\n5. Back\n";
        char key = term::menuKey("Selection (1-5): ", "12345");
        if (key == '5') return catalog;

        guarded([&] {
            if (key == '1') {
                term::newScreen(0);
                history::undoLastAction(catalog, size, capacity);
            } else if (key == '2') {
                term::newScreen(0);
                history::redoLastAction(catalog, size, capacity);
            } else if (key == '3') {
                term::newScreen(display::tableWidth());
                display::displayCatalog(catalog, size);
                cout << "\n";
                Product* p = askForProduct("Enter Product ID to restock (0 to go back): ");
                if (!p) return;
                int units = input::readInt("Units to add (1-1000, 0 to go back): ", 0, 1000);
                if (units == 0) return;
                history::restockProduct(p, units);
            } else {
                term::newScreen(display::tableWidth());
                cout << term::centered("--- Add a New Product ---") << "\n\n";
                if (!addNewProduct(catalog, size, capacity)) return;   // went back: no pause
            }
            term::pause();
        });
    }
}

// The main menu has 10 options, so "10" needs two digits. It is read as a full line
// (type the number, then Enter), exactly like the sample menu in the project specification.
// The submenus still react to a single key press. No attempt limit here: it simply asks again.
static int readMainChoice() {
    while (true) {
        cout << "Enter selection (1-10): ";
        string line;
        input::readRawLine(line);
        line = input::trimText(line);
        if (!line.empty() && line.size() <= 2 && isdigit((unsigned char)line[0]) &&
            (line.size() == 1 || isdigit((unsigned char)line[1]))) {
            int value = stoi(line);
            if (value >= 1 && value <= 10 && to_string(value) == line) return value;
        }
        cout << "Invalid input. Please enter a whole number from 1 to 10. Try again.\n";
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
    cout << "10. Exit\n";
    cout << "======================================================================\n";
}

int main() {
    term::init();

    int catalogSize = 0, catalogCapacity = 0;
    Product* catalog = inventory::createCatalog(catalogSize, catalogCapacity);   // Lab 1
    inventory::rebuildIndexes(catalog, catalogSize);                              // Lab 6 + Lab 3

    while (true) {
        showMainMenu();
        int choice = readMainChoice();
        if (choice == 10) break;

        guarded([&] {
            switch (choice) {
                case 1: viewCatalog(catalog, catalogSize); break;
                case 2: brandFilter(catalog, catalogSize); break;
                case 3: keywordSearch(catalog, catalogSize); break;
                case 4: priceFilter(catalog, catalogSize); break;
                case 5: cartMenu(catalog, catalogSize); break;
                case 6: checkoutMenu(); break;
                case 7: catalog = adminMenu(catalog, catalogSize, catalogCapacity); break;
                case 8: bannerScreen(); break;
                case 9: sortMenu(catalog, catalogSize); break;
            }
        });
    }

    // ---- Free every dynamic structure (zero memory leaks) ----
    cart::destroyCart();                 // Lab 2
    checkout::destroyCheckoutQueue();    // Lab 5
    history::clearUndo();                // Lab 4
    history::clearRedo();
    bst::destroyBST(globals::bstRoot);   // Lab 6
    globals::bstRoot = nullptr;
    banner::destroyBrandBanner();        // Lab 3
    delete[] catalog;                    // Lab 1
    term::newScreen(0);
    cout << "Thank you for shopping at BlitzShop. Goodbye!\n\n";
    term::shutdown();
    return 0;
}
