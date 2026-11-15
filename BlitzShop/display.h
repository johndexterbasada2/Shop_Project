#pragma once
#include "models.h"

using namespace std;

namespace display {

// Width of the product tables. Use it for term::newScreen so the table is centered.
// showId is only true on admin screens: customers never see the Product ID.
int tableWidth(bool showId = false);

void displayHeader(bool showId = false);               // separator + column titles + separator
void displayFooter(bool showId = false);               // closing separator
void displayProduct(Product* p, bool showId = false);  // one row, in the brand's color
void displayCatalog(Product* catalog, int size, bool showId = false);
void displayPickList(Product** items, int count);      // numbered table (1., 2., ...) for choosing

}  // namespace display
