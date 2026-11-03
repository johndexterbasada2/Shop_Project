#include "display.h"
#include "console_color.h"
#include <iostream>
#include <iomanip>

using namespace std;

void displayProduct(Product* p) {
    setBrandColor(p->brand);
    cout << left << setw(5) << p->id 
         << setw(30) << p->name 
         << setw(15) << p->category 
         << setw(20) << p->brand 
         << "$" << setw(8) << p->price 
         << "Stock: " << p->stockQuantity 
         << "\n";
    resetColor();
}

void displayCatalog(Product* catalog, int size) {
    cout << "\n========================================================================================\n";
    cout << left << setw(5) << "ID" << setw(30) << "Name" << setw(15) << "Category" << setw(20) << "Brand" << setw(9) << "Price" << "Stock\n";
    cout << "========================================================================================\n";
    for (int i = 0; i < size; i++) {
        displayProduct(&catalog[i]);
    }
    cout << "========================================================================================\n";
}
