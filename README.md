### README: E-Commerce Management System

#### Overview
This project implements a console-based **E-Commerce Management System** in C. The system facilitates basic operations for an e-commerce platform, such as user registration, product management, shopping cart functionality, order placement, and admin controls. It is designed to handle multiple users and provides a simple interface for customers and administrators.

---

#### Features
1. **User Management**:
   - User registration (sign-up) with validation.
   - User login and session handling.
   - Password-protected admin access.

2. **Product Management**:
   - View available products by category.
   - Add, modify, or delete products (admin-only functionality).

3. **Cart Management**:
   - Add items to the cart.
   - Edit or remove items from the cart.
   - View total cart value.

4. **Order Management**:
   - Place orders with support for:
     - Cash on Delivery (COD)
     - Online payment via debit card.
   - Track order status and expected delivery time.
   - Cancel orders with automatic stock updates.

5. **Help & Support**:
   - Customer support contact details.
   - FAQs and troubleshooting tips.

6. **Admin Features**:
   - View registered users and their details.
   - Access order history of any user.
   - Manage product inventory.

---

#### How It Works
- **Sign-Up and Login**:
  - Users can register with their details. Unique email validation is implemented.
  - Registered users can log in with their credentials.
  
- **Shopping Workflow**:
  - Users can browse products by categories (e.g., Mobiles, Laptops, etc.).
  - Items can be added to the cart and edited before checkout.
  - Orders can be placed with a delivery address and payment option.
  
- **Admin Interface**:
  - Admins can log in using the predefined credentials in the system.
  - Manage user accounts and product inventory via a menu-driven interface.

---

#### Technology Stack
- **Programming Language**: C
- **Libraries**: 
  - `stdio.h` for input/output operations.
  - `string.h` for string manipulation.
  - `stdbool.h` for Boolean operations.
  - `time.h` for handling time-based functionalities.

---

#### Structure and Key Functions
1. **Data Structures**:
   - `User`: Stores user details (name, phone, email, password, etc.).
   - `Product`: Stores product information (id, name, price, category, stock).
   - `Cart`: Stores items added to the cart by a user.
   - `Order`: Stores details of placed orders.

2. **Admin Functions**:
   - `viewRegisteredUsers()`: Displays all registered users.
   - `manageProducts()`: Allows admin to add, delete, or update products.

3. **User Functions**:
   - `signup()`: Facilitates new user registration.
   - `signin()`: Authenticates a user.

4. **Cart Functions**:
   - `addToCart()`: Adds a product to the user's cart.
   - `viewCart()`: Displays all items in the cart.

5. **Order Functions**:
   - `placeOrder()`: Processes the user's cart into an order.
   - `cancelOrder()`: Cancels an existing order.

6. **Help & Support**:
   - `displayHelpSupport()`: Displays help desk contact information and FAQs.

---
#### Constraints
- Maximum Users: 100
- Maximum Products: 100
- Maximum Cart Items per User: 10
- Maximum Orders: 50

---

#### Future Enhancements
- Implement a database for persistent data storage.
- Add a web interface for better usability.
- Enhance the payment system with secure encryption for card details.
- Support product reviews and ratings.

---
