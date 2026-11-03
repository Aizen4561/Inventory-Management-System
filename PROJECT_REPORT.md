# INVENTORY MANAGEMENT SYSTEM
## C Language Mini Project Report

**Student Name:** __________________________  
**Roll Number:** ____________________________  
**Course / Section:** ________________________  
**Subject:** C Programming  
**Submitted To:** ___________________________  
**Academic Session:** _______________________

---

## 1. Introduction

The Inventory Management System is a menu-driven console application developed using the C programming language. It is designed to maintain basic product and stock information for a small shop or store. The application allows a user to add and update products, record stock purchases and sales, search products, view all products, identify low-stock items and calculate the total value of the inventory.

The project applies the topics from the C programming syllabus, including operators, decision-making, loops, arrays, strings, pointers, dynamic memory allocation, functions, recursion, structures, unions and file handling.

## 2. Objectives

- To understand the practical application of C programming.
- To maintain product details and stock quantities.
- To record stock received and stock sold.
- To search, update and delete product records.
- To identify products that need to be reordered.
- To calculate total stock units and inventory value.
- To save records permanently using files.

## 3. Software and Hardware Requirements

**Software**
- GCC / MinGW C compiler
- VS Code, Code::Blocks, Dev-C++ or another C editor
- Windows, Linux or macOS

**Hardware**
- Computer or laptop
- Keyboard and display

## 4. Main Modules

1. **Add Product:** Stores a unique product ID, name, category, supplier, opening quantity, reorder level and unit price.
2. **Update Product:** Allows the user to edit product name, category, supplier and price.
3. **Stock In:** Increases available quantity when new stock is received.
4. **Stock Out:** Decreases available quantity when products are sold or removed. It prevents stock from becoming negative.
5. **Search Product:** Finds product details using the product ID.
6. **Display Products:** Shows all active product records.
7. **Low-Stock Report:** Lists products whose quantity is at or below their reorder level.
8. **Delete Product:** Marks a product record as inactive.
9. **Inventory Summary:** Calculates active product count, total units, low-stock count and total stock value.
10. **Stock History:** Displays recorded stock additions, removals and product creation/deletion.

## 5. C Programming Concepts Used

- **Variables and data types:** Store IDs, names, quantities and prices.
- **Operators:** Calculate stock value and compare quantities.
- **Decision statements:** Validate input and choose menu operations.
- **Loops:** Repeat the menu and traverse product records.
- **Arrays and strings:** Store text fields and the collection of products.
- **Pointers:** Access records and manage dynamic memory.
- **Dynamic memory allocation:** `realloc()` expands the product list and `free()` releases memory.
- **Functions:** Divide the application into separate modules.
- **Recursion:** Search product records through a recursive function.
- **Structures:** Group related product and inventory information.
- **Union:** Demonstrate a union for alternative product information.
- **File handling:** Save inventory data and append stock history.
- **Preprocessor directives:** Use header files and macros.

## 6. Algorithm

1. Start the program.
2. Load existing product records from the inventory file.
3. Display the main menu.
4. Accept the user's choice.
5. Execute the selected inventory operation.
6. Validate product IDs and quantities.
7. Save modified inventory records to the data file.
8. Append stock changes to the history file.
9. Return to the main menu.
10. Stop when the user selects Exit and release allocated memory.

## 7. Sample Menu

```text
------------------------------------------------------------
               INVENTORY MANAGEMENT SYSTEM
------------------------------------------------------------
1. Add new product
2. Update product details
3. Stock in (purchase / add quantity)
4. Stock out (sale / remove quantity)
5. Search product
6. Display all products
7. Low-stock report
8. Delete product
9. Inventory summary
10. View stock movement history
0. Exit
------------------------------------------------------------
Enter your choice:
```

## 8. File Handling

The project uses two files:
- `inventory.dat` stores product structures in binary format.
- `stock_history.txt` stores stock movement entries in readable text format.

The files allow inventory information to remain available after the program is closed and restarted.

## 9. Testing

| Test case | Expected result |
|---|---|
| Add a product with a new ID | Product is added |
| Add a duplicate product ID | Duplicate is rejected |
| Stock in a valid quantity | Quantity increases |
| Stock out within available quantity | Quantity decreases |
| Stock out more than available quantity | Operation is rejected |
| Search for an existing ID | Product details are displayed |
| Search for an unknown ID | Product not found |
| Quantity reaches reorder level | Product appears in low-stock report |
| Restart the application | Saved products load again |

## 10. Conclusion

The Inventory Management System demonstrates how fundamental C programming concepts can be combined to create a useful menu-driven application. It provides practical experience in modular programming, data organization, input validation, dynamic memory management and file storage. The project can be extended with barcode support, user authentication, supplier management, sales invoices and database integration.

## 11. Future Scope

- Add user login and role-based access.
- Generate invoices and sales reports.
- Add date-wise purchase and sales reports.
- Add supplier and customer management.
- Add product barcode support.
- Use a database for larger inventories.
- Develop a graphical user interface.

## 12. References

- C programming class notes and supplied syllabus.
- C standard library documentation.
- GCC compiler documentation.
