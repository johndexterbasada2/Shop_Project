// Lab 3 - Circular Linked List: partner brand banner
#include "lab3_brand_banner.h"
#include "globals.h"
#include "console_color.h"
#include <iostream>

using namespace std;

static int bannerCount = 0;   // number of nodes (the list is circular, so we count instead of looking for nullptr)

void destroyBrandBanner() {
    if (!bannerHead) return;
    BrandNode* curr = bannerHead;
    for (int i = 0; i < bannerCount; i++) {
        BrandNode* next = curr->next;
        delete curr;
        curr = next;
    }
    bannerHead = nullptr;
    bannerCount = 0;
}

void initBrandBanner(Product* catalog, int size) {
    destroyBrandBanner();
    BrandNode* tail = nullptr;
    for (int i = 0; i < size; i++) {
        bool seen = false;
        BrandNode* c = bannerHead;
        for (int k = 0; k < bannerCount; k++, c = c->next) {
            if (c->brandName == catalog[i].brand) { seen = true; break; }
        }
        if (seen) continue;
        BrandNode* node = new BrandNode{catalog[i].brand, nullptr};
        if (!bannerHead) bannerHead = node;
        else tail->next = node;
        tail = node;
        bannerCount++;
    }
    if (tail) tail->next = bannerHead;   // close the circle
}

void showBrandBanner() {
    if (!bannerHead) return;
    cout << "\n--- Featured Partner Brands ---\n";
    BrandNode* curr = bannerHead;
    for (int i = 0; i < bannerCount; i++) {
        cout << "[ ";
        setBrandColor(curr->brandName);
        cout << curr->brandName;
        resetColor();
        cout << " ] ";
        curr = curr->next;
    }
    cout << "\n";
}

string brandListText() {
    string text;
    BrandNode* curr = bannerHead;
    for (int i = 0; i < bannerCount; i++) {
        if (i > 0) text += ", ";
        text += curr->brandName;
        curr = curr->next;
    }
    return text;
}
