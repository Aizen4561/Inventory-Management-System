# Inventory Management System (C Language)

A menu-driven console mini project developed in standard C and mapped to the supplied Unit I, Unit II and Unit III C programming syllabus.

## Features

1. Add a new product with ID, name, category, supplier, quantity, reorder level and price.
2. Update product details.
3. Stock in: record new purchases or received stock.
4. Stock out: record sales or stock removal, with quantity validation.
5. Search for a product using its ID.
6. Display all active products.
7. Generate a low-stock report.
8. Delete a product record (soft delete).
9. View inventory summary, total units and total stock value.
10. View stock movement history.
11. Save records permanently using file handling.

    
## 🎨 Inventory Management System – Flowchart

The following flowchart represents the complete working process of the Inventory Management System, including product management, stock updates, searching, reports and file handling.

![Inventory Management System Flowchart](inventory-flowchart.png)

    








## 📸 Project Screenshots

### 1. Inventory Management System – Main Screen
![Main Screen](Screenshot%202026-11-03%20135448.png)

### 2. Inventory Management System – Output
![Project Output](Screenshot%202026-11-03%20135729.png)


## Project files

- `main.c` — complete C source code
- `README.md` — project description and instructions
- `PROJECT_REPORT.md` — editable college project report
- `Makefile` — GCC build instructions
- `.gitignore` — ignores generated files

## Requirements

- GCC or another C99-compatible compiler
- Windows with MinGW-w64, or Linux/macOS with GCC/Clang

## Compile and run

### Windows (MinGW)

```bat
gcc -std=c99 -Wall -Wextra -pedantic main.c -o inventory.exe
inventory.exe
```

### Linux / macOS

```bash
gcc -std=c99 -Wall -Wextra -pedantic main.c -o inventory
./inventory
```

Optional custom inventory data filename:

```bash
./inventory store_inventory.dat
```

## Data files

- `inventory.dat` stores product records in binary format.
- `stock_history.txt` stores readable stock movement entries.

These files are created automatically in the program's working directory. Keep them if you want to retain your records between runs. Do not edit `inventory.dat` manually.

## Syllabus mapping

| Syllabus concept | Project implementation |
|---|---|
| Program structure, compilation and execution | `main()` and build commands |
| Variables and data types | `int`, `double`, `char`, `size_t`, `time_t` |
| Constants and preprocessor directives | `#define` macros and header files |
| Input and output | `printf()`, `puts()`, `fgets()` |
| Operators and type conversion | Arithmetic, relational, logical and assignment operators |
| Decision control | `if`, `else`, `switch` |
| Looping constructs | `for`, `while`, `do-while` |
| One-dimensional arrays | Character arrays and dynamic product array |
| Array traversal | Loops over inventory records |
| Searching | Recursive product-ID search |
| Strings | `strlen()`, `strcmp()`, `strcpy()`, `snprintf()` |
| Pointers | Product pointers, array pointers and file pointers |
| Dynamic memory allocation | `realloc()` and `free()` |
| Functions | Separate functions for each inventory operation |
| Recursion | `searchRecursive()` |
| Structures | `Product` and `Inventory` |
| Unions | `ProductInfo` union |
| File handling | `fopen()`, `fread()`, `fwrite()`, `fprintf()`, `fgets()`, `fclose()` |
| Command-line arguments | Optional data filename through `argv[1]` |

## Suggested project demonstration

1. Add three products with different IDs and quantities.
2. Search for one product by ID.
3. Add stock to that product.
4. Record a sale using Stock Out.
5. Try to remove more stock than available to show validation.
6. Generate the low-stock report.
7. Display the inventory summary.
8. View stock movement history.
9. Exit and restart to demonstrate saved records.

## Academic note and limitations

This is an educational project, not a production inventory platform. It uses a simple local binary file, has no login system or multi-user locking, and is not designed for concurrent users or critical business records. The low-stock report uses each product's reorder level. No real business data should be stored in this demonstration application.
