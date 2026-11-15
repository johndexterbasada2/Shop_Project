// Lab 3 - Circular Linked List: partner brand banner
#include "lab3_brand_banner.h"
#include "globals.h"
#include "console_color.h"
#include "terminal.h"
#include <algorithm>
#include <iostream>

using namespace std;

namespace banner {

static int bannerCount = 0;   // number of nodes (the list is circular, so we count instead of looking for nullptr)

void destroyBrandBanner() {
    if (!globals::bannerHead) return;
    BrandNode* curr = globals::bannerHead;
    for (int i = 0; i < bannerCount; i++) {
        BrandNode* next = curr->next;
        delete curr;
        curr = next;
    }
    globals::bannerHead = nullptr;
    bannerCount = 0;
}

void initBrandBanner(Product* catalog, int size) {
    destroyBrandBanner();
    BrandNode* tail = nullptr;
    for (int i = 0; i < size; i++) {
        bool seen = false;
        BrandNode* c = globals::bannerHead;
        for (int k = 0; k < bannerCount; k++, c = c->next) {
            if (c->brandName == catalog[i].brand) { seen = true; break; }
        }
        if (seen) continue;
        BrandNode* node = new BrandNode{catalog[i].brand, nullptr};
        if (!globals::bannerHead) globals::bannerHead = node;
        else tail->next = node;
        tail = node;
        bannerCount++;
    }
    if (tail) tail->next = globals::bannerHead;   // close the circle
}

void showBrandBanner() {
    if (!globals::bannerHead) return;
    cout << "\n--- Featured Partner Brands ---\n";
    BrandNode* curr = globals::bannerHead;
    for (int i = 0; i < bannerCount; i++) {
        cout << "[ ";
        color::setBrandColor(curr->brandName);
        cout << curr->brandName;
        color::resetColor();
        cout << " ] ";
        curr = curr->next;
    }
    cout << "\n";
}

// Boxed table with one brand per row. Brands are centered in their cells, the whole table is
// centered on the screen, and brands added later simply follow at the bottom.
void showBrandTable() {
    if (!globals::bannerHead) return;
    const string title = "OFFICIAL BRANDS";
    int inner = (int)title.size();
    BrandNode* c = globals::bannerHead;
    for (int i = 0; i < bannerCount; i++, c = c->next) inner = max(inner, (int)c->brandName.size());
    inner += 6;   // breathing room on both sides

    string margin((size_t)term::centerPad(inner + 2), ' ');
    string line = "+" + string((size_t)inner, '-') + "+";

    int titleLeft = (inner - (int)title.size()) / 2;
    cout << margin << line << "\n";
    cout << margin << "|" << string((size_t)titleLeft, ' ') << title
         << string((size_t)(inner - titleLeft - (int)title.size()), ' ') << "|\n";
    cout << margin << line << "\n";

    c = globals::bannerHead;
    for (int i = 0; i < bannerCount; i++, c = c->next) {
        int len = (int)c->brandName.size();
        int left = (inner - len) / 2;
        cout << margin << "|" << string((size_t)left, ' ');
        color::setBrandColor(c->brandName);
        cout << c->brandName;
        color::resetColor();
        cout << string((size_t)(inner - left - len), ' ') << "|\n";
    }
    cout << margin << line << "\n";
}

string brandListText() {
    string text;
    BrandNode* curr = globals::bannerHead;
    for (int i = 0; i < bannerCount; i++) {
        if (i > 0) text += ", ";
        text += curr->brandName;
        curr = curr->next;
    }
    return text;
}

}  // namespace banner
