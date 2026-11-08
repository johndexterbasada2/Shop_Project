# BlitzShop

**Brand-Affiliated E-Commerce Inventory & Search Engine System**
A console-based C++ program (CS201 Data Structures & Algorithms final output) that simulates a gaming-products store. Every feature is built on a data structure or algorithm from Labs 1–8.

**Author:** John Dexter Basada, BSCPE 3A

---

## Features

- Browse the full product catalog in a color-coded table (each brand has its own color)
- Brand integration everywhere: the brand shows in the catalog table, the brand filter, keyword search (matches name, category **and brand**) and the partner brand banner
- Shopping cart you can add to, view, and **browse forward and backward** (doubly linked list)
- **Live stock tracking:** adding an item to the cart reduces its stock right away, and Undo returns it
- **Admin Undo and Redo** for cart additions and restocking (two stacks)
- **Checkout queue** that holds several orders and serves them first-come, first-served
- **Dynamic catalog:** an admin can add new products and the catalog array resizes itself (capacity doubles) when full
- Sorting by price, stock, or brand
- **Input validation:** wrong input never crashes or exits the program. It shows an error and asks the same question again
- **Zero memory leaks:** every list, stack, queue, tree and array is freed on exit (checked with AddressSanitizer and LeakSanitizer)

## Data Structures & Algorithms Used

| Lab | File | Concept | Used for |
|---|---|---|---|
| 1 | `lab1_catalog` | Dynamic memory allocation and resizing | Catalog array that grows when full |
| 2 | `lab2_cart` | Doubly linked list | Shopping cart with Next / Previous navigation |
| 3 | `lab3_brand_banner` | Circular linked list | Partner brand banner (built from the catalog's brands) |
| 4 | `lab4_undo_stack` | Two stacks (LIFO) | Admin Undo and Redo |
| 5 | `lab5_checkout_queue` | Queue (FIFO) | Checkout order line |
| 6 | `lab6_bst` | Balanced binary search tree | Product lookup by ID in O(log n) |
| 7 | `lab7_searching` | Linear and binary search | Keyword search, price range filter |
| 8 | `lab8_sorting` | Quick sort and insertion sort | Sort by price, stock, brand |

Other files: `models.h` (structs), `globals.h/.cpp` (shared pointers), `display.h/.cpp` (table output), `console_color.h/.cpp` (brand colors), `input_utils.h` (safe input functions).

## Menu

| # | Option | Notes |
|---|---|---|
| 1 | View Complete Catalog | Shows current stock |
| 2 | Filter by Brand | Not case-sensitive (`sony` works). The brand list comes from the catalog |
| 3 | Search by Keyword | Linear search, not case-sensitive, matches name, category and brand |
| 4 | Filter by Price Range | Binary search, max must be at least min |
| 5 | Add Item to Shopping Cart | Submenu: add item, view cart, browse cart (Next / Previous) |
| 6 | Process Customer Checkout Queue | Submenu: place order, process next order, view queue |
| 7 | Perform Admin Undo Action | Submenu: undo, redo, restock a product, add a new product |
| 8 | Brand Banner | Circular list of partner brands |
| 9 | Sort Inventory Display | Price, stock, or brand (display only) |
| 10 | Exit | Frees all memory, then quits |

## How the Stock Works

1. **Add to cart:** stock goes down by 1 immediately (shown in the catalog table).
2. **Undo:** the item is removed from the cart and the stock goes back up by 1. Undoing a restock removes the added units.
3. **Redo:** re-applies the action that was just undone. Any new action clears the redo history.
4. **Place order (checkout):** the order joins the back of the queue. Stock is not reduced again, and the undo and redo history is cleared because a placed order is final.
5. An item with 0 stock cannot be added to the cart.

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

## Limitations (Downsides)

- **Nothing is saved.** The catalog is hard-coded, so stock, carts, orders and added products reset every time the program restarts. A file or database would be needed for permanent data.
- **Windows only.** Brand colors use `windows.h`, so the program does not compile on Linux or macOS without changes.
- **Console interface only.** There is no GUI.
- **Stock is reserved as soon as an item is added.** An abandoned cart keeps its items out of stock until the program closes. A real store would release them after a timeout.
- **No quantity per item.** Adding the same product twice creates two cart entries, and Undo removes only the most recent addition (LIFO), not an item from the middle.
- **Placing an order clears the undo and redo history**, including earlier restocks.
- **Added brands have no custom color.** A new brand shows in the default console color.
- **Sorting is display-only.** Option 9 sorts a copy, so the catalog table always goes back to ID order.
- **No payment, customer accounts, or editing and deleting products.**

## Possible Improvements

- Save and load the catalog from a file
- Add quantity per cart item and remove-any-item
- Release reserved stock from abandoned carts after a timeout
- Let an admin edit and delete products
- Make the program portable beyond Windows

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
