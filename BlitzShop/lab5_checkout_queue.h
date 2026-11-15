// Lab 5 - Queue (FIFO): checkout line
#pragma once
#include "models.h"

using namespace std;

namespace checkout {

void enqueueCheckout(CartNode* items);   // asks for the customer name, then the order joins the back of the line
void processCheckoutQueue();             // serves the order at the FRONT of the line
void displayCheckoutQueue();             // waiting orders (with their items) and completed orders
void destroyCheckoutQueue();             // frees every waiting order, its items and the completed list

}  // namespace checkout
