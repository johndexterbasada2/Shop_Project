#include "display.h"
#include "console_color.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

namespace display {

static const int W_NO = 5, W_ID = 9, W_NAME = 30, W_CAT = 15, W_BRAND = 20, W_PRICE = 16, W_STOCK = 10;
static const int W_REST = W_NAME + W_CAT + W_BRAND + W_PRICE + W_STOCK;

int tableWidth(bool showId) {
    return (showId ? W_ID : 0) + W_REST;
}

int pickListWidth() {
    return W_NO + W_ID + W_REST;
}

string fitText(const string& s, int width) {
    if ((int)s.size() < width) return s;
    return s.substr(0, width - 4) + "...";
}

string money(double amount) {
    ostringstream out;
    out << "PHP " << fixed << setprecision(2) << amount;
    return out.str();
}

static void printSeparator(int width) {
    cout << string(width, '=') << "\n";
}

// firstWidth: optional "No." column, idWidth: optional "Prod_ID" column
static void printHeader(int numberWidth, int idWidth) {
    printSeparator(numberWidth + idWidth + W_REST);
    cout << left;
    if (numberWidth > 0) cout << setw(numberWidth) << "No.";
    if (idWidth > 0) cout << setw(idWidth) << "Prod_ID";
    cout << setw(W_NAME) << "Name" << setw(W_CAT) << "Category" << setw(W_BRAND) << "Brand"
         << setw(W_PRICE) << "Price" << "Stock\n";
    printSeparator(numberWidth + idWidth + W_REST);
}

// The number and the Prod_ID are printed in the normal color, the rest of the row in the brand color
static void printRow(const string& number, int numberWidth, Product* p, int idWidth) {
    cout << left;
    if (numberWidth > 0) cout << setw(numberWidth) << number;
    if (idWidth > 0) cout << setw(idWidth) << p->id;
    color::setBrandColor(p->brand);
    cout << setw(W_NAME) << fitText(p->name, W_NAME)
         << setw(W_CAT) << fitText(p->category, W_CAT)
         << setw(W_BRAND) << fitText(p->brand, W_BRAND)
         << setw(W_PRICE) << money(p->price);
    color::setStockColor(p->stockQuantity);   // the stock text goes from green to red as it runs out
    cout << "Stock: " << p->stockQuantity << "\n";
    color::resetColor();
}

void displayHeader(bool showId) {
    printHeader(0, showId ? W_ID : 0);
}

void displayFooter(bool showId) {
    printSeparator(tableWidth(showId));
}

void displayProduct(Product* p, bool showId) {
    printRow("", 0, p, showId ? W_ID : 0);
}

void displayCatalog(Product* catalog, int size, bool showId) {
    displayHeader(showId);
    for (int i = 0; i < size; i++) displayProduct(&catalog[i], showId);
    displayFooter(showId);
}

// One line that explains the stock colors under a catalog table
void displayStockLegend() {
    cout << "Stock level:  ";
    color::setStockColor(21);  cout << "21+";
    color::resetColor();       cout << "   ";
    color::setStockColor(15);  cout << "11-20";
    color::resetColor();       cout << "   ";
    color::setStockColor(8);   cout << "6-10";
    color::resetColor();       cout << "   ";
    color::setStockColor(3);   cout << "1-5";
    color::resetColor();       cout << "   ";
    color::setStockColor(0);   cout << "0 (out of stock)";
    color::resetColor();       cout << "\n";
}

void displayPickList(Product** items, int count) {
    printHeader(W_NO, W_ID);
    for (int i = 0; i < count; i++) printRow(to_string(i + 1) + ".", W_NO, items[i], W_ID);
    printSeparator(pickListWidth());
}

}  // namespace display