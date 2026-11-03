// Lab 1 - Dynamic Allocation: product catalog
#include "lab1_catalog.h"

Product* createCatalog() {
    Product* catalog = new Product[CATALOG_SIZE]{
        {101, "PlayStation 5 Console", "Consoles", "Sony", 499.99, 10},
        {102, "Spider-Man 2 (PS5)", "Games", "Sony", 69.99, 25},
        {103, "Xbox Series X Console", "Consoles", "Microsoft", 499.99, 8},
        {104, "Halo Infinite (Xbox)", "Games", "Microsoft", 59.99, 30},
        {105, "Nintendo Switch OLED", "Consoles", "Nintendo", 349.99, 15},
        {106, "Mario Kart 8 Deluxe", "Games", "Nintendo", 59.99, 40},
        {107, "Pokemon TCG: 151 ETB", "Pokemon Cards", "The Pokemon Company", 49.99, 12},
        {108, "Pokemon Booster Bundle", "Pokemon Cards", "The Pokemon Company", 26.99, 20},
        {109, "Fortnite 2800 V-Bucks", "Gift Cards", "Epic Games", 19.99, 50},
        {110, "Roblox $10 Gift Card", "Gift Cards", "Roblox Corporation", 10.00, 50}
    };
    return catalog;
}
