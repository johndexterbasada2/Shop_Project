#include "console_color.h"
#include "terminal.h"

using namespace std;

namespace color {

// The platform code (Windows API or ANSI codes) lives in terminal.cpp.
void setBrandColor(string brand) {
    if (brand == "Sony") term::setColor(term::BLUE);
    else if (brand == "Microsoft") term::setColor(term::GREEN);
    else if (brand == "Nintendo") term::setColor(term::RED);
    else if (brand == "The Pokemon Company") term::setColor(term::YELLOW);
    else if (brand == "Epic Games") term::setColor(term::CYAN);
    else if (brand == "Roblox Corporation") term::setColor(term::MAGENTA);
    else term::setColor(term::DEFAULT);
}

void setGreen() {
    term::setColor(term::GREEN);
}

// Stock gradient: the fewer units are left, the hotter the color.
//   21 or more = green, 11-20 = yellow, 6-10 = orange, 1-5 = red, 0 = dark red
// Change the limits here to re-tune it.
void setStockColor(int stock) {
    if (stock <= 0) term::setColor(term::DARK_RED);
    else if (stock <= 5) term::setColor(term::RED);
    else if (stock <= 10) term::setColor(term::ORANGE);
    else if (stock <= 20) term::setColor(term::YELLOW);
    else term::setColor(term::GREEN);
}

void resetColor() {
    term::setColor(term::DEFAULT);
}

}  // namespace color