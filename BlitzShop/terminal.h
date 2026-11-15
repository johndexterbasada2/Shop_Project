#pragma once
// Terminal helpers: centered output, clear screen, single key input, colors.
// Works on Windows (windows.h / conio.h) and on Linux/macOS (ANSI + termios).
#include <string>

using namespace std;

namespace term {

enum Color { DEFAULT = 0, BLUE, GREEN, RED, YELLOW, CYAN, MAGENTA };

// Call once at the start of main. Every cout line after this is centered.
void init();
// Call before the program ends (restores normal cout and the default color).
void shutdown();

// Clears the screen and picks how the next output is centered.
//   blockWidth > 0 : the whole block is centered as one piece (columns stay aligned)
//   blockWidth == 0: every line is centered on its own (good for short messages)
void newScreen(int blockWidth);

// Pads a heading so it sits in the middle of the current block (block mode only).
string centered(const string& text);

// Number of spaces that center a text of this width in the current block.
int centerPad(int textWidth);

// Call after the user pressed Enter, so the next line gets its margin again.
void lineStartReset();

// One key press, no Enter needed, nothing is echoed. Arrow/function keys return 0.
int readKey();

// Prints the prompt and waits until the user presses one of the keys in `valid`
// (lowercase letters/digits). Returns that key in lowercase. Wrong keys are ignored.
char menuKey(const string& prompt, const string& valid);

// "Press any key to continue..."
void pause();

// Used by console_color.cpp. The color is applied in order with the text.
void setColor(Color c);

}  // namespace term
