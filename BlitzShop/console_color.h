#pragma once
#include <string>

using namespace std;

namespace color {

void setBrandColor(string brand);
void setGreen();      // green text, used for "stock added" messages
void setStockColor(int stock);   // stock gradient: green (plenty) -> yellow -> orange -> red -> dark red (0)
void resetColor();

}  // namespace color
