# BlitzShop

**Brand-Affiliated E-Commerce Inventory & Search Engine System**
A console-based C++ program (CS201 Data Structures & Algorithms final output) that simulates a gaming-products store. Every feature is built on a data structure or algorithm from Labs 1–8.

**Author:** John Dexter Basada, BSCPE 3A

---

## Features

- Browse the full product catalog in a color-coded table (each brand has its own color)
- Filter products by brand, search by keyword, and filter by price range
- Add items to a shopping cart, and undo the last addition
- **Live stock tracking:** adding an item to the cart reduces its stock in the catalog right away, and **Undo returns the stock**
- Checkout queue that processes orders in first-come, first-served order
- Sort the inventory by price, stock, or brand
- **Input validation:** wrong input never crashes or exits the program. It shows an error and asks the same question again

## Data Structures & Algorithms Used

| Lab | File | Concept | Used for |
|---|---|---|---|
| 1 | `lab1_catalog` | Dynamic memory allocation | Product catalog (10 products) |
| 2 | `lab2_cart` | Doubly linked list | Shopping cart |
| 3 | `lab3_brand_banner` | Circular linked list | Partner brand banner |
| 4 | `lab4_undo_stack` | Stack (LIFO) | Undo last cart action |
| 5 | `lab5_checkout_queue` | Queue (FIFO) | Checkout order line |
| 6 | `lab6_bst` | Binary search tree | Product lookup by ID |
| 7 | `lab7_searching` | Linear & binary search | Keyword search, price range filter |
| 8 | `lab8_sorting` | Quick sort & insertion sort | Sort by price, stock, brand |

Other files: `models.h` (structs), `globals.h/.cpp` (shared pointers), `display.h/.cpp` (table output), `console_color.h/.cpp` (brand colors), `input_utils.h` (safe input functions).

## Menu

| # | Option | Notes |
|---|---|---|
| 1 | View Complete Catalog | Shows current stock |
| 2 | Filter by Brand | Not case-sensitive (`sony` works) |
| 3 | Search by Keyword | Linear search, spaces allowed |
| 4 | Filter by Price Range | Binary search, max must be at least min |
| 5 | Add Item to Cart | Enter Product ID (101–110), `0` to cancel |
| 6 | Process Checkout Queue | Completes the order and shows the total |
| 7 | Undo | Removes the last cart item and restores its stock |
| 8 | Brand Banner | Shows partner brands |
| 9 | Sort Inventory Display | Price, stock, or brand (display only) |
| 10 | Exit | |

## How the Stock Works

1. **Add to cart:** stock goes down by 1 immediately (shown in the catalog table).
2. **Undo:** the item is removed from the cart and the stock goes back up by 1.
3. **Checkout:** the order is final, so stock is not reduced again and the undo history is cleared (finished orders cannot be undone).
4. An item with 0 stock cannot be added to the cart.

## Input Validation

All input is read as full lines, so mistakes are caught instead of breaking the program.

| Prompt | Accepted input |
|---|---|
| Menu selection | Whole number 1–10 |
| Brand | A real catalog brand |
| Keyword | Any non-empty text |
| Price range | Non-negative numbers, max ≥ min |
| Product ID | An existing ID, or `0` to cancel |
| Sort option | 1, 2, or 3 |

Invalid input (letters, blanks, `5abc`, out-of-range numbers) prints `Invalid input... Try again.` and repeats the same prompt.

## Requirements

- A C++ compiler with C++11 or newer (g++ from MSYS2/MinGW recommended)
- **Windows** (brand colors use `windows.h`)

## Build and Run

All `.cpp` files must be compiled together, so do not use "build active file" in VS Code. Open a terminal in the project folder and run:

```
g++ -std=c++17 *.cpp -o blitzshop
./blitzshop
```

In PowerShell, run it with `.\blitzshop.exe`.

If `*.cpp` does not work in your terminal, list the files instead:

```
g++ -std=c++17 main.cpp console_color.cpp display.cpp globals.cpp lab1_catalog.cpp lab2_cart.cpp lab3_brand_banner.cpp lab4_undo_stack.cpp lab5_checkout_queue.cpp lab6_bst.cpp lab7_searching.cpp lab8_sorting.cpp -o blitzshop
```

## Sample Catalog

| ID | Product | Brand | Price (PHP) | Stock |
|---|---|---|---|---|
| 101 | PlayStation 5 Console | Sony | 31,749.37 | 10 |
| 102 | Spider-Man 2 (PS5) | Sony | 4,444.37 | 25 |
| 103 | Xbox Series X Console | Microsoft | 31,749.37 | 8 |
| 104 | Halo Infinite (Xbox) | Microsoft | 3,809.37 | 30 |
| 105 | Nintendo Switch OLED | Nintendo | 22,224.37 | 15 |
| 106 | Mario Kart 8 Deluxe | Nintendo | 3,809.37 | 40 |
| 107 | Pokemon TCG: 151 ETB | The Pokemon Company | 3,174.37 | 12 |
| 108 | Pokemon Booster Bundle | The Pokemon Company | 1,713.87 | 20 |
| 109 | Fortnite 2800 V-Bucks | Epic Games | 1,269.37 | 50 |
| 110 | Roblox $10 Gift Card | Roblox Corporation | 635.00 | 50 |
