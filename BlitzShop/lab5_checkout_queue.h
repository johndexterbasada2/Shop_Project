// Lab 5 - Queue (FIFO): checkout line
#pragma once
#include "models.h"

void enqueueCheckout(CartNode* cart);   // customer joins the back of the line
void processCheckoutQueue();            // serves the order at the FRONT of the line
void displayCheckoutQueue();            // shows who is waiting, in order
void destroyCheckoutQueue();            // frees every waiting order and its items
