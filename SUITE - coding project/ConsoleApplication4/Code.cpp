// SUITE Online Clothing Store - Console Based

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <cstring>
#include <cctype>


using namespace std;

// Declaring Classes
class User;
class Customer;
class Admin;
class Product;
class CartItem;
class Cart;
class Order;
class Discount;
class Store;

// Declaring Functions
void displayProducts(const vector<Product>& products);
void clearInputBuffer();
int getIntInput();
double getDoubleInput();
void browseProducts(Store& store, Customer* customer);
void manageCart(Store& store, Customer* customer);
void viewOrderHistory(const Customer* customer);
void manageProducts(Store& store);
void manageDiscounts(Store& store);
void generateReports(Store& store);
void runApp();



// =================
// Class Definitions
// =================


// Product Class - Stores product's details
class Product {
private:
    int id;
    string name;
    double price;
    vector<string> sizes;
    vector<string> colours;
    int stock;

public:
    Product(int id, string name, double price, vector<string> sizes, vector<string> colours, int stock)
        : id(id), name(name), price(price), sizes(sizes), colours(colours), stock(stock) {
    }

    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    vector<string> getSizes() const { return sizes; }
    vector<string> getColours() const { return colours; }
    int getStock() const { return stock; }
    void setPrice(double newPrice) { price = newPrice; }
    void setStock(int newStock) { stock = newStock; }

    // Decreases stock
    bool decreaseStock(int quantity) {
        if (stock >= quantity) {
            stock -= quantity;
            return true;
        }
        return false;
    }

    // Increases stock
    void increaseStock(int quantity) {
        stock += quantity;
    }

    // Displays product details
    void display() const {
        cout << left << setw(5) << id << setw(20) << name << "£" << fixed << setprecision(2) << setw(8) << price;
        cout << "Stock: " << setw(5) << stock;

        // Displays sizes
        cout << "Sizes: ";
        for (size_t i = 0; i < sizes.size(); i++) {
            cout << sizes[i];
            if (i < sizes.size() - 1) cout << ", ";
        }

        // Display colours
        cout << " | Colours: ";
        for (size_t i = 0; i < colours.size(); i++) {
            cout << colours[i];
            if (i < colours.size() - 1) cout << ", ";
        }
        cout << endl;
    }
};


class Cart;

// User Class
class User {
protected:
    string username;
    string password;
    string role;

public:
    User(string username, string password, string role)
        : username(username), password(password), role(role) {
    }

    // For inheritance
    virtual ~User() {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getRole() const { return role; }

    virtual void displayMenu() const = 0;
};

// Cart Item class - Stores product in cart
class CartItem {
private:
    Product product;
    int quantity;
    string selectedSize;
    string selectedColor;

public:
    CartItem(Product product, int quantity, string size, string color)
        : product(product), quantity(quantity), selectedSize(size), selectedColor(color) {
    }

    Product getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    string getSelectedSize() const { return selectedSize; }
    string getSelectedColor() const { return selectedColor; }

    // Update quantity
    void updateQuantity(int newQuantity) {
        quantity = newQuantity;
    }

    // Calculate total price
    double getTotalPrice() const {
        return product.getPrice() * quantity;
    }

    // Display cart item
    void display() const {
        cout << left << setw(20) << product.getName()
            << " Size: " << setw(5) << selectedSize
            << " Color: " << setw(10) << selectedColor
            << " Qty: " << setw(3) << quantity
            << " Price: $" << fixed << setprecision(2) << product.getPrice()
            << " Total: $" << fixed << setprecision(2) << getTotalPrice()
            << endl;
    }
};

// Order class to store order details
class Order {
private:
    int id;
    vector<CartItem> items;
    double subtotal;
    double discountAmount;
    string discountCode;
    double total;
    time_t orderDate;

public:

    Order(int id, const vector<CartItem>& items, double subtotal,
        double discountAmount, string discountCode, time_t orderDate)
        : id(id), items(items), subtotal(subtotal),
        discountAmount(discountAmount), discountCode(discountCode),
        total(subtotal - discountAmount), orderDate(orderDate) {
    }

    int getId() const { return id; }
    vector<CartItem> getItems() const { return items; }
    double getSubtotal() const { return subtotal; }
    double getDiscountAmount() const { return discountAmount; }
    string getDiscountCode() const { return discountCode; }
    double getTotal() const { return total; }
    time_t getOrderDate() const { return orderDate; }

    // Display order details
    void display() const {
        cout << "\n==== Order #" << id << " ====" << endl;

        char dateStr[26];
        tm* timeinfo = localtime(&orderDate);
        strftime(dateStr, sizeof(dateStr), "%c", timeinfo);
        cout << "Date: " << dateStr << endl;

        // Display ordered items
        cout << "Items:" << endl;
        for (size_t i = 0; i < items.size(); i++) {
            cout << i + 1 << ". ";
            items[i].display();
        }

        cout << "\nSubtotal: $" << fixed << setprecision(2) << subtotal << endl;

        if (discountAmount > 0) {
            cout << "Discount";
            if (!discountCode.empty()) {
                cout << " (" << discountCode << ")";
            }
            cout << ": -$" << fixed << setprecision(2) << discountAmount << endl;
        }

        cout << "Total: $" << fixed << setprecision(2) << total << endl;
    }
};

// Cart class to manage cart items
class Cart {
private:
    vector<CartItem> items;
    double discountAmount;
    string discountCode;

public:
    Cart() : discountAmount(0.0), discountCode("") {}

    // Add item to cart
    void addItem(CartItem item) {
        // Checks if product already exists with same size and color
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].getProduct().getId() == item.getProduct().getId() &&
                items[i].getSelectedSize() == item.getSelectedSize() &&
                items[i].getSelectedColor() == item.getSelectedColor()) {
                // Updates quantity instead of adding new item
                items[i].updateQuantity(items[i].getQuantity() + item.getQuantity());
                return;
            }
        }

        // Adds new item
        items.push_back(item);
    }

    // Removes item from cart
    bool removeItem(int index) {
        if (index >= 0 && index < static_cast<int>(items.size())) {
            items.erase(items.begin() + index);
            return true;
        }
        return false;
    }

    // Updates item quantity
    bool updateItemQuantity(int index, int quantity) {
        if (index >= 0 && index < static_cast<int>(items.size())) {
            items[index].updateQuantity(quantity);
            return true;
        }
        return false;
    }

    // Gets cart items
    vector<CartItem> getItems() const {
        return items;
    }

    // Clears cart
    void clear() {
        items.clear();
        discountAmount = 0.0;
        discountCode = "";
    }

    // Applys discount
    void applyDiscount(double amount, string code) {
        discountAmount = amount;
        discountCode = code;
    }

    // Removes discount
    void removeDiscount() {
        discountAmount = 0.0;
        discountCode = "";
    }

    // Gets subtotal
    double getSubtotal() const {
        double subtotal = 0.0;
        for (const CartItem& item : items) {
            subtotal += item.getTotalPrice();
        }
        return subtotal;
    }

    // Gets discount amount
    double getDiscountAmount() const {
        return discountAmount;
    }

    // Gets discount code
    string getDiscountCode() const {
        return discountCode;
    }

    // Gets total after discount
    double getTotal() const {
        return getSubtotal() - discountAmount;
    }

    // Checks if cart is empty
    bool isEmpty() const {
        return items.empty();
    }

    // Displays cart
    void display() const {
        if (items.empty()) {
            cout << "Your cart is empty." << endl;
            return;
        }

        cout << "\n==== Your Cart ====" << endl;
        for (size_t i = 0; i < items.size(); i++) {
            cout << i + 1 << ". ";
            items[i].display();
        }

        cout << "\nSubtotal: $" << fixed << setprecision(2) << getSubtotal() << endl;

        if (discountAmount > 0) {
            cout << "Discount (" << discountCode << "): -$" << fixed << setprecision(2) << discountAmount << endl;
        }

        cout << "Total: $" << fixed << setprecision(2) << getTotal() << endl;
    }
};

// Customer class derived from User superclass
class Customer : public User {
private:
    Cart cart;
    vector<Order> orderHistory;

public:
    Customer(string username, string password)
        : User(username, password, "customer") {
    }

    // Gets cart
    Cart& getCart() { return cart; }

    // Gets order history
    const vector<Order>& getOrderHistory() const { return orderHistory; }

    // Adds order to history
    void addOrder(const Order& order) {
        orderHistory.push_back(order);
    }

    // Displays customer menu
    void displayMenu() const override {
        cout << "\n==== Customer Menu ====" << endl;
        cout << "1. Browse Products" << endl;
        cout << "2. View Cart" << endl;
        cout << "3. View Order History" << endl;
        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
    }
};

// Admin class derived from User superclass
class Admin : public User {
public:
    Admin(string username, string password)
        : User(username, password, "admin") {
    }

    // Displays admin menu
    void displayMenu() const override {
        cout << "\n==== Admin Menu ====" << endl;
        cout << "1. Manage Products" << endl;
        cout << "2. Manage Discounts" << endl;
        cout << "3. Generate Reports" << endl;
        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
    }
};

// Discount class 
class Discount {
private:
    string code;
    double amount;
    string type;
    bool active;

public:
    Discount(string code, double amount, string type, bool active = true)
        : code(code), amount(amount), type(type), active(active) {
    }

    string getCode() const { return code; }
    double getAmount() const { return amount; }
    string getType() const { return type; }
    bool isActive() const { return active; }

    // Checks if discount is valid
    bool isValid() const {
        return active;
    }

    void setActive(bool isActive) { active = isActive; }

    // Calculates discount amount based on subtotal
    double calculateDiscount(double subtotal) const {
        if (!isValid()) return 0.0;

        if (type == "percentage") {
            return subtotal * (amount / 100.0);
        }
        else { 
            return (amount > subtotal) ? subtotal : amount;
        }
    }

    // Displays discount details
    void display() const {
        cout << "Code: " << setw(10) << code;

        if (type == "percentage") {
            cout << " Amount: " << amount << "%";
        }
        else {
            cout << " Amount: $" << fixed << setprecision(2) << amount;
        }
        cout << endl;
    }
};

// Store class 
class Store {
private:
    vector<Product> products;
    vector<User*> users;
    vector<Discount> discounts;
    vector<Order> allOrders;
    User* currentUser;
    int nextOrderId;

    // Initialize with sample data
    void initializeSampleData() {
        //  sample products
        products.push_back(Product(1, "T-Shirt", 19.99, { "S", "M", "L", "XL" }, { "White", "Black", "Blue" }, 50));
        products.push_back(Product(2, "Jeans", 49.99, { "28", "30", "32", "34" }, { "Blue", "Black" }, 30));
        products.push_back(Product(3, "Hoodie", 39.99, { "S", "M", "L", "XL" }, { "Gray", "Black", "Red" }, 25));
        products.push_back(Product(4, "Dress Shirt", 29.99, { "S", "M", "L" }, { "White", "Blue", "Pink" }, 40));
        products.push_back(Product(5, "Socks", 9.99, { "One Size" }, { "Black", "White", "Gray" }, 100));

        //  sample users
        users.push_back(new Admin("admin", "admin123"));
        users.push_back(new Customer("john", "john123"));
        users.push_back(new Customer("mary", "mary123"));

        //  sample discounts
        discounts.push_back(Discount("SUMMER20", 20.0, "percentage", true));
        discounts.push_back(Discount("WELCOME10", 10.0, "fixed", true));

        // Initializes next order ID
        nextOrderId = 1;

        currentUser = nullptr;
    }

public:
    Store() {
        initializeSampleData();
    }

    ~Store() {
        for (User* user : users) {
            delete user;
        }
    }

    // Login user
    bool login(string username, string password) {
        for (User* user : users) {
            if (user->getUsername() == username && user->getPassword() == password) {
                currentUser = user;
                return true;
            }
        }
        return false;
    }

    // Register user
    bool registerCustomer(string username, string password) {
        // Check if username already exists
        for (User* user : users) {
            if (user->getUsername() == username) {
                return false;
            }
        }

        // Creates user
        users.push_back(new Customer(username, password));
        return true;
    }

    // Logout user
    void logout() {
        currentUser = nullptr;
    }

    // Gets current user
    User* getCurrentUser() const {
        return currentUser;
    }

    // Gets products
    vector<Product>& getProducts() {
        return products;
    }

    // Gets discounts
    vector<Discount>& getDiscounts() {
        return discounts;
    }

    // Adds product
    void addProduct(const Product& product) {
        products.push_back(product);
    }

    // Removes product
    bool removeProduct(int id) {
        for (size_t i = 0; i < products.size(); i++) {
            if (products[i].getId() == id) {
                products.erase(products.begin() + i);
                return true;
            }
        }
        return false;
    }

    // Finds product by ID
    Product* findProduct(int id) {
        for (size_t i = 0; i < products.size(); i++) {
            if (products[i].getId() == id) {
                return &products[i];
            }
        }
        return nullptr;
    }

    // Adds discount
    void addDiscount(const Discount& discount) {
        discounts.push_back(discount);
    }

    // Removes discount
    bool removeDiscount(const string& code) {
        for (size_t i = 0; i < discounts.size(); i++) {
            if (discounts[i].getCode() == code) {
                discounts.erase(discounts.begin() + i);
                return true;
            }
        }
        return false;
    }

    // Finsd discount by code
    Discount* findDiscount(const string& code) {
        for (size_t i = 0; i < discounts.size(); i++) {
            if (discounts[i].getCode() == code) {
                return &discounts[i];
            }
        }
        return nullptr;
    }

    // Process checkout
    bool processCheckout() {
        // Check if user is logged in and is a customer
        if (!currentUser || currentUser->getRole() != "customer") {
            return false;
        }

        Customer* customer = static_cast<Customer*>(currentUser);

        // Checks if cart is empty
        if (customer->getCart().isEmpty()) {
            return false;
        }

        // Check if products are in stock
        for (const CartItem& item : customer->getCart().getItems()) {
            Product* product = findProduct(item.getProduct().getId());
            if (!product || product->getStock() < item.getQuantity()) {
                return false;
            }
        }

        // Updates stock
        for (const CartItem& item : customer->getCart().getItems()) {
            Product* product = findProduct(item.getProduct().getId());
            product->decreaseStock(item.getQuantity());
        }

        // Creates order
        Order order(
            nextOrderId++,
            customer->getCart().getItems(),
            customer->getCart().getSubtotal(),
            customer->getCart().getDiscountAmount(),
            customer->getCart().getDiscountCode(),
            time(nullptr)
        );

        // Adds order to customer's history
        customer->addOrder(order);

        // Adds order to all orders
        allOrders.push_back(order);

        // Clears cart
        customer->getCart().clear();

        return true;
    }

    // Gets all orders for reports
    vector<Order> getAllOrders() const {
        return allOrders;
    }

    // Generates revenue report by time-frame (daily, monthly, yearly)
    pair<int, double> generateRevenueReport(string timeFrame) const {
        int orderCount = 0;
        double totalRevenue = 0.0;

        time_t now = time(nullptr);
        time_t startTime;

        // Determines start time based on time-frame
        if (timeFrame == "daily") {
            struct tm* timeinfo = localtime(&now);
            timeinfo->tm_hour = 0;
            timeinfo->tm_min = 0;
            timeinfo->tm_sec = 0;
            startTime = mktime(timeinfo);
        }
        else if (timeFrame == "monthly") {
            struct tm* timeinfo = localtime(&now);
            timeinfo->tm_mday = 1;
            timeinfo->tm_hour = 0;
            timeinfo->tm_min = 0;
            timeinfo->tm_sec = 0;
            startTime = mktime(timeinfo);
        }
        else if (timeFrame == "yearly") {
            struct tm* timeinfo = localtime(&now);
            timeinfo->tm_mon = 0;
            timeinfo->tm_mday = 1;
            timeinfo->tm_hour = 0;
            timeinfo->tm_min = 0;
            timeinfo->tm_sec = 0;
            startTime = mktime(timeinfo);
        }
        else {
            startTime = 0;
        }

        // Calculates revenue
        for (const Order& order : allOrders) {
            if (order.getOrderDate() >= startTime) {
                orderCount++;
                totalRevenue += order.getTotal();
            }
        }

        return make_pair(orderCount, totalRevenue);
    }
};

//==================================
// Functions
//==================================

// Function to clear input buffer to prevent inf loop
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to get valid integer input
int getIntInput() {
    int value;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

// Function to get valid double input
double getDoubleInput() {
    double value;
    while (!(cin >> value)) {
        cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

// Function to display products
void displayProducts(const vector<Product>& products) {
    cout << "\n==== Product Catalog ====" << endl;
    cout << left << setw(5) << "ID" << setw(20) << "Name" << setw(9) << "Price" << setw(10) << "Stock" << "Details" << endl;
    cout << string(80, '-') << endl;

    for (const Product& product : products) {
        product.display();
    }
}

// Function for customer browsing products
void browseProducts(Store& store, Customer* customer) {
    while (true) {
        // Displays products
        displayProducts(store.getProducts());

        cout << "\n1. View Product Details" << endl;
        cout << "2. Add to Cart" << endl;
        cout << "3. Back to Menu" << endl;
        cout << "Enter your choice: ";

        int choice = getIntInput();

        switch (choice) {
        case 1: { // View product details
            cout << "Enter product ID: ";
            int productId = getIntInput();

            Product* product = store.findProduct(productId);
            if (product) {
                cout << "\n==== Product Details ====" << endl;
                product->display();
            }
            else {
                cout << "Product not found." << endl;
            }
            break;
        }

        case 2: { // Add to cart
            cout << "Enter product ID: ";
            int productId = getIntInput();

            Product* product = store.findProduct(productId);
            if (!product) {
                cout << "Product not found." << endl;
                break;
            }

            if (product->getStock() <= 0) {
                cout << "Sorry, this product is out of stock." << endl;
                break;
            }

            // Select size
            if (product->getSizes().empty()) {
                cout << "This product has no size options." << endl;
                break;
            }

            cout << "Available sizes: ";
            for (size_t i = 0; i < product->getSizes().size(); i++) {
                cout << product->getSizes()[i];
                if (i < product->getSizes().size() - 1) cout << ", ";
            }
            cout << endl;

            string selectedSize;
            bool validSize = false;

            while (!validSize) {
                cout << "Enter size: ";
                cin >> selectedSize;
                clearInputBuffer();

                for (const string& size : product->getSizes()) {
                    if (size == selectedSize) {
                        validSize = true;
                        break;
                    }
                }

                if (!validSize) {
                    cout << "Invalid size. Please select from available sizes." << endl;
                }
            }

            // Select color
            if (product->getColours().empty()) {
                cout << "This product has no color options." << endl;
                break;
            }

            cout << "Available colors: ";
            for (size_t i = 0; i < product->getColours().size(); i++) {
                cout << product->getColours()[i];
                if (i < product->getColours().size() - 1) cout << ", ";
            }
            cout << endl;

            string selectedColor;
            bool validColor = false;

            while (!validColor) {
                cout << "Enter color: ";
                cin >> selectedColor;
                clearInputBuffer();

                for (const string& color : product->getColours()) {
                    if (color == selectedColor) {
                        validColor = true;
                        break;
                    }
                }

                if (!validColor) {
                    cout << "Invalid color. Please select from available colors." << endl;
                }
            }

            // Select quantity
            int selectedQuantity;
            do {
                cout << "Enter quantity (max " << product->getStock() << "): ";
                selectedQuantity = getIntInput();

                if (selectedQuantity <= 0) {
                    cout << "Quantity must be greater than zero." << endl;
                }
                else if (selectedQuantity > product->getStock()) {
                    cout << "Not enough stock available." << endl;
                }
            } while (selectedQuantity <= 0 || selectedQuantity > product->getStock());

            // Add to cart
            CartItem item(*product, selectedQuantity, selectedSize, selectedColor);
            customer->getCart().addItem(item);

            cout << "Product added to cart successfully!" << endl;
            break;
        }

        case 3: // Back to menu
            return;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Function to run application
void runApp() {
    Store store;
    bool running = true;

    while (running) {
        // Clear screen
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        cout << "==== SUITE Clothing Store ====" << endl;

        User* currentUser = store.getCurrentUser();

        if (!currentUser) {
            // If User not logged in
            cout << "1. Login" << endl;
            cout << "2. Register" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";

            int choice = getIntInput();

            switch (choice) {
            case 1: { // Login
                string username, password;

                cout << "\n==== Login ====" << endl;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                clearInputBuffer();

                if (store.login(username, password)) {
                    cout << "Login successful!" << endl;
                }
                else {
                    cout << "Invalid username or password." << endl;
                }
                break;
            }

            case 2: { // Register
                string username, password;

                cout << "\n==== Register ====" << endl;
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                clearInputBuffer();

                if (store.registerCustomer(username, password)) {
                    cout << "Registration successful! You can now login." << endl;
                }
                else {
                    cout << "Username already exists. Please choose another username." << endl;
                }
                break;
            }

            case 3: // Exit
                running = false;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        }
        else {
            // When User logged in
            cout << "Welcome, " << currentUser->getUsername() << " (" << currentUser->getRole() << ")" << endl;

            // Displays appropriate menu based on user role
            currentUser->displayMenu();

            int choice = getIntInput();

            if (currentUser->getRole() == "customer") {
                // Customer menu
                Customer* customer = static_cast<Customer*>(currentUser);

                switch (choice) {
                case 1: // Browse products
                    browseProducts(store, customer);
                    break;

                case 2: // View cart
                    manageCart(store, customer);
                    break;

                case 3: // View order history
                    viewOrderHistory(customer);
                    break;

                case 4: // Logout
                    store.logout();
                    cout << "Logged out successfully." << endl;
                    break;

                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
            else if (currentUser->getRole() == "admin") {
                // Admin menu
                switch (choice) {
                case 1: // Manage products
                    manageProducts(store);
                    break;

                case 2: // Manage discounts
                    manageDiscounts(store);
                    break;

                case 3: // Generate reports
                    generateReports(store);
                    break;

                case 4: // Logout
                    store.logout();
                    cout << "Logged out successfully." << endl;
                    break;

                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            }
        }
    }

    cout << "Thank you for using SUITE Online Clothing Store!" << endl;
}

// Function to handle admin product management
void manageProducts(Store& store) {
    while (true) {
        // Display products
        displayProducts(store.getProducts());

        cout << "\n==== Product Management ====" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Update Product Price" << endl;
        cout << "3. Update Product Stock" << endl;
        cout << "4. Remove Product" << endl;
        cout << "5. Back to Menu" << endl;
        cout << "Enter your choice: ";

        int choice = getIntInput();

        switch (choice) {
        case 1: { // Add product
            int id;
            string name;
            double price;
            vector<string> sizes;
            vector<string> colors;
            int stock;

            // Get next available ID
            int maxId = 0;
            for (const Product& product : store.getProducts()) {
                if (product.getId() > maxId) {
                    maxId = product.getId();
                }
            }
            id = maxId + 1;

            cout << "Enter product name: ";
            getline(cin, name);

            cout << "Enter price: $";
            price = getDoubleInput();

            cout << "Enter sizes (comma-separated, e.g., S,M,L): ";
            string sizesInput;
            getline(cin, sizesInput);

            // Parse sizes
            size_t pos = 0;
            while ((pos = sizesInput.find(',')) != string::npos) {
                sizes.push_back(sizesInput.substr(0, pos));
                sizesInput.erase(0, pos + 1);
            }
            if (!sizesInput.empty()) {
                sizes.push_back(sizesInput);
            }

            cout << "Enter colors (comma-separated, e.g., Red,Blue,Black): ";
            string colorsInput;
            getline(cin, colorsInput);

            // Parse colors
            pos = 0;
            while ((pos = colorsInput.find(',')) != string::npos) {
                colors.push_back(colorsInput.substr(0, pos));
                colorsInput.erase(0, pos + 1);
            }
            if (!colorsInput.empty()) {
                colors.push_back(colorsInput);
            }

            cout << "Enter stock quantity: ";
            stock = getIntInput();

            if (stock < 0) {
                cout << "Stock cannot be negative." << endl;
                break;
            }

            // Create and add product
            Product newProduct(id, name, price, sizes, colors, stock);
            store.addProduct(newProduct);

            cout << "Product added successfully!" << endl;
            break;
        }

        case 2: { // Update product price
            cout << "Enter product ID: ";
            int productId = getIntInput();

            Product* product = store.findProduct(productId);
            if (!product) {
                cout << "Product not found." << endl;
                break;
            }

            cout << "Current price: $" << fixed << setprecision(2) << product->getPrice() << endl;
            cout << "Enter new price: $";
            double newPrice = getDoubleInput();

            if (newPrice < 0) {
                cout << "Price cannot be negative." << endl;
                break;
            }

            product->setPrice(newPrice);
            cout << "Price updated successfully!" << endl;
            break;
        }

        case 3: { // Update product stock
            cout << "Enter product ID: ";
            int productId = getIntInput();

            Product* product = store.findProduct(productId);
            if (!product) {
                cout << "Product not found." << endl;
                break;
            }

            cout << "Current stock: " << product->getStock() << endl;
            cout << "Enter new stock: ";
            int newStock = getIntInput();

            if (newStock < 0) {
                cout << "Stock cannot be negative." << endl;
                break;
            }

            product->setStock(newStock);
            cout << "Stock updated successfully!" << endl;
            break;
        }

        case 4: { // Remove product
            cout << "Enter product ID to remove: ";
            int productId = getIntInput();

            if (store.removeProduct(productId)) {
                cout << "Product removed successfully!" << endl;
            }
            else {
                cout << "Product not found." << endl;
            }
            break;
        }

        case 5: // Back to menu
            return;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Function to handle admin reports
void generateReports(Store& store) {
    while (true) {
        cout << "\n==== Reports ====" << endl;
        cout << "1. Revenue Report (Daily)" << endl;
        cout << "2. Revenue Report (Monthly)" << endl;
        cout << "3. Revenue Report (Yearly)" << endl;
        cout << "4. Revenue Report (All Time)" << endl;
        cout << "5. Back to Menu" << endl;
        cout << "Enter your choice: ";

        int choice = getIntInput();

        string timeFrame;
        pair<int, double> report;

        switch (choice) {
        case 1: // Daily report
            timeFrame = "daily";
            report = store.generateRevenueReport(timeFrame);

            cout << "\n==== Daily Revenue Report ====" << endl;
            cout << "Orders: " << report.first << endl;
            cout << "Revenue: $" << fixed << setprecision(2) << report.second << endl;
            break;

        case 2: // Monthly report
            timeFrame = "monthly";
            report = store.generateRevenueReport(timeFrame);

            cout << "\n==== Monthly Revenue Report ====" << endl;
            cout << "Orders: " << report.first << endl;
            cout << "Revenue: $" << fixed << setprecision(2) << report.second << endl;
            break;

        case 3: // Yearly report
            timeFrame = "yearly";
            report = store.generateRevenueReport(timeFrame);

            cout << "\n==== Yearly Revenue Report ====" << endl;
            cout << "Orders: " << report.first << endl;
            cout << "Revenue: $" << fixed << setprecision(2) << report.second << endl;
            break;

        case 4: // All time report
            timeFrame = "all";
            report = store.generateRevenueReport(timeFrame);

            cout << "\n==== All Time Revenue Report ====" << endl;
            cout << "Orders: " << report.first << endl;
            cout << "Revenue: $" << fixed << setprecision(2) << report.second << endl;
            break;

        case 5: // Back to menu
            return;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Function to handle admin discount management
void manageDiscounts(Store& store) {
    while (true) {
        // Display discounts
        cout << "\n==== Discount Codes ====" << endl;
        vector<Discount>& discounts = store.getDiscounts();

        if (discounts.empty()) {
            cout << "No discount codes available." << endl;
        }
        else {
            for (size_t i = 0; i < discounts.size(); i++) {
                cout << i + 1 << ". ";
                discounts[i].display();
            }
        }

        cout << "\n==== Discount Management ====" << endl;
        cout << "1. Add Discount" << endl;
        cout << "2. Activate/Deactivate Discount" << endl;
        cout << "3. Remove Discount" << endl;
        cout << "4. Back to Menu" << endl;
        cout << "Enter your choice: ";

        int choice = getIntInput();

        switch (choice) {
        case 1: { // Add discount
            string code;
            double amount;
            string type;

            cout << "Enter discount code: ";
            cin >> code;
            clearInputBuffer();

            // Check if code already exists
            if (store.findDiscount(code)) {
                cout << "Discount code already exists." << endl;
                break;
            }

            cout << "Enter discount type (percentage or fixed): ";
            cin >> type;
            clearInputBuffer();

            if (type != "percentage" && type != "fixed") {
                cout << "Invalid discount type. Please enter 'percentage' or 'fixed'." << endl;
                break;
            }

            cout << "Enter discount amount";
            if (type == "percentage") {
                cout << " (%)";
            }
            else {
                cout << " ($)";
            }
            cout << ": ";
            amount = getDoubleInput();

            if (amount <= 0) {
                cout << "Amount must be greater than zero." << endl;
                break;
            }

            // Create and add discount
            Discount newDiscount(code, amount, type, true);
            store.addDiscount(newDiscount);

            cout << "Discount added successfully!" << endl;
            break;
        }

        case 2: { // Activate/Deactivate discount
            cout << "Enter discount code: ";
            string code;
            cin >> code;
            clearInputBuffer();

            Discount* discount = store.findDiscount(code);
            if (!discount) {
                cout << "Discount code not found." << endl;
                break;
            }

            discount->setActive(!discount->isActive());

            cout << "Discount " << (discount->isActive() ? "activated" : "deactivated") << " successfully!" << endl;
            break;
        }

        case 3: { // Remove discount
            cout << "Enter discount code to remove: ";
            string code;
            cin >> code;
            clearInputBuffer();

            if (store.removeDiscount(code)) {
                cout << "Discount removed successfully!" << endl;
            }
            else {
                cout << "Discount code not found." << endl;
            }
            break;
        }

        case 4: // Back to menu
            return;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }
}

// Function to view order history
void viewOrderHistory(const Customer* customer) {
    const vector<Order>& orders = customer->getOrderHistory();

    if (orders.empty()) {
        cout << "You have no previous orders." << endl;
        return;
    }

    cout << "\n==== Order History ====" << endl;
    cout << "You have " << orders.size() << " order(s):" << endl;

    for (size_t i = 0; i < orders.size(); i++) {
        cout << i + 1 << ". Order #" << orders[i].getId();

        // Convert time_t to readable date
        char dateStr[26];
        time_t orderTime = orders[i].getOrderDate();
        tm* timeinfo = localtime(&orderTime);
        strftime(dateStr, sizeof(dateStr), "%c", timeinfo);

        cout << " - " << dateStr;
        cout << " - Total: $" << fixed << setprecision(2) << orders[i].getTotal() << endl;
    }

    cout << "\nEnter order number to view details (0 to go back): ";
    int choice = getIntInput();

    if (choice > 0 && choice <= static_cast<int>(orders.size())) {
        orders[choice - 1].display();
    }
    else if (choice != 0) {
        cout << "Invalid order number." << endl;
    }
}

// Function to handle customer cart management
void manageCart(Store& store, Customer* customer) {
    while (true) {
        // Display cart
        customer->getCart().display();

        if (customer->getCart().isEmpty()) {
            cout << "\n1. Back to Menu" << endl;
            cout << "Enter your choice: ";

            int choice = getIntInput();
            if (choice == 1) {
                return;
            }
            else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
        else {
            cout << "\n1. Update Item Quantity" << endl;
            cout << "2. Remove Item" << endl;
            cout << "3. Apply Discount" << endl;
            cout << "4. Remove Discount" << endl;
            cout << "5. Checkout" << endl;
            cout << "6. Back to Menu" << endl;
            cout << "Enter your choice: ";

            int choice = getIntInput();

            switch (choice) {
            case 1: { // Update item quantity
                cout << "Enter item number: ";
                int itemIndex = getIntInput() - 1;

                if (itemIndex < 0 || itemIndex >= static_cast<int>(customer->getCart().getItems().size())) {
                    cout << "Invalid item number." << endl;
                    break;
                }

                // Get product ID from cart item
                int productId = customer->getCart().getItems()[itemIndex].getProduct().getId();

                // Get fresh product data from store
                Product* product = store.findProduct(productId);
                if (!product) {
                    cout << "Error: Product no longer exists." << endl;
                    break;
                }

                cout << "Current quantity: " << customer->getCart().getItems()[itemIndex].getQuantity() << endl;
                cout << "Enter new quantity (0 to remove): ";
                int newQuantity = getIntInput();

                if (newQuantity < 0) {
                    cout << "Quantity cannot be negative." << endl;
                }
                else if (newQuantity == 0) {
                    customer->getCart().removeItem(itemIndex);
                    cout << "Item removed from cart." << endl;
                }
                else if (newQuantity > product->getStock()) {
                    cout << "Not enough stock available. Maximum: " << product->getStock() << endl;
                }
                else {
                    customer->getCart().updateItemQuantity(itemIndex, newQuantity);
                    cout << "Quantity updated successfully." << endl;
                }
                break;
            }

            case 2: { // Remove item
                cout << "Enter item number to remove: ";
                int itemIndex = getIntInput() - 1;

                if (customer->getCart().removeItem(itemIndex)) {
                    cout << "Item removed from cart." << endl;
                }
                else {
                    cout << "Invalid item number." << endl;
                }
                break;
            }

            case 3: { // Apply discount
                if (!customer->getCart().getDiscountCode().empty()) {
                    cout << "A discount is already applied. Remove it first." << endl;
                    break;
                }

                cout << "Enter discount code: ";
                string code;
                cin >> code;
                clearInputBuffer();

                Discount* discount = store.findDiscount(code);
                if (!discount) {
                    cout << "Invalid discount code." << endl;
                    break;
                }

                if (!discount->isValid()) {
                    cout << "Discount code is inactive." << endl;
                    break;
                }

                double discountAmount = discount->calculateDiscount(customer->getCart().getSubtotal());
                customer->getCart().applyDiscount(discountAmount, code);

                cout << "Discount applied successfully!" << endl;
                break;
            }

            case 4: { // Remove discount
                if (customer->getCart().getDiscountCode().empty()) {
                    cout << "No discount is currently applied." << endl;
                    break;
                }

                customer->getCart().removeDiscount();
                cout << "Discount successfully removed." << endl;
                break;
            }

            case 5: { // Checkout
                cout << "\n=== Checkout ===" << endl;
                customer->getCart().display();

                cout << "\nConfirm order? (y/n): ";
                char confirm;
                cin >> confirm;
                clearInputBuffer();

                if (tolower(confirm) == 'y') {
                    if (store.processCheckout()) {
                        cout << "Order placed successfully!" << endl;
                        return;
                    }
                    else {
                        cout << "Error - Please check if items are in still instock." << endl;
                    }
                }
                else {
                    cout << "Error - checkout cancelled." << endl;
                }
                break;
            }

            case 6: // Back to menu
                return;

            default:
                cout << "Invalid - Please try again." << endl;
            }
        }
    }
}

int main() {

    // Starts the application
    runApp();

    return 0;
}