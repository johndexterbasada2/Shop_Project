#include "terminal.h"

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <streambuf>
#include <string>

using namespace std;

#ifdef _WIN32
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <conio.h>
  #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
  #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
  #endif
#else
  #include <signal.h>
  #include <sys/ioctl.h>
  #include <termios.h>
  #include <unistd.h>
#endif

namespace term {

namespace {

const char kColorMarker = '\x01';   // inside the text stream: next byte is a color id

#ifdef _WIN32
const WORD kWinColor[] = {7, 9, 10, 12, 14, 11, 13, 6, 4};
bool g_vtEnabled = false;
#else
const int kAnsiColor[] = {0, 94, 92, 91, 93, 96, 95, 33, 31};
termios g_savedTermios;
volatile sig_atomic_t g_rawMode = 0;
#endif

int terminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return info.srWindow.Right - info.srWindow.Left + 1;
#else
    winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) return ws.ws_col;
#endif
    return 120;
}

// Counts the characters the user will actually see (skips color markers and UTF-8 tails)
int visibleLength(const string& s) {
    int n = 0;
    for (size_t i = 0; i < s.size(); i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == (unsigned char)kColorMarker) { i++; continue; }
        if ((c & 0xC0) == 0x80) continue;
        n++;
    }
    return n;
}

// Sits between cout and the real output. It holds one line at a time, adds the
// left margin, and applies color changes in the right order.
class CenterBuf : public streambuf {
public:
    explicit CenterBuf(streambuf* real) : real_(real) {}

    bool lineMode = false;
    int blockMargin = 0;
    bool started = false;   // the current line already got its left margin

    void applyColor(int id) {
#ifdef _WIN32
        real_->pubsync();   // text before the color change must reach the console first
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), kWinColor[id]);
#else
        char code[16];
        snprintf(code, sizeof code, "\033[%dm", kAnsiColor[id]);
        real_->sputn(code, (streamsize)strlen(code));
#endif
    }

    void rawWrite(const char* text) { real_->sputn(text, (streamsize)strlen(text)); }
    void rawSync() { real_->pubsync(); }

    void emit() {
        if (pending_.empty()) return;
        int visible = visibleLength(pending_);
        if (visible > 0 && !started) {
            int margin = lineMode ? max(0, (terminalWidth() - visible) / 2) : blockMargin;
            string spaces((size_t)margin, ' ');
            real_->sputn(spaces.data(), (streamsize)spaces.size());
            started = true;
        }
        for (size_t i = 0; i < pending_.size(); i++) {
            if (pending_[i] == kColorMarker && i + 1 < pending_.size()) {
                applyColor(pending_[++i] - '0');
            } else {
                real_->sputc(pending_[i]);
            }
        }
        pending_.clear();
    }

protected:
    int_type overflow(int_type ch) override {
        if (!traits_type::eq_int_type(ch, traits_type::eof())) {
            char c = traits_type::to_char_type(ch);
            if (c == '\n') {
                emit();
                real_->sputc('\n');
                started = false;
            } else {
                pending_.push_back(c);
            }
        }
        return traits_type::not_eof(ch);
    }

    int sync() override {
        emit();
        return real_->pubsync();
    }

private:
    streambuf* real_;
    string pending_;
};

CenterBuf* g_buf = nullptr;
streambuf* g_original = nullptr;
int g_blockWidth = 0;

#ifndef _WIN32
void onInterrupt(int) {
    // Ctrl+C while a key was being read: put the terminal back before leaving
    if (g_rawMode) tcsetattr(STDIN_FILENO, TCSANOW, &g_savedTermios);
    const char reset[] = "\033[0m\n";
    if (write(STDOUT_FILENO, reset, sizeof reset - 1) < 0) {}
    _exit(130);
}
#endif

[[noreturn]] void inputClosed() {
    cout << "\nInput closed. Exiting program.\n";
    shutdown();
    exit(0);
}

void clearTerminal() {
    cout.flush();
#ifdef _WIN32
    if (g_vtEnabled) {
        g_buf->rawWrite("\033[2J\033[3J\033[H");
        g_buf->rawSync();
        return;
    }
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(h, &csbi)) return;
    DWORD cells = (DWORD)csbi.dwSize.X * (DWORD)csbi.dwSize.Y, written;
    COORD home = {0, 0};
    FillConsoleOutputCharacterA(h, ' ', cells, home, &written);
    FillConsoleOutputAttribute(h, csbi.wAttributes, cells, home, &written);
    SetConsoleCursorPosition(h, home);
#else
    g_buf->rawWrite("\033[2J\033[3J\033[H");
    g_buf->rawSync();
#endif
}

}  // namespace

void init() {
    if (g_buf) return;
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(h, &mode))
        g_vtEnabled = SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
#else
    signal(SIGINT, onInterrupt);
#endif
    g_original = cout.rdbuf();
    g_buf = new CenterBuf(g_original);
    cout.rdbuf(g_buf);
}

void shutdown() {
    if (!g_buf) return;
    cout.flush();
    g_buf->applyColor(DEFAULT);
    g_buf->rawSync();
    cout.rdbuf(g_original);
    delete g_buf;
    g_buf = nullptr;
}

void newScreen(int blockWidth) {
    if (!g_buf) return;
    clearTerminal();
    g_blockWidth = blockWidth;
    g_buf->lineMode = (blockWidth <= 0);
    g_buf->blockMargin = max(0, (terminalWidth() - blockWidth) / 2);
    g_buf->started = false;
}

int centerPad(int textWidth) {
    if (g_blockWidth <= 0) return 0;
    return max(0, (g_blockWidth - textWidth) / 2);
}

string centered(const string& text) {
    return string((size_t)centerPad((int)text.size()), ' ') + text;
}

void lineStartReset() {
    if (g_buf) g_buf->started = false;
}

void setColor(Color c) {
    if (g_buf) cout << kColorMarker << (char)('0' + (int)c);
}

int readKey() {
    cout.flush();   // make sure the prompt is on screen first
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 0xE0) { _getch(); return 0; }   // arrow / function key
    if (c == 3) { cout << "\n"; shutdown(); exit(0); }   // Ctrl+C
    return c;
#else
    if (!isatty(STDIN_FILENO)) {
        int c = getchar();
        if (c == EOF) inputClosed();
        return c;
    }
    termios raw;
    tcgetattr(STDIN_FILENO, &g_savedTermios);
    raw = g_savedTermios;
    raw.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    g_rawMode = 1;

    unsigned char c = 0;
    ssize_t n;
    do { n = read(STDIN_FILENO, &c, 1); } while (n < 0 && errno == EINTR);
    int result = (n == 1) ? c : -1;

    if (result == 27) {   // arrow keys send ESC [ A ...: swallow the rest of the sequence
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        unsigned char junk;
        while (read(STDIN_FILENO, &junk, 1) == 1) {}
        result = 0;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &g_savedTermios);
    g_rawMode = 0;
    if (result == -1) inputClosed();
    return result;
#endif
}

char menuKey(const string& prompt, const string& valid) {
    cout << prompt << flush;
    while (true) {
        int c = readKey();
        if (c <= 0 || c > 255) continue;
        char lower = (char)tolower(c);
        if (valid.find(lower) != string::npos) return lower;
    }
}

void pause() {
    cout << "\nPress any key to continue..." << flush;
    readKey();
}

}  // namespace term
