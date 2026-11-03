# BlitzShop

A console-based **Brand-Affiliated E-Commerce Inventory & Search System** written in C++.
It simulates a GameStop-themed store of gaming products (PlayStation, Xbox, Nintendo, Pokémon, Fortnite, Roblox) and demonstrates core **Data Structures and Algorithms** concepts through a menu-driven interface, with each partner brand displayed in its own console color.

> Final laboratory capstone for CS201 – Data Structures and Algorithm Analysis. It combines the data structures and algorithms from **Labs 1 to 8** into one application.

---

## Features

| # | Menu Option | Data Structure / Algorithm | Lab |
|---|-------------|----------------------------|-----|
| 1 | View complete catalog (tabular, color-coded by brand) | Dynamic array | Lab 1 |
| 2 | Filter products by official brand | Linear scan | – |
| 3 | Search products by keyword | Linear search | Lab 7 |
| 4 | Filter products by price range | Quick sort + binary search | Lab 7, 8 |
| 5 | Add item to shopping cart (lookup by product ID) | Binary search tree + doubly linked list | Lab 6, 2 |
| 6 | Process customer checkout queue | Queue (FIFO) | Lab 5 |
| 7 | Admin undo (removes last cart addition) | Stack (LIFO) | Lab 4 |
| 8 | Partner brand banner showcase | Circular linked list | Lab 3 |
| 9 | Sort inventory by price, stock, or brand | Quick sort / Insertion sort | Lab 8 |
| 10 | Exit | – | – |

### Brand color themes

The terminal output uses native `<windows.h>` console colors based on each product's brand:

| Brand | Color |
|-------|-------|
| Sony (PlayStation) | Blue |
| Microsoft (Xbox) | Green |
| Nintendo | Red |
| The Pokemon Company | Yellow |
| Epic Games (Fortnite) | Cyan |
| Roblox Corporation | Default console text |

### Complexity overview

| Operation | Algorithm | Time |
|-----------|-----------|------|
| Product lookup by ID | BST search | O(log n) average, O(n) worst |
| Keyword search | Linear search | O(n) |
| Price range filter | Copy + quick sort, then binary search | O(n log n) average |
| Sort by price / stock | Quick sort | O(n log n) average, O(n²) worst |
| Sort by brand | Insertion sort | O(n²) |
| Add to cart | Doubly linked list tail insert | O(1) |
| Undo | Stack push / pop | O(1) |
| Enqueue / dequeue checkout | Linked-list queue | O(1) |

---

## Project Structure

The program is split into small modules, one per lab, so each part can be read, tested, and debugged on its own. Every module has a `.h` (declarations) and a `.cpp` (implementation); `main.cpp` connects them all.

```
Shop_Project/
├── BlitzShop/
│   ├── main.cpp                   # Entry point: menu loop that wires all modules together
│   ├── models.h                   # All structs (Product, CartNode, BrandNode, StackNode, QueueNode, BSTNode)
│   ├── globals.h / .cpp           # Shared list/stack/queue/BST head pointers
│   ├── console_color.h / .cpp     # setBrandColor, resetColor (Windows console colors)
│   ├── display.h / .cpp           # displayProduct, displayCatalog
│   ├── lab1_catalog.h / .cpp      # Lab 1 - Dynamic allocation: createCatalog
│   ├── lab2_cart.h / .cpp         # Lab 2 - Doubly linked list: addToCart, displayCart, undoLastAction
│   ├── lab3_brand_banner.h / .cpp # Lab 3 - Circular linked list: initBrandBanner, showBrandBanner
│   ├── lab4_undo_stack.h / .cpp   # Lab 4 - Stack (LIFO): pushUndo, popUndo
│   ├── lab5_checkout_queue.h/.cpp # Lab 5 - Queue (FIFO): enqueueCheckout, processCheckoutQueue
│   ├── lab6_bst.h / .cpp          # Lab 6 - Binary search tree: insertBST, searchBST
│   ├── lab7_searching.h / .cpp    # Lab 7 - Searching: linearSearchKeyword, binarySearchPriceFilter
│   └── lab8_sorting.h / .cpp      # Lab 8 - Sorting: quickSortByPrice, quickSortByStock, insertionSortByBrand
├── Makefile
├── build.bat
└── README.md
```

---

## Requirements

- **Windows** (the program uses `<windows.h>` for colored console output)
- A C++17 compiler such as **MinGW-w64 `g++`** (also works with MSYS2 or Code::Blocks' bundled MinGW)

---

## Build & Run

### Option 1: `build.bat` (simplest)

Double-click `build.bat`, or run it from a terminal:

```bat
build.bat
```

It compiles every file in `BlitzShop/` into `BlitzShop.exe` and launches it.

### Option 2: Makefile

```bash
make        # build
make run    # build and run
make clean  # remove BlitzShop.exe
```

### Option 3: Manual compile

```bash
g++ -std=c++17 -g BlitzShop/*.cpp -o BlitzShop.exe
./BlitzShop.exe
```

### Debugging tips

- Compile with `-g` (already included above) and open `BlitzShop.exe` in GDB or the VS Code debugger.
- Because each feature lives in its own file, set breakpoints directly in the module you are testing, e.g. `lab6_bst.cpp` for lookup problems or `lab5_checkout_queue.cpp` for order processing.
- Shared state (cart, undo stack, queue, BST root) is declared in `globals.h`, so you can watch those variables from any file.

---

## Default Catalog

| ID | Product | Category | Brand | Price | Stock |
|----|---------|----------|-------|-------|-------|
| 101 | PlayStation 5 Console | Consoles | Sony | $499.99 | 10 |
| 102 | Spider-Man 2 (PS5) | Games | Sony | $69.99 | 25 |
| 103 | Xbox Series X Console | Consoles | Microsoft | $499.99 | 8 |
| 104 | Halo Infinite (Xbox) | Games | Microsoft | $59.99 | 30 |
| 105 | Nintendo Switch OLED | Consoles | Nintendo | $349.99 | 15 |
| 106 | Mario Kart 8 Deluxe | Games | Nintendo | $59.99 | 40 |
| 107 | Pokemon TCG: 151 ETB | Pokemon Cards | The Pokemon Company | $49.99 | 12 |
| 108 | Pokemon Booster Bundle | Pokemon Cards | The Pokemon Company | $26.99 | 20 |
| 109 | Fortnite 2800 V-Bucks | Gift Cards | Epic Games | $19.99 | 50 |
| 110 | Roblox $10 Gift Card | Gift Cards | Roblox Corporation | $10.00 | 50 |

---

## Usage Example

```
Enter selection: 5
Enter Product ID to add: 105
Added to cart: Nintendo Switch OLED

Enter selection: 6

--- Processing Order ---
Purchased: Nintendo Switch OLED | Remaining Stock: 14
Order Complete. Grand Total: $349.99
```

---

## Author

**John Dexter Basada** ([@johndexterbasada2](https://github.com/johndexterbasada2)) – BSCPE 3A, CS201 Data Structures and Algorithm Analysis
