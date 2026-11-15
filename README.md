# BlitzShop

**Brand-Affiliated E-Commerce Inventory & Search Engine System**
A console-based C++ program (CS201 Data Structures & Algorithms final output) that simulates a gaming-products store. Every feature is built on a data structure or algorithm from Labs 1–8.

**Author:** John Dexter Basada, BSCPE 3A

---

## Features

- Browse the full product catalog in a color-coded table (each brand has its own color). **The Prod_ID column is shown in the normal color while the rest of the row uses the brand color**
- **Centered screen:** every menu and table is centered in the terminal, and the screen is cleared each time you pick an option
- **Menus:** the main menu follows the project specification (options 1–9 and **10. Exit**), so you type the number and press Enter. Every submenu reacts to a single key press (no Enter). Text and number prompts use Enter
- **Green stock messages:** restocking (and redoing a restock) shows its message in green
- Brand integration everywhere: the brand shows in the catalog table, the brand filter, keyword search (matches name, category **and brand**) and the partner brand banner
- Shopping cart (doubly linked list) with **keyword add**, **quantity**, **add several items in one go**, **remove an item**, every item shown in **one table**, and **browse forward and backward** (the current row is marked with `>`)
- **Live stock tracking:** adding an item to the cart reduces its stock right away, and removing it from the cart returns it
- **Admin Undo and Redo** (two stacks) for **restocking** and **added products**. Customer cart actions are not on these stacks: the cart has its own Remove option, so an admin Undo never touches a customer's cart
- **Checkout queue** that holds several orders, serves them first-come, first-served, and shows every waiting order (customer and items) plus the completed orders
- **Dynamic catalog:** an admin can add new products and the catalog array resizes itself (capacity doubles) when full
- Sorting by price, stock, or brand
- **Back options:** the first prompt of every screen accepts `0` (or `B` for the price range) to go back, so an accidental key press is harmless
- **Input validation:** wrong input never crashes the program. It shows an error with the attempts left. **After 3 wrong answers in a row the program goes back to the menu**
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

**Namespaces:** every module has its own namespace, so calls show where they come from (for example `history::restockProduct`, `cart::addToCart`, `globals::cartHead`).

| File | Namespace |
|---|---|
| `globals` | `globals` |
| `lab1_catalog` | `inventory` |
| `lab2_cart` | `cart` |
| `lab3_brand_banner` | `banner` |
| `lab4_undo_stack` | `history` |
| `lab5_checkout_queue` | `checkout` |
| `lab6_bst` | `bst` |
| `lab7_searching` | `searching` |
| `lab8_sorting` | `sorting` |
| `display` | `display` |
| `console_color` | `color` |
| `terminal` | `term` |
| `input_utils` | `input` |

`models.h` (the structs) stays in the global namespace. The code uses `using namespace std;`, so there are no `std::` prefixes.

Other files: `models.h` (structs), `globals.h/.cpp` (shared pointers), `display.h/.cpp` (table output), `console_color.h/.cpp` (brand and green colors), `terminal.h/.cpp` (centering, clear screen, single key input), `input_utils.h` (safe input functions).

## Menu

| # | Option | Notes |
|---|---|---|
| 1 | View Complete Catalog | Shows Prod_ID and current stock |
| 2 | Filter by Brand | The brands are shown in a centered table (new brands follow at the bottom). Type just a keyword (`son`, `nin`, `pokemon`), not case-sensitive |
| 3 | Search by Keyword | Linear search, not case-sensitive. Matches parts of words in the name, category and brand (`box` finds Xbox, `ps5` finds the PS5 console). A number must match a whole word, so `2` finds Spider-Man 2 but not Fortnite 2800 |
| 4 | Filter by Price Range | Binary search, max must be at least min |
| 5 | Add Item to Shopping Cart | Submenu: add items (by keyword, with quantity), remove an item, view cart, browse cart (Next / Previous) |
| 6 | Process Customer Checkout Queue | Submenu: place order (asks the customer name), process next order, view queue (waiting and completed orders) |
| 7 | Perform Admin Undo Action | Submenu: undo, redo, restock a product, add a new product. |
| 8 | Brand Banner | Circular list of partner brands, shown in the same boxed table as the brand filter |
| 9 | Sort Inventory Display | Price, stock, or brand (display only). After a result it returns to the sort menu |
| 10 | Exit | Frees all memory, then quits |

## How the Stock Works

1. **Add to cart:** stock goes down by the quantity immediately (shown in the catalog table). The same product is kept as one cart line with a quantity.
2. **Remove from cart:** the units go back to stock.
3. **Undo (admin):** reverses the latest admin action: a restock, or an added product (the product leaves the catalog and the brand table). Undo of a restock is refused if those units are already in carts, and undo of an added product is refused while it is in the cart or in a waiting order.
4. **Redo:** re-applies the action that was just undone. Any new action clears the redo history.
5. **Place order (checkout):** the order joins the back of the queue. Stock is not reduced again, and the admin undo and redo history is kept.
6. An item with 0 stock cannot be added to the cart.

## Input Validation

All input is read as full lines, so mistakes are caught instead of breaking the program.

| Prompt | Accepted input |
|---|---|
| Main menu | A whole number from 1 to 10, then Enter. Anything else shows an error and asks again |
| Submenus | One key press (invalid keys are ignored) |
| Brand | A keyword that matches a catalog brand |
| Keyword | Any non-empty text. In the cart, `0` goes back |
| Quantity | 1 up to the stock left |
| Cart item number | A number shown in the cart table, or `0` to go back |
| Customer name | Any non-empty text |
| Price range | Non-negative numbers, max ≥ min |
| Product ID (restock) | An existing ID, or `0` to go back |

Invalid input (letters, blanks, `5abc`, out-of-range numbers, a keyword that matches nothing) prints an error with the attempts left and repeats the same prompt. **After 3 failed attempts in a row the program shows `Too many invalid attempts` and goes back to the menu.**

## Final Output Criteria Checklist

How this project covers each part of the CS201 Final Output requirements.

**Product model and brands.** `struct Product` has `id`, `name`, `category`, `brand`, `price`, `stockQuantity`. The catalog uses six official partner brands (Sony, Microsoft, Nintendo, The Pokemon Company, Epic Games, Roblox Corporation), more than the required 3–4.

**Lab integration (35%).**

| Lab | Required use | Where it is in this project |
|---|---|---|
| 1 Dynamic Allocation | Dynamic resizing / catalog memory | Menu 7 > Add a new product: the catalog array doubles when full (`lab1_catalog`) |
| 2 Lists | Shopping cart navigation | Menu 5: doubly linked cart, Browse with Next / Previous (`lab2_cart`) |
| 3 Circular List | Continuous brand showcase | Menu 8: circular list of partner brands (`lab3_brand_banner`) |
| 4 Stack (LIFO) | Admin Undo / Redo | Menu 7: two stacks (`lab4_undo_stack`) |
| 5 Queue (FIFO) | Checkout and order processing | Menu 6: order queue (`lab5_checkout_queue`) |
| 6 BST | O(log n) product ID search | Product ID lookup (restock) uses a balanced BST (`lab6_bst`) |
| 7 Searching | Linear keyword vs binary price filter | Menu 3 (linear) and menu 4 (binary) (`lab7_searching`) |
| 8 Sorting | Insertion / Quick sort for brand and price | Menu 9: quick sort (price, stock), insertion sort (brand) (`lab8_sorting`) |

**Functional correctness (25%).** The main menu matches the specification (1–9, 10 Exit). Brand filtering is menu 2, the checkout queue is menu 6 (place, process, view), and stock updates live in the catalog table (add to cart, remove, restock, undo and redo all change it).

**Brand affiliation (15%).** The brand appears in the catalog table, the brand filter (menu 2), keyword search (name, category and brand), the cart table (it has a Brand column), the checkout queue (items shown in their brand color), and the partner banner. Each brand has its own color.

**Memory management and safety (15%).** Every dynamic structure is freed on exit (cart, queue, completed orders, undo and redo stacks, BST, banner, catalog). Edge cases are handled: empty cart or queue, out-of-stock items, invalid or out-of-range input, empty text, and a closed input stream.

**Code quality and documentation (10%).** One module per lab with its own namespace, comments on every module, and this README.

## Requirements

- A C++ compiler with C++17 (g++ from MSYS2/MinGW recommended on Windows)
- Windows, Linux or macOS
- A terminal at least 100 columns wide, so the catalog table does not wrap

## Build and Run

All `.cpp` files must be compiled together, so do not use "build active file" in VS Code. Open a terminal in the project folder and run:

```
g++ -std=c++17 *.cpp -o blitzshop
./blitzshop
```

In PowerShell, run it with `.\blitzshop.exe`.

If `*.cpp` does not work in your terminal, list the files instead:

```
g++ -std=c++17 main.cpp console_color.cpp display.cpp globals.cpp terminal.cpp lab1_catalog.cpp lab2_cart.cpp lab3_brand_banner.cpp lab4_undo_stack.cpp lab5_checkout_queue.cpp lab6_bst.cpp lab7_searching.cpp lab8_sorting.cpp -o blitzshop
```

## Limitations (Downsides)

- **Nothing is saved.** The catalog is hard-coded, so stock, carts, orders and added products reset every time the program restarts. A file or database would be needed for permanent data.
- **Console interface only.** There is no GUI.
- **Stock is reserved as soon as an item is added.** An abandoned cart keeps its items out of stock until the program closes. A real store would release them after a timeout.
- **An added product can only be undone while it is the newest product** (Undo goes in LIFO order), and not while it is in the cart or in a waiting order.
- **Added brands have no custom color.** A new brand shows in the default console color.
- **Sorting is display-only.** Option 9 sorts a copy, so the catalog table always goes back to its original order.
- **No payment, customer accounts, or editing and deleting products.** The customer name is only a label on the order.

## Possible Improvements

- Save and load the catalog from a file
- Let the customer choose the row color used for sorting (for example by brand or by category)
- Release reserved stock from abandoned carts after a timeout
- Let an admin edit and delete products

## Sample Catalog

| ID | Product | Brand | Price (PHP) | Stock |
|---|---|---|---|---|
| 101 | PlayStation 5 (PS5) Console | Sony | 31,749.37 | 10 |
| 102 | Spider-Man 2 (PS5) | Sony | 4,444.37 | 25 |
| 103 | Xbox Series X Console | Microsoft | 31,749.37 | 8 |
| 104 | Halo Infinite (Xbox) | Microsoft | 3,809.37 | 30 |
| 105 | Nintendo Switch OLED | Nintendo | 22,224.37 | 15 |
| 106 | Mario Kart 8 Deluxe | Nintendo | 3,809.37 | 40 |
| 107 | Pokemon TCG: 151 ETB | The Pokemon Company | 3,174.37 | 12 |
| 108 | Pokemon Booster Bundle | The Pokemon Company | 1,713.87 | 20 |
| 109 | Fortnite 2800 V-Bucks | Epic Games | 1,269.37 | 50 |
| 110 | Roblox $10 Gift Card | Roblox Corporation | 635.00 | 50 |
