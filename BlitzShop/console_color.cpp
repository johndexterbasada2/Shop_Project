#include "console_color.h"
#include "terminal.h"

using namespace std;

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

void resetColor() {
    term::setColor(term::DEFAULT);
}