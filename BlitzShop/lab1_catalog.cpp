// Lab 1 - Dynamic Allocation: product catalog
#include "lab1_catalog.h"

// All prices are in Philippine pesos (PHP).
Product* createCatalog() {
    Product* catalog = new Product[CATALOG_SIZE]{
        {101, "PlayStation 5 Console", "Consoles", "Sony", 31749.37, 10},
        {102, "Spider-Man 2 (PS5)", "Games", "Sony", 4444.37, 25},
        {103, "Xbox Series X Console", "Consoles", "Microsoft", 31749.37, 8},
        {104, "Halo Infinite (Xbox)", "Games", "Microsoft", 3809.37, 30},
        {105, "Nintendo Switch OLED", "Consoles", "Nintendo", 22224.37, 15},
        {106, "Mario Kart 8 Deluxe", "Games", "Nintendo", 3809.37, 40},
        {107, "Pokemon TCG: 151 ETB", "Pokemon Cards", "The Pokemon Company", 3174.37, 12},
        {108, "Pokemon Booster Bundle", "Pokemon Cards", "The Pokemon Company", 1713.87, 20},
        {109, "Fortnite 2800 V-Bucks", "Gift Cards", "Epic Games", 1269.37, 50},
        {110, "Roblox $10 Gift Card", "Gift Cards", "Roblox Corporation", 635.00, 50}
    };
    return catalog;
}