#include "display.h"
#include "console_color.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

namespace display {

static const int W_ID = 5, W_NAME = 30, W_CAT = 15, W_BRAND = 20, W_PRICE = 16, W_STOCK = 10;

int tableWidth(bool showId) {
    return (showId ? W_ID : 0) + W_NAME + W_CAT + W_BRAND + W_PRICE + W_STOCK;
}

// Cuts text that is too long so one long name cannot push the columns out of line
static string fit(const string& s, int width) {
    if ((int)s.size() < width) return s;
    return s.substr(0, width - 4) + "...";
}

static void printSeparator(int width) {
    cout << string(width, '=') << "\n";
}

static void printHeader(const string& firstColumn, int firstWidth) {
    int width = firstWidth + W_NAME + W_CAT + W_BRAND + W_PRICE + W_STOCK;
    printSeparator(width);
    cout << left;
    if (firstWidth > 0) cout << setw(firstWidth) << firstColumn;
    cout << setw(W_NAME) << "Name" << setw(W_CAT) << "Category" << setw(W_BRAND) << "Brand"
         << setw(W_PRICE) << "Price" << "Stock\n";
    printSeparator(width);
}

static void printRow(const string& label, int labelWidth, Product* p) {
    ostringstream price;
    price << "PHP " << fixed << setprecision(2) << p->price;
    color::setBrandColor(p->brand);
    cout << left;
    if (labelWidth > 0) cout << setw(labelWidth) << label;
    cout << setw(W_NAME) << fit(p->name, W_NAME)
         << setw(W_CAT) << fit(p->category, W_CAT)
         << setw(W_BRAND) << fit(p->brand, W_BRAND)
         << setw(W_PRICE) << price.str()
         << "Stock: " << p->stockQuantity
         << "\n";
    color::resetColor();
}

void displayHeader(bool showId) {
    printHeader("ID", showId ? W_ID : 0);
}

void displayFooter(bool showId) {
    printSeparator(tableWidth(showId));
}

void displayProduct(Product* p, bool showId) {
    printRow(showId ? to_string(p->id) : "", showId ? W_ID : 0, p);
}

void displayCatalog(Product* catalog, int size, bool showId) {
    displayHeader(showId);
    for (int i = 0; i < size; i++) displayProduct(&catalog[i], showId);
    displayFooter(showId);
}

void displayPickList(Product** items, int count) {
    printHeader("No.", W_ID);
    for (int i = 0; i < count; i++) printRow(to_string(i + 1) + ".", W_ID, items[i]);
    printSeparator(tableWidth(true));
}

}  // namespace display
