#pragma once
// Safe console input helpers. Header-only, so no extra .cpp needs to be compiled.
// Every function keeps asking the SAME question until the answer is valid.
#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include <climits>

inline bool readRawLine(std::string& out) {
    if (!std::getline(std::cin, out)) {
        // Input stream closed (Ctrl+Z / Ctrl+D): quit instead of looping forever
        std::cout << "\nInput closed. Exiting program.\n";
        std::exit(0);
    }
    return true;
}

inline std::string trimText(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace((unsigned char)s[a])) a++;
    while (b > a && std::isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

// Whole number between minVal and maxVal (inclusive). Rejects letters, "5abc", "3.5", blanks.
inline int readInt(const std::string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    while (true) {
        std::cout << prompt;
        std::string line;
        readRawLine(line);
        line = trimText(line);

        char* end = nullptr;
        errno = 0;
        long v = std::strtol(line.c_str(), &end, 10);
        bool ok = !line.empty() && *end == '\0' && errno == 0 && v >= minVal && v <= maxVal;
        if (ok) return (int)v;

        if (minVal != INT_MIN && maxVal != INT_MAX)
            std::cout << "Invalid input. Please enter a whole number from " << minVal << " to " << maxVal << ". Try again.\n";
        else
            std::cout << "Invalid input. Please enter a whole number. Try again.\n";
    }
}

// Decimal number >= minVal. Rejects letters, "12abc", blanks, nan/inf.
inline double readDouble(const std::string& prompt, double minVal = 0.0) {
    while (true) {
        std::cout << prompt;
        std::string line;
        readRawLine(line);
        line = trimText(line);

        char* end = nullptr;
        errno = 0;
        double v = std::strtod(line.c_str(), &end);
        bool isNumber = !line.empty() && *end == '\0' && errno == 0 &&
                        (std::isdigit((unsigned char)line.back()) || line.back() == '.') &&
                        line.find_first_of("nNiI") == std::string::npos;
        if (isNumber && v >= minVal) return v;

        std::cout << "Invalid input. Please enter a valid number (" << minVal << " or higher). Try again.\n";
    }
}

// Non-empty text (spaces allowed, e.g. "The Pokemon Company").
inline std::string readText(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        readRawLine(line);
        line = trimText(line);
        if (!line.empty()) return line;
        std::cout << "Invalid input. This cannot be empty. Try again.\n";
    }
}
