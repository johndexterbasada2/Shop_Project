// Lab 8 - Sorting: quick sort and insertion sort
#pragma once
#include "models.h"

int  partitionPrice(Product* arr, int low, int high);
void quickSortByPrice(Product* arr, int low, int high);
int  partitionStock(Product* arr, int low, int high);
void quickSortByStock(Product* arr, int low, int high);
void insertionSortByBrand(Product* arr, int size);
