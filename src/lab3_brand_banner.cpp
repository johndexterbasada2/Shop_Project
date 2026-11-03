// Lab 3 - Circular Linked List: partner brand banner
#include "lab3_brand_banner.h"
#include "globals.h"
#include "console_color.h"
#include <iostream>
#include <string>

using namespace std;

void initBrandBanner() {
    string brands[] = {"Sony", "Microsoft", "Nintendo", "The Pokemon Company", "Epic Games", "Roblox Corporation"};
    BrandNode* prev = nullptr;
    for (int i = 0; i < 6; i++) {
        BrandNode* newNode = new BrandNode{brands[i], nullptr};
        if (!bannerHead) {
            bannerHead = newNode;
        } else {
            prev->next = newNode;
        }
        prev = newNode;
    }
    prev->next = bannerHead; 
}

void showBrandBanner() {
    if (!bannerHead) return;
    cout << "\n--- Featured Partner Brands ---\n";
    BrandNode* curr = bannerHead;
    for (int i = 0; i < 6; i++) {
        cout << "[ ";
        setBrandColor(curr->brandName);
        cout << curr->brandName;
        resetColor();
        cout << " ] ";
        curr = curr->next;
    }
    cout << "\n";
}
