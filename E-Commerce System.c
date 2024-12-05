#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_PRODUCTS 100
#define MAX_CART 10
#define MAX_ORDERS 50
#define MAX_CANCELLED_ORDERS 50


typedef struct {
    char email[50];
    char password[20];
} Admin;

typedef struct {
    char name[50];
    long long phone;
    char email[50];
    char password[20];
    int pincode;
    bool isLoggedIn;
} User;

typedef struct {
    int id;
    char name[50];
    float price;
    char category[20];
    int stock;
} Product;

typedef struct {
    int userId;
    Product products[MAX_CART];
    int quantities[MAX_CART];
    int count;
} Cart;

typedef struct {
    int orderId;
    int userId;
    Product products[MAX_CART];
    int quantities[MAX_CART];
    int count;
    char status[50];
    char address[100];
    time_t orderTime;
    time_t deliveryTime;
} Order;

typedef struct {
    char cardNumber[17];
    char expiryDate[6];
    char cvv[4];
    char cardHolderName[50];
} PaymentCard;

// Global variables
Admin systemAdmin = {"saisamay086@gmail.com", "cherry@2006"};
User users[MAX_USERS];
int userCount = 0;
Cart carts[MAX_USERS];
Order orders[MAX_ORDERS];
int orderCount = 0;
Order cancelledOrders[MAX_CANCELLED_ORDERS];
int cancelledOrderCount = 0;

// Sample products database
Product products[] = {
    {1, "iPhone 13", 999.99, "Mobiles", 50},
    {2, "Samsung Galaxy S21", 899.99, "Mobiles", 45},
    {3, "MacBook Pro", 1299.99, "Laptops", 30},
    {4, "Dell XPS 15", 1199.99, "Laptops", 25},
    {5, "HP LaserJet", 299.99, "Printers", 20},
    {6, "Epson EcoTank", 249.99, "Printers", 15},
    {7, "The Great Gatsby", 15.99, "Books", 100},
    {8, "To Kill a Mockingbird", 12.99, "Books", 80},
    {9, "Rolex Submariner", 8999.99, "Watches", 5},
    {10, "Apple Watch", 399.99, "Watches", 40}
};

int productCount = sizeof(products) / sizeof(products[0]);
// Pincode validation
bool isValidPincode(int pincode) {
    return (pincode >= 518001 && pincode <= 518543);
}
//admin functions start
void displayAdminMenu() {
    printf("\n=== Admin Dashboard ===\n");
    printf("1. View Registered Users\n");
    printf("2. View User Details\n");
    printf("3. Manage Products\n");
    printf("4. Sign Out\n");
    printf("0. Exit\n");
}

void viewRegisteredUsers() {
    printf("\n=== Registered Users ===\n");
    if (userCount == 0) {
        printf("No users registered yet.\n");
        return;
    }

    printf("Total Users: %d\n", userCount);
    printf("User ID | Name | Email\n");
    printf("--------|------|------\n");
    
    for (int i = 0; i < userCount; i++) {
        printf("%d | %s | %s\n", 
               i, 
               users[i].name, 
               users[i].email);
    }
}

void viewUserDetails(int userId) {
    if (userId < 0 || userId >= userCount) {
        printf("Invalid User ID!\n");
        return;
    }

    User* user = &users[userId];
    
    printf("\n=== User Details ===\n");
    printf("Name: %s\n", user->name);
    printf("Email: %s\n", user->email);
    printf("Phone: %lld\n", user->phone);
    printf("Pincode: %d\n", user->pincode);

    printf("\nOrder History:\n");
    int ordersFound = 0;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].userId == userId) {
            ordersFound++;
            printf("Order ID: %d\n", orders[i].orderId);
            printf("Status: %s\n", orders[i].status);
            printf("Items:\n");
            for (int j = 0; j < orders[i].count; j++) {
                printf("- %s (Quantity: %d)\n", 
                       orders[i].products[j].name, 
                       orders[i].quantities[j]);
            }
            printf("----------------------------------------\n");
        }
    }

    if (ordersFound == 0) {
        printf("No order history found.\n");
    }
}

void manageProducts() {
    int choice;
    
    while (1) {
        printf("\n=== Product Management ===\n");
        printf("1. Add Product\n");
        printf("2. Delete Product\n");
        printf("3. Modify Product\n");
        printf("4. View All Products\n");
        printf("0. Back to Admin Menu\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // Add new product
                if (productCount >= MAX_PRODUCTS) {
                    printf("Maximum product limit reached!\n");
                    break;
                }

                Product newProduct;
                printf("Enter product ID: ");
                scanf("%d", &newProduct.id);

                // Check if ID already exists
                for (int i = 0; i < productCount; i++) {
                    if (products[i].id == newProduct.id) {
                        printf("Product ID already exists!\n");
                        break;
                    }
                }

                printf("Enter product name: ");
                scanf(" %[^\n]s", newProduct.name);

                printf("Enter price: ");
                scanf("%f", &newProduct.price);

                printf("Enter category: ");
                scanf(" %[^\n]s", newProduct.category);

                printf("Enter stock quantity: ");
                scanf("%d", &newProduct.stock);

                products[productCount++] = newProduct;
                printf("Product added successfully!\n");
                break;
            }

            case 2: {
                // Delete product
                int productId;
                printf("Enter product ID to delete: ");
                scanf("%d", &productId);

                for (int i = 0; i < productCount; i++) {
                    if (products[i].id == productId) {
                        // Remove product by shifting array
                        for (int j = i; j < productCount - 1; j++) {
                            products[j] = products[j+1];
                        }
                        productCount--;
                        printf("Product deleted successfully!\n");
                        break;
                    }
                }
                break;
            }

            case 3: {
                // Modify product
                int productId;
                printf("Enter product ID to modify: ");
                scanf("%d", &productId);

                for (int i = 0; i < productCount; i++) {
                    if (products[i].id == productId) {
                        printf("Current Product Details:\n");
                        printf("Name: %s\n", products[i].name);
                        printf("Price: %.2f\n", products[i].price);
                        printf("Category: %s\n", products[i].category);
                        printf("Stock: %d\n", products[i].stock);

                        printf("\nEnter new price (0 to skip): ");
                        float newPrice;
                        scanf("%f", &newPrice);
                        if (newPrice > 0) products[i].price = newPrice;

                        printf("Enter new stock quantity (0 to skip): ");
                        int newStock;
                        scanf("%d", &newStock);
                        if (newStock > 0) products[i].stock = newStock;

                        printf("Product updated successfully!\n");
                        break;
                    }
                }
                break;
            }

            case 4: {
                // View all products
                printf("\n=== All Products ===\n");
                for (int i = 0; i < productCount; i++) {
                    printf("ID: %d | Name: %s | Price: $%.2f | Category: %s | Stock: %d\n", 
                           products[i].id, 
                           products[i].name, 
                           products[i].price, 
                           products[i].category, 
                           products[i].stock);
                }
                break;
            }

            case 0:
                return;

            default:
                printf("Invalid choice!\n");
        }
    }
}

// admin functionalites ended

// Sign up function
void signup() {
    if (userCount >= MAX_USERS) {
        printf("Maximum user limit reached!\n");
        return;
    }

    User newUser;
    printf("\n=== Sign Up ===\n");
    
    printf("Enter your name: ");
    scanf(" %[^\n]s", newUser.name);
    
    printf("Enter your phone number: ");
    scanf("%lld", &newUser.phone);
    
    printf("Enter your email ID: ");
    scanf(" %[^\n]s", newUser.email);
    
    printf("Enter your pincode: ");
    scanf("%d", &newUser.pincode);
    
    if (!isValidPincode(newUser.pincode)) {
        printf("Sorry, we are unable to deliver at your particular location.\n");
        return;
    }
    
    // Check if email already exists
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].email, newUser.email) == 0) {
            printf("Email already exists! Please use a different email.\n");
            return;
        }
    }
    
    printf("Enter your password: ");
    scanf(" %[^\n]s", newUser.password);
    
    newUser.isLoggedIn = false;
    users[userCount] = newUser;
    
    // Initialize cart for new user
    carts[userCount].userId = userCount;
    carts[userCount].count = 0;
    
    userCount++;
    printf("Signup successful! Please sign in to continue.\n");
}

// User Sign_in
int signin(char *email, char *password) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].email, email) == 0 && strcmp(users[i].password, password) == 0) {
            users[i].isLoggedIn = true;
            return i;
        }
    }
    return -1;
}

void displayHomePage() {
    printf("\n=== Home Page ===\n");
    printf("1. Mobiles\n");
    printf("2. Laptops\n");
    printf("3. Printers\n");
    printf("4. Books\n");
    printf("5. Watches\n");
    printf("6. Cart\n");
    printf("7. Order & Status\n");
    printf("8. Cancel Order\n");
    printf("9. Help & Support\n");
    printf("10. Sign Out\n");
    printf("11. Edit Cart\n");
    printf("0. Exit\n");
}

void displayProducts(char *category) {
    printf("\n=== %s ===\n", category);
    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].category, category) == 0) {
            printf("%d. %s - $%.2f (Stock: %d)\n", 
                   products[i].id, products[i].name, 
                   products[i].price, products[i].stock);
        }
    }
}

void addToCart(int userId, int productId, int quantity) {
    int productIndex = -1;
    int cartIndex = -1;

    // Find the product in the product list
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == productId) {
            productIndex = i;
            break;
        }
    }

    if (productIndex == -1) {
        printf("Product not found!\n");
        return;
    }

    // Check if the product is already in the cart
    for (int i = 0; i < carts[userId].count; i++) {
        if (carts[userId].products[i].id == productId) {
            cartIndex = i;
            break;
        }
    }

    // Check stock availability
    if (quantity > products[productIndex].stock) {
        printf("Not enough stock available!\n");
        return;
    }

    if (cartIndex != -1) {
        // Product already exists in cart, update quantity
        int currentQuantity = carts[userId].quantities[cartIndex];
        int totalQuantity = currentQuantity + quantity;

        // Check if total quantity exceeds available stock
        if (totalQuantity > products[productIndex].stock) {
            printf("Not enough stock available!\n");
            return;
        }

        // Update stock and cart quantities
        products[productIndex].stock -= quantity;
        carts[userId].quantities[cartIndex] = totalQuantity;
        printf("Updated cart item quantity successfully!\n");
    } else {
        // New product in cart
        if (carts[userId].count >= MAX_CART) {
            printf("Cart is full!\n");
            return;
        }

        // Update stock and add to cart
        products[productIndex].stock -= quantity;
        carts[userId].products[carts[userId].count] = products[productIndex];
        carts[userId].quantities[carts[userId].count] = quantity;
        carts[userId].count++;
        
        printf("Added to cart successfully!\n");
    }
}


void viewCart(int userId) {
    printf("\n=== Your Cart ===\n");
    float total = 0;
    
    if (carts[userId].count == 0) {
        printf("Your cart is empty!\n");
        return;
    }

    for (int i = 0; i < carts[userId].count; i++) {
        printf("%d. %s - Quantity: %d - $%.2f\n",
               i + 1,
               carts[userId].products[i].name,
               carts[userId].quantities[i],
               carts[userId].products[i].price * carts[userId].quantities[i]);
        total += carts[userId].products[i].price * carts[userId].quantities[i];
    }
    printf("Total: $%.2f\n", total);
}

void editCart(int userId) {
    if (carts[userId].count == 0) {
        printf("Your cart is empty!\n");
        return;
    }

    viewCart(userId);

    int itemIndex, newQuantity;
    printf("Enter the item number to edit (1-%d): ", carts[userId].count);
    scanf("%d", &itemIndex);

    if (itemIndex < 1 || itemIndex > carts[userId].count) {
        printf("Invalid item number!\n");
        return;
    }

    // Adjust index to zero-based
    itemIndex--;

    // Find the original product in the product list
    int productIndex = -1;
    for (int i = 0; i < productCount; i++) {
        if (carts[userId].products[itemIndex].id == products[i].id) {
            productIndex = i;
            break;
        }
    }

    if (productIndex == -1) {
        printf("Product not found!\n");
        return;
    }

    printf("Current quantity: %d\n", carts[userId].quantities[itemIndex]);
    printf("Enter new quantity (0 to remove item): ");
    scanf("%d", &newQuantity);

    if (newQuantity == 0) {
        // Restore stock for the removed item
        products[productIndex].stock += carts[userId].quantities[itemIndex];
        
        // Remove item by shifting array
        for (int i = itemIndex; i < carts[userId].count - 1; i++) {
            carts[userId].products[i] = carts[userId].products[i+1];
            carts[userId].quantities[i] = carts[userId].quantities[i+1];
        }
        carts[userId].count--;
        printf("Item removed from cart!\n");
    } else {
        // Check stock availability
        int currentQuantity = carts[userId].quantities[itemIndex];
        
        if (newQuantity > currentQuantity) {
            // Want to increase quantity
            int additionalQuantity = newQuantity - currentQuantity;
            
            if (additionalQuantity > products[productIndex].stock) {
                printf("Not enough stock available!\n");
                return;
            }
            
            // Update stock and cart quantity
            products[productIndex].stock -= additionalQuantity;
            carts[userId].quantities[itemIndex] = newQuantity;
        } else {
            // Want to decrease quantity
            int stockToRestore = currentQuantity - newQuantity;
            
            // Restore stock
            products[productIndex].stock += stockToRestore;
            carts[userId].quantities[itemIndex] = newQuantity;
        }
        
        printf("Cart item updated successfully!\n");
    }
}

void placeOrder(int userId) {
    if (carts[userId].count == 0) {
        printf("Cart is empty! Cannot place order.\n");
        return;
    }

    printf("\n=== Order Summary ===\n");
    float total = 0;
    for (int i = 0; i < carts[userId].count; i++) {
        printf("%s - Quantity: %d - $%.2f\n",
               carts[userId].products[i].name,
               carts[userId].quantities[i],
               carts[userId].products[i].price * carts[userId].quantities[i]);
        total += carts[userId].products[i].price * carts[userId].quantities[i];
    }
    printf("\nTotal Amount: $%.2f\n", total);

    Order newOrder;
    newOrder.orderId = orderCount + 1;
    newOrder.userId = userId;
    newOrder.count = carts[userId].count;
    
    printf("\nEnter delivery address: ");
    scanf(" %[^\n]s", newOrder.address);

    printf("\n=== Payment Options ===\n");
    printf("1. Online Payment (Debit Card)\n");
    printf("2. Cash on Delivery (COD)\n");
    printf("Enter your choice: ");
    
    int paymentChoice;
    scanf("%d", &paymentChoice);

    bool paymentSuccess = false;

    switch(paymentChoice) {
        case 1: {
            PaymentCard card;
            printf("\n=== Debit Card Payment ===\n");
            
            printf("Enter Card Number (16 digits): ");
            scanf(" %[^\n]s", card.cardNumber);
            
            if (strlen(card.cardNumber) != 16) {
                printf("Invalid card number!\n");
                return;
            }

            printf("Enter Card Holder Name: ");
            scanf(" %[^\n]s", card.cardHolderName);

            printf("Enter Expiry Date (MM/YY): ");
            scanf(" %[^\n]s", card.expiryDate);
            
            printf("Enter CVV (3 digits): ");
            scanf(" %[^\n]s", card.cvv);
            
            if (strlen(card.cvv) != 3) {
                printf("Invalid CVV!\n");
                return;
            }

            printf("\nProcessing payment...\n");
            for(int i = 0; i < 3; i++) {
                printf(".");
                sleep(1);
            }
            printf("\nPayment successful!\n");
            paymentSuccess = true;
            strcpy(newOrder.status, "Order Placed - Will be delivered within 24 hours");
            break;
        }
        
        case 2: {
            printf("\n=== Cash on Delivery ===\n");
            printf("Total amount to be paid: $%.2f\n", total);
            
            printf("Confirm COD order? (1 for Yes, 0 for No): ");
            int confirm;
            scanf("%d", &confirm);
            
            if (confirm == 1) {
                paymentSuccess = true;
                strcpy(newOrder.status, "Order Placed - Will be delivered within 24 hours");
            } else {
                printf("Order cancelled!\n");
                return;
            }
            break;
        }
        
        default:
            printf("Invalid payment option!\n");
            return;
    }

    if (paymentSuccess) {
        // Set order and delivery times
        time(&newOrder.orderTime);
        // assiging the present ordered time to neworder.ordertime
        newOrder.deliveryTime = newOrder.orderTime + (24 * 60 * 60); // 24 hours from now
        
        for (int i = 0; i < carts[userId].count; i++) {
            newOrder.products[i] = carts[userId].products[i];
            newOrder.quantities[i] = carts[userId].quantities[i];
        }
        
        orders[orderCount++] = newOrder;
        carts[userId].count = 0;
        
        printf("\n=== Order Confirmation ===\n");
        printf("Order ID: %d\n", newOrder.orderId);
        printf("Status: %s\n", newOrder.status);
        printf("Delivery Address: %s\n", newOrder.address);
        printf("Total Amount: $%.2f\n", total);
        printf("\nThank you for your order!\n");
        printf("You will receive a confirmation email shortly.\n");
    }
}

void viewOrders(int userId) {
    printf("\n=== Your Orders & Status ===\n");
    int found = 0;
    
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].userId == userId) {
            found = 1;
            printf("\nOrder ID: %d\n", orders[i].orderId);
            printf("Status: %s\n", orders[i].status);
            printf("Items:\n");
            for (int j = 0; j < orders[i].count; j++) {
                printf("- %s (Quantity: %d)\n", 
                       orders[i].products[j].name, 
                       orders[i].quantities[j]);
            }
            printf("Delivery Address: %s\n", orders[i].address);
            
            // Calculate remaining delivery time
            time_t now;
            time(&now);
            double hoursLeft = difftime(orders[i].deliveryTime, now) / 3600.0;
            
            if (hoursLeft > 0 && strcmp(orders[i].status, "Cancelled") != 0) {
                printf("Estimated delivery in: %.1f hours\n", hoursLeft);
            }
            printf("----------------------------------------\n");
        }
    }
    
    if (!found) {
        printf("No orders found!\n");
    }
}

void cancelOrder(int userId) {
    int orderId;
    printf("Enter Order ID to cancel: ");
    scanf("%d", &orderId);
    
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderId == orderId && orders[i].userId == userId) {
            if (strcmp(orders[i].status, "Cancelled") == 0) {
                printf("Order is already cancelled!\n");
                return;
            }
            
            // Restore stock for each product in the order
            for (int j = 0; j < orders[i].count; j++) {
                int productId = orders[i].products[j].id;
                int quantity = orders[i].quantities[j];
                
                // Find the product in the main products array and restore its stock
                for (int k = 0; k < productCount; k++) {
                    if (products[k].id == productId) {
                        products[k].stock += quantity;
                        break;
                    }
                }
            }
            
            strcpy(orders[i].status, "Cancelled");
            cancelledOrders[cancelledOrderCount++] = orders[i];
            printf("Order cancelled successfully!\n");
            return;
        }
    }
    printf("Order not found!\n");
}

void displayHelpSupport() {
    printf("\n====== Help & Support Center ======\n\n");
    
    printf("1. Customer Care Contact Information:\n");
    printf("   - 24/7 Toll-Free: 1-800-123-4567\n");
    printf("   - Premium Support: 1-888-765-4321\n");
    printf("   - International: +1-555-987-6543\n\n");
    
    printf("2. Email Support:\n");
    printf("   - General Inquiries: support@ecommerce.com\n");
    printf("   - Order Issues: orders@ecommerce.com\n");
    printf("   - Technical Support: tech@ecommerce.com\n");
    printf("   - Returns & Refunds: returns@ecommerce.com\n\n");
    
    printf("3. Frequently Asked Questions:\n");
    printf("   Q: How long does delivery take?\n");
    printf("   A: Standard delivery: expected to be delivered in 24 hrs \n");
    
    printf("   Q: What is the return policy?\n");
    printf("   A: NO Returns And Exchange\n\n");
    
    printf("   Q: How do I track my order?\n");
    printf("   A: Use your Order ID in the 'Orders' section\n\n");
    
    printf("4. Office Address:\n");
    printf("   E-Commerce Headquarters\n");
    printf("   123 Commerce Street\n");
    printf("   Digital Valley, Tech State 12345\n\n");
    
    printf("5. Business Hours:\n");
    printf("   Monday - Friday: 9:00 AM - 8:00 PM\n");
    printf("   Saturday: 10:00 AM - 6:00 PM\n");
    printf("   Sunday: Closed (Online support available)\n\n");
    
    printf("6. Social Media Support:\n");
    printf("   - Twitter: @ecommerce_support\n");
    printf("   - Facebook: fb.com/ecommercesupport\n");
    printf("   - Instagram: @ecommerce_official\n\n");
    
    printf("Press Enter to return to main menu...\n");
    getchar();
    getchar();
}

// Signout function
void signout(int userId) {
    users[userId].isLoggedIn = false;
    printf("Signed out successfully!\n");
}

int main() {
    int choice, subchoice, userId = -1, isAdmin = 0;
    char email[50], password[20];

    while (1) {
        if (userId == -1 && !isAdmin) {
            printf("\n=== E-Commerce System ===\n");
            printf("1. Sign Up\n");
            printf("2. Sign In\n");
            printf("3. Admin Sign In\n");
            printf("0. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    signup();
                    break;
                
                case 2:
                    printf("Enter email: ");
                    scanf(" %[^\n]s", email);
                    printf("Enter password: ");
                    scanf(" %[^\n]s", password);
                    userId = signin(email, password);
                    if (userId == -1) {
                        printf("Invalid email or password!\n");
                    } else {
                        printf("Signed in successfully!\n");
                    }
                    break;
                
                case 3:
                    printf("Enter admin email: ");
                    scanf(" %[^\n]s", email);
                    printf("Enter admin password: ");
                    scanf(" %[^\n]s", password);
                    
                    if (strcmp(email, systemAdmin.email) == 0 && 
                        strcmp(password, systemAdmin.password) == 0) {
                        isAdmin = 1;
                        printf("Admin signed in successfully!\n");
                    } else {
                        printf("Invalid admin credentials!\n");
                    }
                    break;
                
                case 0:
                    printf("Thank you for using our system!\n");
                    return 0;
                
                default:
                    printf("Invalid choice!\n");
            }
        } else if (isAdmin) {
            displayAdminMenu();
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    viewRegisteredUsers();
                    break;

                case 2: {
                    int userIdToView;
                    printf("Enter User ID to view details: ");
                    scanf("%d", &userIdToView);
                    viewUserDetails(userIdToView);
                    break;
                }

                case 3:
                    manageProducts();
                    break;

                case 4:
                    isAdmin = 0;
                    printf("Admin signed out successfully!\n");
                    break;

                case 0:
                    printf("Thank you for using our system!\n");
                    return 0;

                default:
                    printf("Invalid choice!\n");
            }
        } else {
            displayHomePage();
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    displayProducts("Mobiles");
                    printf("\nEnter product ID to add to cart (0 to go back): ");
                    scanf("%d", &subchoice);
                    if (subchoice != 0) {
                        int quantity;
                        printf("Enter quantity: ");
                        scanf("%d", &quantity);
                        addToCart(userId, subchoice, quantity);
                    }
                    break;

                case 2:
                    displayProducts("Laptops");
                    printf("\nEnter product ID to add to cart (0 to go back): ");
                    scanf("%d", &subchoice);
                    if (subchoice != 0) {
                        int quantity;
                        printf("Enter quantity: ");
                        scanf("%d", &quantity);
                        addToCart(userId, subchoice, quantity);
                    }
                    break;

                case 3:
                    displayProducts("Printers");
                    printf("\nEnter product ID to add to cart (0 to go back): ");
                    scanf("%d", &subchoice);
                    if (subchoice != 0) {
                        int quantity;
                        printf("Enter quantity: ");
                        scanf("%d", &quantity);
                        addToCart(userId, subchoice, quantity);
                    }
                    break;

                case 4:
                    displayProducts("Books");
                    printf("\nEnter product ID to add to cart (0 to go back): ");
                    scanf("%d", &subchoice);
                    if (subchoice != 0) {
                        int quantity;
                        printf("Enter quantity: ");
                        scanf("%d", &quantity);
                        addToCart(userId, subchoice, quantity);
                    }
                    break;

                case 5:
                    displayProducts("Watches");
                    printf("\nEnter product ID to add to cart (0 to go back): ");
                    scanf("%d", &subchoice);
                    if (subchoice != 0) {
                        int quantity;
                        printf("Enter quantity: ");
                        scanf("%d", &quantity);
                        addToCart(userId, subchoice, quantity);
                    }
                    break;

                case 6:
                    viewCart(userId);
                    printf("\nWould you like to place the order? (1 for Yes, 0 for No): ");
                    scanf("%d", &subchoice);
                    if (subchoice == 1) {
                        placeOrder(userId);
                    }
                    break;

                case 7:
                    viewOrders(userId);
                    break;

                case 8:
                    cancelOrder(userId);
                    break;

                case 9:
                    displayHelpSupport();
                    break;

                case 10:
                    signout(userId);
                    userId = -1;
                    break;
                
                case 11:
    		    editCart(userId);
                    break;

                case 0:
                    printf("Thank you for using our system!\n");
                    return 0;

                default:
                    printf("Invalid choice!\n");
            }
        }
    }
    return 0;
}
