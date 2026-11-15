#pragma once
#include "models.h"

using namespace std;

namespace display {

// Width of the product tables. Use it for term::newScreen so the table is centered.
int tableWidth(bool showId = true);
int pickListWidth();                                    // the numbered table is a bit wider

void displayHeader(bool showId = true);                 // separator + column titles + separator
void displayFooter(bool showId = true);                 // closing separator
void displayProduct(Product* p, bool showId = true);    // one row: Prod_ID in plain color, the rest in the brand's color
void displayCatalog(Product* catalog, int size, bool showId = true);
void displayPickList(Product** items, int count);       // numbered table (1., 2., ...) for choosing

string fitText(const string& s, int width);             // cuts long text so columns stay aligned
string money(double amount);                            // "PHP 1234.50"

}  // namespace display
