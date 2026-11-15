#pragma once
// Safe console input helpers. Header-only, so no extra .cpp needs to be compiled.
// Every function keeps asking the SAME question until the answer is valid.
#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include <climits>
#include "terminal.h"

using namespace std;

namespace input {

inline bool readRawLine(string& out) {
    if (!getline(cin, out)) {
        // Input stream closed (Ctrl+Z / Ctrl+D): quit instead of looping forever
        cout << "\nInput closed. Exiting program.\n";
        term::shutdown();
        exit(0);
    }
    term::lineStartReset();   // the user pressed Enter, so the cursor is at the start of a line
    return true;
}

inline string trimText(const string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) a++;
    while (b > a && isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

// Whole number between minVal and maxVal (inclusive). Rejects letters, "5abc", "3.5", blanks.
inline int readInt(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    while (true) {
        cout << prompt;
        string line;
        readRawLine(line);
        line = trimText(line);

        char* end = nullptr;
        errno = 0;
        long v = strtol(line.c_str(), &end, 10);
        bool ok = !line.empty() && *end == '\0' && errno == 0 && v >= minVal && v <= maxVal;
        if (ok) return (int)v;

        if (minVal != INT_MIN && maxVal != INT_MAX)
            cout << "Invalid input. Please enter a whole number from " << minVal << " to " << maxVal << ". Try again.\n";
        else
            cout << "Invalid input. Please enter a whole number. Try again.\n";
    }
}

// Decimal number >= minVal. Rejects letters, "12abc", blanks, nan/inf.
inline double readDouble(const string& prompt, double minVal = 0.0) {
    while (true) {
        cout << prompt;
        string line;
        readRawLine(line);
        line = trimText(line);

        char* end = nullptr;
        errno = 0;
        double v = strtod(line.c_str(), &end);
        bool isNumber = !line.empty() && *end == '\0' && errno == 0 &&
                        (isdigit((unsigned char)line.back()) || line.back() == '.') &&
                        line.find_first_of("nNiI") == string::npos;
        if (isNumber && v >= minVal) return v;

        cout << "Invalid input. Please enter a valid number (" << minVal << " or higher). Try again.\n";
    }
}

// Non-empty text (spaces allowed, e.g. "The Pokemon Company").
inline string readText(const string& prompt) {
    while (true) {
        cout << prompt;
        string line;
        readRawLine(line);
        line = trimText(line);
        if (!line.empty()) return line;
        cout << "Invalid input. This cannot be empty. Try again.\n";
    }
}

}  // namespace input
