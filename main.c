/*
 * Inventory Management System - C Language Mini Project
 * Syllabus coverage: variables, operators, conditions, loops, arrays,
 * strings, pointers, dynamic memory, functions, recursion, structures,
 * unions, file handling, macros and command-line arguments.
 *
 * Compile: gcc -std=c99 -Wall -Wextra -pedantic main.c -o inventory
 * Run:     ./inventory [optional_data_filename]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define NAME_LEN 60
#define CATEGORY_LEN 40
#define SUPPLIER_LEN 60
#define INPUT_LEN 256
#define DATA_FILE "inventory.dat"
#define LOG_FILE "stock_history.txt"
#define LOW_STOCK_LIMIT 5

typedef union {
    char supplier[SUPPLIER_LEN]; /* Used for physical products */
    char serviceNote[SUPPLIER_LEN]; /* Alternative metadata example */
} ProductInfo;

typedef struct {
    int id;
    char name[NAME_LEN];
    char category[CATEGORY_LEN];
    ProductInfo info;
    int quantity;
    int reorderLevel;
    double price;
    int active;
} Product;

typedef struct {
    Product *items;
    size_t count;
    size_t capacity;
} Inventory;

static char dataFile[260] = DATA_FILE;

static void line(void) {
    puts("------------------------------------------------------------");
}

static void readText(const char *prompt, char *out, size_t size) {
    for (;;) {
        printf("%s", prompt);
        if (!fgets(out, (int)size, stdin)) {
            out[0] = '\0';
            return;
        }
        size_t len = strlen(out);
        if (len && out[len - 1] == '\n') {
            out[len - 1] = '\0';
            return;
        }
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        if (len < size - 1) return;
        printf("Input too long. Maximum %lu characters.\n",
               (unsigned long)(size - 1));
    }
}

static int readInt(const char *prompt, int min, int max) {
    char input[INPUT_LEN], *end;
    long value;
    for (;;) {
        readText(prompt, input, sizeof(input));
        errno = 0;
        value = strtol(input, &end, 10);
        while (*end == ' ' || *end == '\t') end++;
        if (input[0] && *end == '\0' && errno == 0 &&
            value >= min && value <= max) return (int)value;
        printf("Enter a valid number from %d to %d.\n", min, max);
    }
}

static double readDouble(const char *prompt, double min) {
    char input[INPUT_LEN], *end;
    double value;
    for (;;) {
        readText(prompt, input, sizeof(input));
        errno = 0;
        value = strtod(input, &end);
        while (*end == ' ' || *end == '\t') end++;
        if (input[0] && *end == '\0' && errno == 0 &&
            value >= min && value < 1000000000000.0) return value;
        printf("Enter a valid value (minimum %.2f).\n", min);
    }
}

static void pauseScreen(void) {
    char temp[INPUT_LEN];
    readText("\nPress Enter to continue...", temp, sizeof(temp));
}

static int reserveProducts(Inventory *inv, size_t needed) {
    if (needed <= inv->capacity) return 1;
    size_t cap = inv->capacity ? inv->capacity * 2 : 10;
    while (cap < needed) cap *= 2;
    Product *ptr = (Product *)realloc(inv->items, cap * sizeof(Product));
    if (!ptr) {
        puts("Memory allocation failed.");
        return 0;
    }
    inv->items = ptr;
    inv->capacity = cap;
    return 1;
}

static int loadInventory(Inventory *inv) {
    FILE *fp = fopen(dataFile, "rb");
    if (!fp) {
        if (errno == ENOENT) return 1;
        perror("Unable to open inventory file");
        return 0;
    }
    Product p;
    while (fread(&p, sizeof(Product), 1, fp) == 1) {
        if (!reserveProducts(inv, inv->count + 1)) {
            fclose(fp);
            return 0;
        }
        inv->items[inv->count++] = p;
    }
    if (ferror(fp)) {
        perror("Error reading inventory file");
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

static int saveInventory(const Inventory *inv) {
    FILE *fp = fopen(dataFile, "wb");
    if (!fp) {
        perror("Unable to save inventory");
        return 0;
    }
    for (size_t i = 0; i < inv->count; i++) {
        if (fwrite(&inv->items[i], sizeof(Product), 1, fp) != 1) {
            perror("Error writing inventory file");
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

static void logStock(int id, const char *action, int quantity, int balance) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp) {
        perror("Unable to write stock history");
        return;
    }
    time_t now = time(NULL);
    struct tm *tmNow = localtime(&now);
    char stamp[32] = "Unknown time";
    if (tmNow) strftime(stamp, sizeof(stamp), "%Y-%m-%d %H:%M:%S", tmNow);
    fprintf(fp, "%s | Product ID: %d | %-12s | Quantity: %d | Stock: %d\n",
            stamp, id, action, quantity, balance);
    fclose(fp);
}

/* Recursive search over the product array. */
static int searchRecursive(const Product *items, int count, int id) {
    if (count <= 0) return -1;
    if (items[count - 1].id == id && items[count - 1].active)
        return count - 1;
    return searchRecursive(items, count - 1, id);
}

static int findProduct(const Inventory *inv, int id) {
    if (inv->count > 2147483647U) return -1;
    return searchRecursive(inv->items, (int)inv->count, id);
}

static int idExists(const Inventory *inv, int id) {
    for (size_t i = 0; i < inv->count; i++)
        if (inv->items[i].id == id) return 1;
    return 0;
}

static void showProduct(const Product *p) {
    line();
    printf("Product ID     : %d\n", p->id);
    printf("Product name   : %s\n", p->name);
    printf("Category       : %s\n", p->category);
    printf("Supplier       : %s\n", p->info.supplier);
    printf("Quantity       : %d\n", p->quantity);
    printf("Reorder level  : %d\n", p->reorderLevel);
    printf("Unit price     : Rs. %.2f\n", p->price);
    printf("Stock value    : Rs. %.2f\n", p->price * p->quantity);
    if (p->quantity <= p->reorderLevel)
        puts("Stock status   : LOW STOCK - Reorder required");
    else
        puts("Stock status   : Available");
    line();
}

static void addProduct(Inventory *inv) {
    Product p;
    memset(&p, 0, sizeof(p));
    puts("\n--- ADD NEW PRODUCT ---");
    p.id = readInt("Enter unique product ID: ", 1, 999999999);
    if (idExists(inv, p.id)) {
        puts("This product ID already exists.");
        return;
    }
    readText("Product name: ", p.name, sizeof(p.name));
    readText("Category: ", p.category, sizeof(p.category));
    readText("Supplier name: ", p.info.supplier, sizeof(p.info.supplier));
    p.quantity = readInt("Opening quantity: ", 0, 1000000000);
    p.reorderLevel = readInt("Low-stock / reorder level: ", 0, 1000000000);
    p.price = readDouble("Unit price (Rs.): ", 0.0);
    p.active = 1;

    if (!reserveProducts(inv, inv->count + 1)) return;
    inv->items[inv->count++] = p;
    if (!saveInventory(inv)) {
        inv->count--;
        return;
    }
    logStock(p.id, "NEW PRODUCT", p.quantity, p.quantity);
    puts("Product added successfully.");
}

static void updateProduct(Inventory *inv) {
    puts("\n--- UPDATE PRODUCT ---");
    int id = readInt("Enter product ID: ", 1, 999999999);
    int index = findProduct(inv, id);
    if (index < 0) {
        puts("Product not found.");
        return;
    }
    Product old = inv->items[index];
    char input[INPUT_LEN];
    puts("Leave a field blank to keep its current value.");

    printf("Current name: %s\nNew name: ", old.name);
    readText("", input, sizeof(input));
    if (input[0]) snprintf(inv->items[index].name, NAME_LEN, "%s", input);

    printf("Current category: %s\nNew category: ", old.category);
    readText("", input, sizeof(input));
    if (input[0]) snprintf(inv->items[index].category, CATEGORY_LEN, "%s", input);

    printf("Current supplier: %s\nNew supplier: ", old.info.supplier);
    readText("", input, sizeof(input));
    if (input[0]) snprintf(inv->items[index].info.supplier, SUPPLIER_LEN, "%s", input);

    inv->items[index].price =
        readDouble("Enter new unit price (0 to keep current): ", 0.0);
    if (inv->items[index].price == 0.0)
        inv->items[index].price = old.price;

    if (!saveInventory(inv)) inv->items[index] = old;
    else puts("Product details updated successfully.");
}

static void stockIn(Inventory *inv) {
    puts("\n--- STOCK IN / PURCHASE ---");
    int id = readInt("Enter product ID: ", 1, 999999999);
    int index = findProduct(inv, id);
    if (index < 0) {
        puts("Product not found.");
        return;
    }
    int qty = readInt("Quantity to add: ", 1, 1000000000);
    if (inv->items[index].quantity > 1000000000 - qty) {
        puts("Quantity limit exceeded.");
        return;
    }
    inv->items[index].quantity += qty;
    if (!saveInventory(inv)) {
        inv->items[index].quantity -= qty;
        return;
    }
    logStock(id, "STOCK IN", qty, inv->items[index].quantity);
    printf("Stock updated. Current quantity: %d\n", inv->items[index].quantity);
}

static void stockOut(Inventory *inv) {
    puts("\n--- STOCK OUT / SALE ---");
    int id = readInt("Enter product ID: ", 1, 999999999);
    int index = findProduct(inv, id);
    if (index < 0) {
        puts("Product not found.");
        return;
    }
    int qty = readInt("Quantity to remove: ", 1, 1000000000);
    if (qty > inv->items[index].quantity) {
        puts("Insufficient stock. Sale cannot be completed.");
        return;
    }
    inv->items[index].quantity -= qty;
    if (!saveInventory(inv)) {
        inv->items[index].quantity += qty;
        return;
    }
    logStock(id, "STOCK OUT", qty, inv->items[index].quantity);
    printf("Stock reduced. Current quantity: %d\n", inv->items[index].quantity);
}

static void searchProduct(const Inventory *inv) {
    puts("\n--- SEARCH PRODUCT ---");
    int id = readInt("Enter product ID: ", 1, 999999999);
    int index = findProduct(inv, id);
    if (index < 0) puts("Product not found.");
    else showProduct(&inv->items[index]);
}

static void displayProducts(const Inventory *inv) {
    puts("\n--- COMPLETE INVENTORY ---");
    int found = 0;
    for (size_t i = 0; i < inv->count; i++) {
        if (inv->items[i].active) {
            showProduct(&inv->items[i]);
            found = 1;
        }
    }
    if (!found) puts("No products are currently available.");
}

static void lowStockReport(const Inventory *inv) {
    puts("\n--- LOW STOCK REPORT ---");
    int found = 0;
    for (size_t i = 0; i < inv->count; i++) {
        const Product *p = &inv->items[i];
        if (p->active && p->quantity <= p->reorderLevel) {
            printf("ID: %-6d Name: %-20s Quantity: %-6d Reorder level: %d\n",
                   p->id, p->name, p->quantity, p->reorderLevel);
            found = 1;
        }
    }
    if (!found) puts("No low-stock products. Inventory levels are sufficient.");
}

static void deleteProduct(Inventory *inv) {
    puts("\n--- DELETE PRODUCT ---");
    int id = readInt("Enter product ID: ", 1, 999999999);
    int index = findProduct(inv, id);
    if (index < 0) {
        puts("Product not found.");
        return;
    }
    showProduct(&inv->items[index]);
    char answer[INPUT_LEN];
    readText("Confirm deletion (yes/no): ", answer, sizeof(answer));
    if (strcmp(answer, "yes") != 0 && strcmp(answer, "YES") != 0) {
        puts("Deletion cancelled.");
        return;
    }
    Product old = inv->items[index];
    inv->items[index].active = 0;
    if (!saveInventory(inv)) inv->items[index] = old;
    else {
        logStock(id, "DELETED", 0, old.quantity);
        puts("Product deleted successfully.");
    }
}

static void inventorySummary(const Inventory *inv) {
    int productCount = 0, totalUnits = 0, lowCount = 0;
    double totalValue = 0.0;
    for (size_t i = 0; i < inv->count; i++) {
        const Product *p = &inv->items[i];
        if (!p->active) continue;
        productCount++;
        totalUnits += p->quantity;
        totalValue += p->price * p->quantity;
        if (p->quantity <= p->reorderLevel) lowCount++;
    }
    puts("\n--- INVENTORY SUMMARY ---");
    printf("Number of active products : %d\n", productCount);
    printf("Total units in stock       : %d\n", totalUnits);
    printf("Products needing reorder   : %d\n", lowCount);
    printf("Total inventory value      : Rs. %.2f\n", totalValue);
}

static void viewHistory(void) {
    puts("\n--- STOCK MOVEMENT HISTORY ---");
    FILE *fp = fopen(LOG_FILE, "r");
    if (!fp) {
        puts("No stock history found yet.");
        return;
    }
    char buffer[300];
    while (fgets(buffer, sizeof(buffer), fp)) fputs(buffer, stdout);
    fclose(fp);
}

static void menu(void) {
    line();
    puts("               INVENTORY MANAGEMENT SYSTEM");
    line();
    puts("1.  Add new product");
    puts("2.  Update product details");
    puts("3.  Stock in (purchase / add quantity)");
    puts("4.  Stock out (sale / remove quantity)");
    puts("5.  Search product");
    puts("6.  Display all products");
    puts("7.  Low-stock report");
    puts("8.  Delete product");
    puts("9.  Inventory summary");
    puts("10. View stock movement history");
    puts("0.  Exit");
    line();
}

int main(int argc, char *argv[]) {
    Inventory inventory = {NULL, 0, 0};

    if (argc > 1) {
        if (strlen(argv[1]) >= sizeof(dataFile)) {
            fprintf(stderr, "Data filename is too long.\n");
            return EXIT_FAILURE;
        }
        strcpy(dataFile, argv[1]);
    }

    if (!loadInventory(&inventory)) {
        free(inventory.items);
        return EXIT_FAILURE;
    }

    puts("Welcome to the Inventory Management System!");
    printf("Inventory data file: %s\n", dataFile);

    int choice;
    do {
        menu();
        choice = readInt("Enter your choice: ", 0, 10);
        switch (choice) {
            case 1: addProduct(&inventory); break;
            case 2: updateProduct(&inventory); break;
            case 3: stockIn(&inventory); break;
            case 4: stockOut(&inventory); break;
            case 5: searchProduct(&inventory); break;
            case 6: displayProducts(&inventory); break;
            case 7: lowStockReport(&inventory); break;
            case 8: deleteProduct(&inventory); break;
            case 9: inventorySummary(&inventory); break;
            case 10: viewHistory(); break;
            case 0: puts("Thank you for using the Inventory Management System."); break;
        }
        if (choice != 0) pauseScreen();
    } while (choice != 0);

    free(inventory.items);
    return EXIT_SUCCESS;
}
