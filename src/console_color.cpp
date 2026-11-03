#include "console_color.h"
#include <windows.h>

using namespace std;

void setBrandColor(string brand) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (brand == "Sony") SetConsoleTextAttribute(hConsole, 9);
    else if (brand == "Microsoft") SetConsoleTextAttribute(hConsole, 10);
    else if (brand == "Nintendo") SetConsoleTextAttribute(hConsole, 12);
    else if (brand == "The Pokemon Company") SetConsoleTextAttribute(hConsole, 14);
    else if (brand == "Epic Games") SetConsoleTextAttribute(hConsole, 11);
    else SetConsoleTextAttribute(hConsole, 7);
}

void resetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
