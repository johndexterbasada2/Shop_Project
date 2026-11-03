#pragma once
#include "models.h"

// Shared state. Defined once in globals.cpp.
extern CartNode*  cartHead;
extern CartNode*  cartTail;
extern BrandNode* bannerHead;
extern StackNode* undoStackHead;
extern QueueNode* checkoutQueueHead;
extern QueueNode* checkoutQueueTail;
extern BSTNode*   bstRoot;
