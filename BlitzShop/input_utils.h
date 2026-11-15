#pragma once
// Safe console input helpers. Header-only, so no extra .cpp needs to be compiled.
// Every function keeps asking the SAME question until the answer is valid, but after
// MAX_ATTEMPTS wrong answers in a row it throws TooManyAttempts. The menus catch it
// and go back to the menu.
#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include <climits>
#include "terminal.h"

using namespace std;

namespace input {

const int MAX_ATTEMPTS = 3;

struct TooManyAttempts {};

// Call after a wrong answer: prints the message with the attempts left, or throws on the last one.
inline void invalidEntry(const string& message, int& attempts) {
    attempts++;
    if (attempts >= MAX_ATTEMPTS) throw TooManyAttempts();
    int left = MAX_ATTEMPTS - attempts;
    cout << message << " (" << left << (left == 1 ? " attempt" : " attempts") << " left)\n";
}

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
// `attempts` can be shared by several questions that count as one try (see askForProduct).
inline int readInt(const string& prompt, int minVal, int maxVal, int& attempts) {
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
            invalidEntry("Invalid input. Please enter a whole number from " + to_string(minVal) + " to " + to_string(maxVal) + ". Try again.", attempts);
        else
            invalidEntry("Invalid input. Please enter a whole number. Try again.", attempts);
    }
}

inline int readInt(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    int attempts = 0;
    return readInt(prompt, minVal, maxVal, attempts);
}

// Decimal number >= minVal. Rejects letters, "12abc", blanks, nan/inf.
// If goBack is given, typing B (or b) sets it to true and returns 0.
inline double readDoubleImpl(const string& prompt, double minVal, int& attempts, bool* goBack) {
    while (true) {
        cout << prompt;
        string line;
        readRawLine(line);
        line = trimText(line);

        if (goBack && (line == "b" || line == "B")) {
            *goBack = true;
            return 0;
        }

        char* end = nullptr;
        errno = 0;
        double v = strtod(line.c_str(), &end);
        bool isNumber = !line.empty() && *end == '\0' && errno == 0 &&
                        (isdigit((unsigned char)line.back()) || line.back() == '.') &&
                        line.find_first_of("nNiI") == string::npos;
        if (isNumber && v >= minVal) return v;

        invalidEntry("Invalid input. Please enter a valid number (" + to_string((long long)minVal) + " or higher). Try again.", attempts);
    }
}

inline double readDouble(const string& prompt, double minVal, int& attempts) {
    return readDoubleImpl(prompt, minVal, attempts, nullptr);
}

inline double readDouble(const string& prompt, double minVal = 0.0) {
    int attempts = 0;
    return readDoubleImpl(prompt, minVal, attempts, nullptr);
}

inline double readDoubleOrBack(const string& prompt, double minVal, bool& goBack, int& attempts) {
    return readDoubleImpl(prompt, minVal, attempts, &goBack);
}

inline double readDoubleOrBack(const string& prompt, double minVal, bool& goBack) {
    int attempts = 0;
    return readDoubleImpl(prompt, minVal, attempts, &goBack);
}

// Non-empty text (spaces allowed, e.g. "The Pokemon Company").
inline string readText(const string& prompt, int& attempts) {
    while (true) {
        cout << prompt;
        string line;
        readRawLine(line);
        line = trimText(line);
        if (!line.empty()) return line;
        invalidEntry("Invalid input. This cannot be empty. Try again.", attempts);
    }
}

inline string readText(const string& prompt) {
    int attempts = 0;
    return readText(prompt, attempts);
}

}  // namespace input
