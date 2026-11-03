// Lab 8 - Sorting: quick sort and insertion sort
#include "lab8_sorting.h"
#include <utility>

using namespace std;

int partitionPrice(Product* arr, int low, int high) {
    double pivot = arr[high].price;
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].price < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortByPrice(Product* arr, int low, int high) {
    if (low < high) {
        int pi = partitionPrice(arr, low, high);
        quickSortByPrice(arr, low, pi - 1);
        quickSortByPrice(arr, pi + 1, high);
    }
}

int partitionStock(Product* arr, int low, int high) {
    int pivot = arr[high].stockQuantity;
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j].stockQuantity > pivot) { 
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortByStock(Product* arr, int low, int high) {
    if (low < high) {
        int pi = partitionStock(arr, low, high);
        quickSortByStock(arr, low, pi - 1);
        quickSortByStock(arr, pi + 1, high);
    }
}

void insertionSortByBrand(Product* arr, int size) {
    for (int i = 1; i < size; i++) {
        Product key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].brand > key.brand) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
