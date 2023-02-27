#include <conio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;

const int ARRAY_SIZE = 50;
int sessionUserIndex;
string currentDate;

// Users Data 
string usersFile = "usersData.csv";
int usersCount = 0;
string userfullNames[ARRAY_SIZE];
string usernames[ARRAY_SIZE];
string passwords[ARRAY_SIZE];
string userTypes[ARRAY_SIZE];

// Sellers Data
string sellersFile = "sellersData.csv";
int sellingItems = 0;
int sellerIds[ARRAY_SIZE];
string itemNames[ARRAY_SIZE];
string itemDescriptions[ARRAY_SIZE];
string itemCategories[ARRAY_SIZE];
float itemPrices[ARRAY_SIZE];
int itemQuantities[ARRAY_SIZE];
int itemSold[ARRAY_SIZE];

// Buyers Data
string cartFile = "cartData.csv";
int cartItems = 0;
int cartItemIds[ARRAY_SIZE];
int cartBuyerIds[ARRAY_SIZE];
float cartItemPrices[ARRAY_SIZE];
int cartItemQuantities[ARRAY_SIZE];

string orderFile = "orderData.csv";
int orderItems = 0;
int orderItemIds[ARRAY_SIZE];
int orderBuyerIds[ARRAY_SIZE];
float orderItemPrices[ARRAY_SIZE];
int orderItemQuantities[ARRAY_SIZE];
int orderItemTrackingCodes[ARRAY_SIZE];
string orderDates[ARRAY_SIZE];
int orderArrivalTimes[ARRAY_SIZE];

// Display Data
int displayItems;
int displayItemIndex[ARRAY_SIZE];

int TERMINAL_WIDTH, TERMINAL_HEIGHT;
int HEADER_WIDTH, HEADER_HEIGHT;
int TITLE_HEIGHT = 2;
int headerPadding, optionsPadding;
bool largeTerminal;

void startApplication();
void printMainScreen();

void loginUser();
void signUpUser();

void openSellerAccount();
void printSellerMenu();
void loadSellerItems();
void addSellerItem();
void viewSellerItem();
void sellerItemExtendedView(int itemIndex);
void updateItemScreen();
void updateItemDetails(int itemIndex);
void removeSellerItemScreen();
void removeSellerItem(int itemIndex);
void removeItem(int itemIndex);
void viewSellerStats();

void openBuyerAccount();
void printBuyerMenu();
void viewMarketPlace();
void searchMarketPlace();
void shopByCategories();
void buyerCategoryExtendedView(string categoryName);
void buyerItemExtendedView(int itemIndex, string title);
void handleOrder(int itemIndex, char orderType);
void addItemToCart(int itemIndex, int itemQuantity, int percentageDiscount);
void placeOrder(int itemIndex, int itemQuantity, int percentageDiscount);
void viewShoppingCart();
void shoppingCartExtended(int cartIndex);
void checkoutItem(int cartIndex);
void checkOutAll();
void trackOrder();
void removeDeletedItemFromCarts(int itemIndex);
void removeCartItem(int cartIndex);

bool validateLogin(string username, string password);
bool uniqueUsername(string username);
bool detectCommas(string text);
bool validCoupon(string coupon);

void loadFiles();
void writeUsersData();
void writeSellerData();
void reWriteSellerData();
void writeCartData();
void reWriteCartData();
void writeOrderData();
void reWriteOrderData();

void setTerminalDimensions();
string parseCSV(string dataText, int index);
string getCurrentDate();
string currentDateTime();
int dateDifference(string dateOne, string dateTwo);
int dateToDays(string date);
int generateTrackingCode();
bool uniqueCode(int code);
int generateArrivalTime();
void confirmationDialog();
bool tryAgainDialog();
int displayItemsPage(char displayType);
void displayItem(int itemIndex);
void displayCartItem(int itemIndex);
void displayOrderItem(int itemIndex);
void clearItems(int startingY);

void printTitle(string title);
void printHeader();
void printSmallHeader();
void printBigHeader();
void printLineBreak();

int handleOptionSelection(int arrowX, int arrowY, int numberOfOptions, int offset, bool isScrollable);
void displayArrow(int x, int y);
void removeArrow(int x, int y);
void gotoPosition(int x, int y);
int wherey();
void showConsoleCursor(bool flag);

int main() {
    showConsoleCursor(false);
    loadFiles();
    setTerminalDimensions();
    startApplication();
}

void startApplication() {
    currentDate = getCurrentDate();
    int options = 4, offset = 1;
    bool isScrollable = false;
    bool applicationRunning = true;
    while (applicationRunning) {
        printMainScreen();
        int option = handleOptionSelection(optionsPadding, wherey() - options, options, offset, isScrollable);
        if (option == 1) {
            loginUser();
        } else if (option == 2) {
            signUpUser();
        } else if (option == 3) {
            // displayManual();
        } else if (option == 4) {
            applicationRunning = false;
        }
    }
}

void printMainScreen() {
    printTitle("Index");

    cout << setw(optionsPadding) << "" << "   Login to Your Account" << endl;
    cout << setw(optionsPadding) << "" << "   Create Account & Sign Up" << endl;
    cout << setw(optionsPadding) << "" << "   Read Application Manual" << endl;
    cout << setw(optionsPadding) << "" << "   Exit" << endl;
}

void loginUser() {
    int option;
    bool onLoginScreen = true, loggedIn = false;
    string username, password;

    while (onLoginScreen) {
        printTitle("Login");
        showConsoleCursor(true);
        cout << setw(optionsPadding) << "" << "Username: ";
        cin >> username;
        cout << setw(optionsPadding) << "" << "Password: ";
        cin >> password;
        showConsoleCursor(false);
        cout << endl;

        if (validateLogin(username, password)) {
            cout << setw(optionsPadding) << "" << "Login Successful!!!" << endl << endl;
            onLoginScreen = false;
            loggedIn = true;
            confirmationDialog();
        } else {
            cout << setw(optionsPadding) << "" << "Login Failed!!!" << endl;
            cout << setw(optionsPadding) << "" << "Incorrect Username/Password" << endl << endl;
            if (!tryAgainDialog()) {
                onLoginScreen = false;
            }
        }
    }
    if (loggedIn) {
        if (userTypes[sessionUserIndex] == "Seller") {
            loadSellerItems();
            openSellerAccount();
        } else {
            openBuyerAccount();
        }
    }
}

void openSellerAccount() {
    int options = 8;
    int option;
    bool onSellerScreen = true;
    while (onSellerScreen) {
        printTitle("Seller");
        printSellerMenu();
        option = handleOptionSelection(optionsPadding, wherey() - options, options, 1, false);
        if (option == 1) {
            addSellerItem();
        } else if (option == 2) {
            viewSellerItem();
        } else if (option == 3) {
            updateItemScreen();
        } else if (option == 4) {
            removeSellerItemScreen();
        } else if (option == 5) {
            viewSellerStats();
        } else {
            onSellerScreen = false;
        }
    }
}

void printSellerMenu() {
    cout << setw(optionsPadding) << "" << "   Add Item to Market Place" << endl;
    cout << setw(optionsPadding) << "" << "   View Your Items" << endl;
    cout << setw(optionsPadding) << "" << "   Update Your Item Details" << endl;
    cout << setw(optionsPadding) << "" << "   Remove Item From Market Place" << endl;
    cout << setw(optionsPadding) << "" << "   View Stats About Your Items" << endl;
    cout << setw(optionsPadding) << "" << "   Add/Update Coupon" << endl;
    cout << setw(optionsPadding) << "" << "   Remove Coupon" << endl;
    cout << setw(optionsPadding) << "" << "   Log Out" << endl;
}

void loadSellerItems() {
    displayItems = 0;
    for (int i = 0; i < sellingItems; i++) {
        if (sellerIds[i] == sessionUserIndex) {
            displayItemIndex[displayItems] = i;
            displayItems++;
        }
    }
}

void addSellerItem() {
    string categories[] = { "Cosmetics", "Electronics", "Consoles and Gaming", "Toys", "Home Appliances", "Sports Accessories", "Health and Beauty", "Groceries" };
    int option;
    string itemName, itemDescription, itemCategory;
    float itemPrice;
    int itemQuantity;
    bool addingItem = true, containCommas;

    while (addingItem) {
        containCommas = false;
        printTitle("Seller > Add Item");
        showConsoleCursor(true);
        cout << setw(optionsPadding) << "" << "Item Name: ";
        cin.clear();
        cin.sync();
        getline(cin, itemName);
        cout << setw(optionsPadding) << "" << "Item Description: ";
        getline(cin, itemDescription);
        cout << setw(optionsPadding) << "" << "Item Price: ";
        cin >> itemPrice;
        cout << setw(optionsPadding) << "" << "Item Quantity: ";
        cin >> itemQuantity;
        showConsoleCursor(false);
        cout << setw(optionsPadding) << "" << "Item Category: " << endl;
        for (int i = 0; i < 8; i++) {
            cout << setw(optionsPadding) << "" << "   " << categories[i] << endl;
        }
        int arrowY = wherey();
        option = handleOptionSelection(optionsPadding, arrowY - 8, 8, 1, false);
        itemCategory = categories[option - 1];

        containCommas = detectCommas(itemName) ? true : containCommas;
        containCommas = detectCommas(itemDescription) ? true : containCommas;

        gotoPosition(0, arrowY + 1);
        if (!containCommas) {
            sellerIds[sellingItems] = sessionUserIndex;
            itemNames[sellingItems] = itemName;
            itemDescriptions[sellingItems] = itemDescription;
            itemCategories[sellingItems] = itemCategory;
            itemPrices[sellingItems] = itemPrice;
            itemQuantities[sellingItems] = itemQuantity;
            itemSold[sellingItems] = 0;
            writeSellerData();
            addingItem = false;
            displayItemIndex[displayItems] = sellingItems;
            sellingItems++;
            displayItems++;
            cout << setw(optionsPadding) << "" << "Item Add Successfully!!!" << endl << endl;
            confirmationDialog();
        } else {
            cout << setw(optionsPadding) << "" << "Could Not Add Item!!!" << endl;
            cout << setw(optionsPadding) << "" << "Input Cannot Contain Commas" << endl;
            if (!tryAgainDialog()) {
                addingItem = false;
            }
        }
    }
}

void viewSellerItem() {
    int itemIndex;
    bool onViewScreen = true;
    while (onViewScreen) {
        printTitle("Seller > View Item");
        itemIndex = displayItemsPage('I');
        if (itemIndex != -1) {
            sellerItemExtendedView(itemIndex);
        } else {
            onViewScreen = false;
        }
    }
}

void sellerItemExtendedView(int itemIndex) {
    string itemName = itemNames[itemIndex];
    bool viewingItem = true;
    while (viewingItem) {
        printTitle("Seller > View Items > " + itemName);
        cout << setw(optionsPadding) << "" << "Item Name: " << itemName << endl;
        cout << setw(optionsPadding) << "" << "Item Description: " << itemDescriptions[itemIndex] << endl;
        cout << setw(optionsPadding) << "" << "Item Category: " << itemCategories[itemIndex] << endl;
        cout << setw(optionsPadding) << "" << "Item Price: PKR " << itemPrices[itemIndex] << endl;
        cout << setw(optionsPadding) << "" << "Item Remaining Quantity: " << itemQuantities[itemIndex] - itemSold[itemIndex] << endl;
        cout << setw(optionsPadding) << "" << "Item Sold: " << itemSold[itemIndex] << endl;
        cout << endl;

        cout << setw(optionsPadding) << "" << "   Update Item Details" << endl;
        cout << setw(optionsPadding) << "" << "   Remove Item From Market Place" << endl;
        cout << setw(optionsPadding) << "" << "   Go Back" << endl;
        int option = handleOptionSelection(optionsPadding, wherey() - 3, 3, 1, false);
        if (option == 1) {
            updateItemDetails(itemIndex);
        } else if (option == 2) {
            removeSellerItem(itemIndex);
            viewingItem = false;
        } else {
            viewingItem = false;
        }
    }
}

void updateItemScreen() {
    int itemIndex;
    bool onUpdateScreen = true;
    while (onUpdateScreen) {
        printTitle("Seller > Update Item");
        itemIndex = displayItemsPage('I');
        if (itemIndex != -1) {
            updateItemDetails(itemIndex);
        } else {
            onUpdateScreen = false;
        }
    }
}

void updateItemDetails(int itemIndex) {
    string itemName, itemDescription;
    int additionalStock;
    float itemPrice;
    string displayName = itemNames[itemIndex];
    bool onUpdateScreen = true;
    int option;
    while (onUpdateScreen) {
        printTitle("Seller > Update Item > " + displayName);
        cout << setw(optionsPadding) << "" << "Select Attribute to Update: " << endl;
        cout << setw(optionsPadding) << "" << "   Item Name" << endl;
        cout << setw(optionsPadding) << "" << "   Item Description" << endl;
        cout << setw(optionsPadding) << "" << "   Item Price" << endl;
        cout << setw(optionsPadding) << "" << "   Add More Stock" << endl;
        cout << setw(optionsPadding) << "" << "   Go Back" << endl;
        int currentY = wherey();
        option = handleOptionSelection(optionsPadding, currentY - 5, 5, 1, false);
        gotoPosition(0, currentY + 1);
        showConsoleCursor(true);
        if (option == 1) {
            cout << setw(optionsPadding) << "" << "Updated Item Name: ";
            cin.clear();
            cin.sync();
            getline(cin, itemName);
            cout << endl;
            if (detectCommas(itemName)) {
                cout << setw(optionsPadding) << "" << "Invalid Input!!!" << endl;
                cout << setw(optionsPadding) << "" << "Item Name Cannot Contains Commas" << endl;
                if (!tryAgainDialog()) {
                    onUpdateScreen = false;
                }
            } else {
                itemNames[itemIndex] = itemName;
                cout << setw(optionsPadding) << "" << "Item Name Updated Successfully!!!" << endl;
                confirmationDialog();
                onUpdateScreen = false;
            }
        } else if (option == 2) {
            cout << setw(optionsPadding) << "" << "Updated Item Description: ";
            cin.clear();
            cin.sync();
            getline(cin, itemDescription);
            cout << endl;
            if (detectCommas(itemDescription)) {
                cout << setw(optionsPadding) << "" << "Invalid Input!!!" << endl;
                cout << setw(optionsPadding) << "" << "Item Description Cannot Contains Commas" << endl;
                if (!tryAgainDialog()) {
                    onUpdateScreen = false;
                }
            } else {
                itemDescriptions[itemIndex] = itemDescription;
                cout << setw(optionsPadding) << "" << "Item Description Updated Successfully!!!" << endl;
                confirmationDialog();
                onUpdateScreen = false;
            }
        } else if (option == 3) {
            cout << setw(optionsPadding) << "" << "Updated Item Price: ";
            cin >> itemPrice;
            cout << endl;
            itemPrices[itemIndex] = itemPrice;
            cout << setw(optionsPadding) << "" << "Item Price Updated Successfully!!!" << endl;
            confirmationDialog();
            onUpdateScreen = false;
        } else if (option == 4) {
            cout << setw(optionsPadding) << "" << "Add Additional Stock: ";
            cin >> additionalStock;
            cout << endl;
            itemQuantities[itemIndex] += additionalStock;
            cout << setw(optionsPadding) << "" << "Item Quantity Updated Successfully!!!" << endl;
            confirmationDialog();
            onUpdateScreen = false;
        } else {
            onUpdateScreen = false;
        }
        showConsoleCursor(false);
        reWriteSellerData();
    }
}

void removeSellerItemScreen() {
    int itemIndex;
    bool onRemoveScreen = true;
    while (onRemoveScreen) {
        printTitle("Seller > Remove Item");
        itemIndex = displayItemsPage('I');
        if (itemIndex != -1) {
            removeSellerItem(itemIndex);
        } else {
            onRemoveScreen = false;
        }
    }
}

void removeSellerItem(int itemIndex) {
    string itemName = itemNames[itemIndex];
    printTitle("Seller > Remove Item > " + itemName);
    cout << setw(optionsPadding) << "" << "Are You Sure You want to remove " << itemName << " from Marketplace?" << endl;
    cout << setw(optionsPadding) << "" << endl;
    cout << setw(optionsPadding) << "" << "   Yes" << endl;
    cout << setw(optionsPadding) << "" << "   Cancel" << endl;
    int option = handleOptionSelection(optionsPadding, wherey() - 2, 2, 1, false);
    if (option == 1) {
        removeItem(itemIndex);
    }
}

void removeItem(int itemIndex) {
    int i = 0;
    while (i < sellingItems) {
        if (itemIndex == i) {
            for (int j = i; j < sellingItems - 1; j++) {
                sellerIds[j] = sellerIds[j + 1];
                itemNames[j] = itemNames[j + 1];
                itemDescriptions[j] = itemDescriptions[j + 1];
                itemCategories[j] = itemCategories[j + 1];
                itemPrices[j] = itemPrices[j + 1];
                itemQuantities[j] = itemQuantities[j + 1];
                itemSold[j] = itemSold[j + 1];
            }
            break;
        } else {
            i++;
        }
    }
    removeDeletedItemFromCarts(itemIndex);
    sellingItems--;
    loadSellerItems();
    reWriteSellerData();
}

void viewSellerStats() {
    printTitle("Seller > View Stats");
    cout << left << setw(optionsPadding) << "" << setw(10) << "No." << setw(40) << "Item Name" << setw(20) << "Item Remaining" << setw(20) << "Item Sold" << endl << endl;
    for (int i = 0, itemIndex; i < displayItems; i++) {
        itemIndex = displayItemIndex[i];
        cout << left << setw(optionsPadding) << "" << setw(10) << i + 1 << setw(40) << itemNames[itemIndex] << setw(20) << itemQuantities[itemIndex] - itemSold[itemIndex] << setw(20) << itemSold[itemIndex] << endl;
    }
    cout << endl;
    cout << setw(optionsPadding) << "" << ">  Go Back" << endl;
    getch();
}

void openBuyerAccount() {
    int options = 8;
    int option;
    bool onBuyerScreen = true;
    while (onBuyerScreen) {
        printTitle("Buyer");
        printBuyerMenu();
        option = handleOptionSelection(optionsPadding, wherey() - options, options, 1, false);
        if (option == 1) {
            viewMarketPlace();
        } else if (option == 2) {
            searchMarketPlace();
        } else if (option == 3) {
            shopByCategories();
        } else if (option == 4) {
            viewShoppingCart();
        } else if (option == 5) {
            checkOutAll();
        } else if (option == 6) {
            trackOrder();
        } else {
            onBuyerScreen = false;
        }
    }
}

void printBuyerMenu() {
    cout << setw(optionsPadding) << "" << "   Visit Market Place" << endl;
    cout << setw(optionsPadding) << "" << "   Search for Product" << endl;
    cout << setw(optionsPadding) << "" << "   Shop By Categories" << endl;
    cout << setw(optionsPadding) << "" << "   View Shopping Cart" << endl;
    cout << setw(optionsPadding) << "" << "   Checkout All Items" << endl;
    cout << setw(optionsPadding) << "" << "   Track Your Order" << endl;
    cout << setw(optionsPadding) << "" << "   Notifications" << endl;
    cout << setw(optionsPadding) << "" << "   Log Out" << endl;
}

void viewMarketPlace() {
    int itemIndex;
    bool onMarketPlace = true;
    while (onMarketPlace) {
        printTitle("Buyer > Market Place");
        displayItems = 0;
        for (int i = 0; i < sellingItems; i++) {
            displayItemIndex[displayItems] = i;
            displayItems++;
        }
        itemIndex = displayItemsPage('I');
        if (itemIndex != -1) {
            buyerItemExtendedView(itemIndex, "Buyer > Market Place");
        } else {
            onMarketPlace = false;
        }
    }
}

void searchMarketPlace() {
    int itemIndex;
    string query;
    bool onSearchPage = true;
    while (onSearchPage) {

        printTitle("Buyer > Search Market Place");
        displayItems = 0;
        showConsoleCursor(true);
        cout << setw(optionsPadding) << "" << "Search: ";
        cin.clear();
        cin.sync();
        getline(cin, query);
        showConsoleCursor(false);
        cout << endl;
        for (int i = 0; i < sellingItems; i++) {
            if (itemNames[i] == query) {
                displayItemIndex[displayItems] = i;
                displayItems++;
            }
        }
        itemIndex = displayItemsPage('I');
        if (itemIndex != -1) {
            buyerItemExtendedView(itemIndex, "Buyer > Search Market Place");
        } else {
            onSearchPage = false;
        }
    }
}

void shopByCategories() {
    int itemIndex;
    bool onCategoryScreen = true;
    string categoryName;
    string categories[] = { "Cosmetics", "Electronics", "Consoles and Gaming", "Toys", "Home Appliances", "Sports Accessories", "Health and Beauty", "Groceries" };
    while (onCategoryScreen) {
        printTitle("Buyer > Categories");
        cout << setw(optionsPadding) << "" << "Choose Category: " << endl << endl;
        for (int i = 0; i < 8; i++) {
            cout << setw(optionsPadding) << "" << "   " << categories[i] << endl;
        }
        cout << setw(optionsPadding) << "" << "   Go Back" << endl;
        int category = handleOptionSelection(optionsPadding, wherey() - 9, 9, 1, false);
        if (category == 9) {
            onCategoryScreen = false;
        } else {
            categoryName = categories[category - 1];
            displayItems = 0;
            for (int i = 0; i < sellingItems; i++) {
                if (itemCategories[i] == categoryName) {
                    displayItemIndex[displayItems] = i;
                    displayItems++;
                }
            }
            buyerCategoryExtendedView(categoryName);
        }
    }
}

void buyerCategoryExtendedView(string categoryName) {
    bool onCategoryView = true;
    int itemIndex;
    while (onCategoryView) {
        printTitle("Buyer > Categories > " + categoryName);
        itemIndex = displayItemsPage('I');
        if (itemIndex != -1) {
            buyerItemExtendedView(itemIndex, "Buyer > Categories > " + categoryName);
        } else {
            onCategoryView = false;
        }
    }
}

void buyerItemExtendedView(int itemIndex, string title) {
    int option;
    bool onItemView = true, itemAvailable;
    string itemName = itemNames[itemIndex];
    string itemSeller = userfullNames[sellerIds[itemIndex]];
    string itemDescription = itemDescriptions[itemIndex];
    int itemPrice = itemPrices[itemIndex];
    while (onItemView) {
        itemAvailable = (itemQuantities[itemIndex] != itemSold[itemIndex]);
        printTitle(title + " > " + itemName);
        cout << setw(optionsPadding) << "" << "Item Name: " << itemName << endl;
        cout << setw(optionsPadding) << "" << "Item Seller: " << itemSeller << endl;
        cout << setw(optionsPadding) << "" << "Item Description: " << itemDescription << endl;
        cout << setw(optionsPadding) << "" << "Item Price: PKR " << itemPrice << endl;
        cout << setw(optionsPadding) << "" << "Item Availability: " << (itemAvailable ? "In-Stock" : "Out-Of-Stock") << endl;
        cout << endl;

        cout << setw(optionsPadding) << "" << "   Add To Cart" << endl;
        cout << setw(optionsPadding) << "" << "   Buy Now" << endl;
        cout << setw(optionsPadding) << "" << "   Go Back" << endl;
        int arrowY = wherey();
        option = handleOptionSelection(optionsPadding, arrowY - 3, 3, 1, false);
        gotoPosition(0, arrowY + 1);
        if (option == 1) {
            handleOrder(itemIndex, 'C');
        } else if (option == 2) {
            handleOrder(itemIndex, 'O');
        } else {
            onItemView = false;
        }
    }
}

void handleOrder(int itemIndex, char orderType) {
    bool onOrderScreen = true;
    string coupon;
    string itemName = itemNames[itemIndex];
    int itemQuantity, currentY, option;
    string title = "Order Item";
    if (orderType == 'C') {
        title = "Add to Cart";
    }
    while (onOrderScreen) {
        bool itemUnavailable = (itemQuantities[itemIndex] == itemSold[itemIndex]);
        int remainingItem = itemQuantities[itemIndex] - itemSold[itemIndex];
        printTitle("Buyer > " + title + " > " + itemName);
        cout << setw(optionsPadding) << "" << "Adding " << itemName << " to " << (orderType == 'C' ? "Shopping Cart" : "Order List") << endl;
        cout << endl;
        if (itemUnavailable) {
            cout << setw(optionsPadding) << "" << "ITEM UNAVAILABLE!!!" << endl;
            onOrderScreen = false;
        } else {
            showConsoleCursor(true);
            cout << setw(optionsPadding) << "" << "Item Quantity: ";
            cin >> itemQuantity;
            showConsoleCursor(false);
            if (itemQuantity < 1 || itemQuantity > remainingItem) {
                cout << setw(optionsPadding) << "" << "ITEM QUANTITY OUT OF RANGE!!!" << endl;
                cout << setw(optionsPadding) << "" << "Valid Range is 1-" << remainingItem << endl;
            } else {
                cout << endl;
                cout << setw(optionsPadding) << "" << "   Apply Coupon" << endl;
                cout << setw(optionsPadding) << "" << "   " << (orderType == 'C' ? "Add to Shopping Cart" : "Order Now") << endl;
                currentY = wherey();
                option = handleOptionSelection(optionsPadding, currentY - 2, 2, 1, false);
                gotoPosition(0, currentY + 2);
                if (option == 1) {
                    showConsoleCursor(true);
                    cout << setw(optionsPadding) << "" << "COUPON: ";
                    cin >> coupon;
                    showConsoleCursor(false);
                    if (!validCoupon(coupon)) {
                        cout << setw(optionsPadding) << "" << "INVALID COUPON!!!" << endl;
                    } else {
                        if (orderType == 'C') {
                            addItemToCart(itemIndex, itemQuantity, 0);
                        } else {
                            placeOrder(itemIndex, itemQuantity, 0);
                        }
                    }
                } else if (option == 2) {
                    cout << endl;
                    if (orderType == 'C') {
                        addItemToCart(itemIndex, itemQuantity, 0);
                        cout << setw(optionsPadding) << "" << "Item Added to Cart Successfully!!!" << endl;
                    } else {
                        placeOrder(itemIndex, itemQuantity, 0);
                        cout << setw(optionsPadding) << "" << "Order Placed Successfully!!!" << endl;
                    }
                    onOrderScreen = false;
                }
            }
        }
        cout << endl;
        confirmationDialog();
    }
}


void addItemToCart(int itemIndex, int itemQuantity, int percentageDiscount) {
    cartItemIds[cartItems] = itemIndex;
    cartBuyerIds[cartItems] = sessionUserIndex;
    cartItemPrices[cartItems] = itemPrices[itemIndex] - ((float)percentageDiscount / 100 * itemPrices[itemIndex]);
    cartItemQuantities[cartItems] = itemQuantity;
    itemSold[itemIndex] += itemQuantity;
    writeCartData();
    reWriteSellerData();
    cartItems++;
}

void placeOrder(int itemIndex, int itemQuantity, int percentageDiscount) {
    orderItemIds[orderItems] = itemIndex;
    orderBuyerIds[orderItems] = sessionUserIndex;
    orderItemPrices[orderItems] = itemPrices[itemIndex] - ((float)percentageDiscount / 100 * itemPrices[itemIndex]);
    orderItemQuantities[orderItems] = itemQuantity;
    orderItemTrackingCodes[orderItems] = generateTrackingCode();
    orderDates[orderItems] = currentDate;
    orderArrivalTimes[orderItems] = generateArrivalTime();
    itemSold[itemIndex] += itemQuantity;

    writeOrderData();
    reWriteSellerData();
    orderItems++;
}

void viewShoppingCart() {
    int cartIndex;
    bool viewingCart = true;
    while (viewingCart) {
        displayItems = 0;
        for (int i = 0; i < cartItems; i++) {
            if (cartBuyerIds[i] == sessionUserIndex) {
                displayItemIndex[displayItems] = i;
                displayItems++;
            }
        }
        printTitle("Buyer > Shopping Cart");
        cartIndex = displayItemsPage('C');
        if (cartIndex != -1) {
            shoppingCartExtended(cartIndex);
        } else {
            viewingCart = false;
        }
    }

}

void shoppingCartExtended(int cartIndex) {
    int option;
    string itemName = itemNames[cartItemIds[cartIndex]];
    string itemSeller = userfullNames[sellerIds[cartItemIds[cartIndex]]];
    int itemQuantity = cartItemQuantities[cartIndex];
    float itemPrice = cartItemPrices[cartIndex];
    float totalPrice = itemPrice * itemQuantity;
    printTitle("Buyer > Shopping Cart > " + itemName);
    cout << setw(optionsPadding) << "" << "Item Name: " << itemName << endl;
    cout << setw(optionsPadding) << "" << "Item Seller: " << itemSeller << endl;
    cout << setw(optionsPadding) << "" << "Item Quantity: " << itemQuantity << endl;
    cout << setw(optionsPadding) << "" << "Item Price Per Unit: PKR " << itemPrice << endl;
    cout << setw(optionsPadding) << "" << "Total Price: PKR " << totalPrice << endl;
    cout << endl;
    cout << setw(optionsPadding) << "" << "   Checkout Item" << endl;
    cout << setw(optionsPadding) << "" << "   Remove from Cart" << endl;
    cout << setw(optionsPadding) << "" << "   Go Back" << endl;
    int currentY = wherey();
    option = handleOptionSelection(optionsPadding, currentY - 3, 3, 1, false);
    gotoPosition(0, currentY + 2);
    if (option == 1) {
        checkoutItem(cartIndex);
        reWriteCartData();
        cout << setw(optionsPadding) << "" << itemName << " Checked Out Successfully!!!" << endl;
        confirmationDialog();
    } else if (option == 2) {
        itemSold[cartItemIds[cartIndex]] -= cartItemQuantities[cartIndex];
        removeCartItem(cartIndex);
        reWriteCartData();
        cout << setw(optionsPadding) << "" << itemName << " Remove From Cart Successfully!!!" << endl;
        confirmationDialog();
    }
}

void checkoutItem(int cartIndex) {
    orderItemIds[orderItems] = cartItemIds[cartIndex];
    orderBuyerIds[orderItems] = cartBuyerIds[cartIndex];
    orderItemPrices[orderItems] = cartItemPrices[cartIndex];
    orderItemQuantities[orderItems] = cartItemQuantities[cartIndex];
    orderItemTrackingCodes[orderItems] = generateTrackingCode();
    orderDates[orderItems] = currentDate;
    orderArrivalTimes[orderItems] = generateArrivalTime();
    writeOrderData();
    orderItems++;
    removeCartItem(cartIndex);
}

void checkOutAll() {
    printTitle("Buyer > Shopping Cart > Check Out All");
    cout << setw(optionsPadding) << "" << "Do You Want to Check Out All Cart Items?" << endl << endl;
    cout << setw(optionsPadding) << "" << "   Yes" << endl;
    cout << setw(optionsPadding) << "" << "   Cancel" << endl;
    int currentY = wherey();
    int option = handleOptionSelection(optionsPadding, currentY - 2, 2, 1, false);
    if (option == 2) return;
    int i = 0;
    while (i < cartItems) {
        if (cartBuyerIds[i] == sessionUserIndex) {
            checkoutItem(i);
        } else {
            i++;
        }
    }
    reWriteCartData();
    gotoPosition(0, currentY + 2);
    cout << setw(optionsPadding) << "" << "All Items Checked Out Successfully!!!" << endl << endl;
    confirmationDialog();
}

void trackOrder() {
    printTitle("Buyer > Track Order");
    displayItems = 0;
    for (int i = 0; i < orderItems; i++) {
        if (orderBuyerIds[i] == sessionUserIndex) {
            displayItemIndex[displayItems] = i;
            displayItems++;
        }
    }
    displayItemsPage('O');
}

void removeDeletedItemFromCarts(int itemIndex) {
    int i = 0;
    while (i < cartItems) {
        if (cartItemIds[i] == itemIndex) {
            removeCartItem(i);
        } else {
            i++;
        }
    }
    reWriteCartData();
}

void removeCartItem(int cartIndex) {
    for (int j = cartIndex; j < cartItems - 1; j++) {
        cartItemIds[j] = cartItemIds[j + 1];
        cartBuyerIds[j] = cartBuyerIds[j + 1];
        cartItemPrices[j] = cartItemPrices[j + 1];
        cartItemQuantities[j] = cartItemQuantities[j + 1];
    }
    cartItems--;
}

bool validCoupon(string coupon) {
    return false;
}
void signUpUser() {
    int option;
    int options = 2, offset = 1;
    bool isScrollable = false;
    bool onSignUpScreen = true;
    bool containCommas;
    string username, fullname, password, userType;

    while (onSignUpScreen) {
        containCommas = false;
        printTitle("Sign Up");

        cout << setw(optionsPadding) << "" << "Select Account Type: " << endl;
        cout << setw(optionsPadding) << "" << "   Seller" << endl;
        cout << setw(optionsPadding) << "" << "   Buyer" << endl;
        int currentY = wherey();
        option = handleOptionSelection(optionsPadding, currentY - 2, options, offset, isScrollable);
        cout << endl;

        if (option == 1) {
            userType = "Seller";
        } else {
            userType = "Buyer";
        }

        showConsoleCursor(true);
        gotoPosition(0, currentY + 1);
        cout << setw(optionsPadding) << "" << "Full Name: ";
        cin.clear();
        cin.sync();
        getline(cin, fullname);
        cout << setw(optionsPadding) << "" << "Username: ";
        cin >> username;
        cout << setw(optionsPadding) << "" << "Password: ";
        cin >> password;
        showConsoleCursor(false);

        containCommas = detectCommas(fullname) ? true : containCommas;
        containCommas = detectCommas(username) ? true : containCommas;
        containCommas = detectCommas(password) ? true : containCommas;

        cout << endl;
        if (uniqueUsername(username) && !containCommas) {
            cout << setw(optionsPadding) << "" << "Account Created Successfully!!!" << endl;
            confirmationDialog();
            onSignUpScreen = false;
            usernames[usersCount] = username;
            userfullNames[usersCount] = fullname;
            passwords[usersCount] = password;
            userTypes[usersCount] = userType;
            writeUsersData();
            usersCount++;
        } else {
            cout << setw(optionsPadding) << "" << "Sign Up Failed!!!" << endl;
            if (!containCommas) {
                cout << setw(optionsPadding) << "" << "Username Already Exists" << endl;
            } else {
                cout << setw(optionsPadding) << "" << "Input Cannot Contain Commas" << endl;
            }
            if (!tryAgainDialog()) {
                onSignUpScreen = false;
            }
        }
    }
}


void loadFiles() {
    string rawData = "";
    string username, fullname, password, type;
    fstream usersFileHandle;
    usersFileHandle.open(usersFile, ios::in);

    while (getline(usersFileHandle, rawData)) {
        fullname = parseCSV(rawData, 1);
        username = parseCSV(rawData, 2);
        password = parseCSV(rawData, 3);
        type = parseCSV(rawData, 4);

        if (rawData != "") {
            userfullNames[usersCount] = fullname;
            usernames[usersCount] = username;
            passwords[usersCount] = password;
            userTypes[usersCount] = type;

            usersCount++;
        }
    }
    usersFileHandle.close();

    rawData = "";
    fstream sellersFileHandle;
    sellersFileHandle.open(sellersFile, ios::in);

    while (getline(sellersFileHandle, rawData)) {
        if (rawData != "") {
            sellerIds[sellingItems] = stoi(parseCSV(rawData, 1));
            itemNames[sellingItems] = parseCSV(rawData, 2);
            itemDescriptions[sellingItems] = parseCSV(rawData, 3);
            itemCategories[sellingItems] = parseCSV(rawData, 4);
            itemPrices[sellingItems] = stof(parseCSV(rawData, 5));
            itemQuantities[sellingItems] = stoi(parseCSV(rawData, 6));
            itemSold[sellingItems] = stoi(parseCSV(rawData, 7));

            sellingItems++;
        }
    }
    sellersFileHandle.close();

    rawData = "";
    fstream cartFileHandle;
    cartFileHandle.open(cartFile, ios::in);

    while (getline(cartFileHandle, rawData)) {
        if (rawData != "") {
            cartItemIds[cartItems] = stoi(parseCSV(rawData, 1));
            cartBuyerIds[cartItems] = stoi(parseCSV(rawData, 2));
            cartItemPrices[cartItems] = stof(parseCSV(rawData, 3));
            cartItemQuantities[cartItems] = stoi(parseCSV(rawData, 4));
            cartItems++;
        }
    }
    cartFileHandle.close();

    rawData = "";
    fstream orderFileHandle;
    orderFileHandle.open(orderFile, ios::in);

    while (getline(orderFileHandle, rawData)) {
        if (rawData != "") {
            orderItemIds[orderItems] = stoi(parseCSV(rawData, 1));
            orderBuyerIds[orderItems] = stoi(parseCSV(rawData, 2));
            orderItemPrices[orderItems] = stof(parseCSV(rawData, 3));
            orderItemQuantities[orderItems] = stoi(parseCSV(rawData, 4));
            orderItemTrackingCodes[orderItems] = stoi(parseCSV(rawData, 5));
            orderDates[orderItems] = parseCSV(rawData, 6);
            orderArrivalTimes[orderItems] = stoi(parseCSV(rawData, 7));
            orderItems++;
        }
    }
    orderFileHandle.close();
}

void writeUsersData() {
    fstream usersFileHandle;
    usersFileHandle.open(usersFile, ios::app);
    usersFileHandle << userfullNames[usersCount] << "," << usernames[usersCount] << "," << passwords[usersCount] << "," << userTypes[usersCount] << endl;
    usersFileHandle.close();
}

void writeSellerData() {
    fstream sellersFileHandle;
    sellersFileHandle.open(sellersFile, ios::app);
    sellersFileHandle << sellerIds[sellingItems] << "," << itemNames[sellingItems] << "," << itemDescriptions[sellingItems] << "," << itemCategories[sellingItems] << ",";
    sellersFileHandle << itemPrices[sellingItems] << "," << itemQuantities[sellingItems] << "," << itemSold[sellingItems] << endl;
    sellersFileHandle.close();
}

void reWriteSellerData() {
    fstream sellersFileHandle;
    sellersFileHandle.open(sellersFile, ios::out);
    for (int i = 0; i < sellingItems; i++) {
        sellersFileHandle << sellerIds[i] << "," << itemNames[i] << "," << itemDescriptions[i] << "," << itemCategories[i] << ",";
        sellersFileHandle << itemPrices[i] << "," << itemQuantities[i] << "," << itemSold[i] << endl;
    }
    sellersFileHandle.close();
}

void writeCartData() {
    fstream cartFileHandle;
    cartFileHandle.open(cartFile, ios::app);
    cartFileHandle << cartItemIds[cartItems] << "," << cartBuyerIds[cartItems] << "," << cartItemPrices[cartItems] << "," << cartItemQuantities[cartItems] << endl;
    cartFileHandle.close();
}

void reWriteCartData() {
    fstream cartFileHandle;
    cartFileHandle.open(cartFile, ios::out);
    for (int i = 0; i < cartItems; i++) {
        cartFileHandle << cartItemIds[i] << "," << cartBuyerIds[i] << "," << cartItemPrices[i] << "," << cartItemQuantities[i] << endl;
    }
    cartFileHandle.close();
}

void writeOrderData() {
    fstream orderFileHandle;
    orderFileHandle.open(orderFile, ios::app);
    orderFileHandle << orderItemIds[orderItems] << "," << orderBuyerIds[orderItems] << "," << orderItemPrices[orderItems] << "," << orderItemQuantities[orderItems];
    orderFileHandle << "," << orderItemTrackingCodes[orderItems] << "," << orderDates[orderItems] << "," << orderArrivalTimes[orderItems] << endl;
    orderFileHandle.close();
}

void reWriteOrderData() {
    fstream orderFileHandle;
    orderFileHandle.open(orderFile, ios::out);
    for (int i = 0; i < orderItems; i++) {
        orderFileHandle << orderItemIds[i] << "," << orderBuyerIds[i] << "," << orderItemPrices[i] << "," << orderItemQuantities[i];
        orderFileHandle << "," << orderItemTrackingCodes[i] << "," << orderDates[i] << "," << orderArrivalTimes[i] << endl;
    }
    orderFileHandle.close();
}

bool validateLogin(string username, string password) {
    bool loginSuccessful = false;
    for (int i = 0; i < usersCount; i++) {
        if (username == usernames[i] && password == passwords[i]) {
            loginSuccessful = true;
            sessionUserIndex = i;
            break;
        }
    }
    return loginSuccessful;
}

bool uniqueUsername(string username) {
    bool isUnique = true;
    for (int i = 0; i < usersCount; i++) {
        if (username == usernames[i]) {
            isUnique = false;
            break;
        }
    }
    return isUnique;
}

bool detectCommas(string text) {
    bool containComma = false;
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == ',') {
            containComma = true;
            break;
        }
    }
    return containComma;
}

void setTerminalDimensions() {
    // setting terminal height and width
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    TERMINAL_WIDTH = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    TERMINAL_HEIGHT = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    // setting terminal type depending on size
    int bigHeaderHeight = 16;
    int bigHeaderWidth = 97;
    int smallHeaderHeight = 10;
    int smallHeaderWidth = 50;
    if (TERMINAL_HEIGHT > 35 && TERMINAL_WIDTH > 100) {
        largeTerminal = true;
        HEADER_WIDTH = bigHeaderWidth;
        HEADER_HEIGHT = bigHeaderHeight;
    } else {
        largeTerminal = false;
        HEADER_WIDTH = smallHeaderWidth;
        HEADER_HEIGHT = smallHeaderHeight;
    }
    // width to align text horizontally
    headerPadding = (TERMINAL_WIDTH - HEADER_WIDTH) / 2;
    optionsPadding = (float)15 / 100 * TERMINAL_WIDTH;
}

string parseCSV(string dataText, int index) {
    string indexValue = "";
    int currentIndex = 1;
    for (int i = 0; dataText[i] != '\0'; i++) {
        if (dataText[i] == ',') {
            currentIndex++;
        } else if (currentIndex == index) {
            indexValue += dataText[i];
        }
    }
    return indexValue;
}

string getCurrentDate() {
    char date[11];
    string dateTime = currentDateTime();
    for (int i = 0; i < 10; i++) {
        date[i] = dateTime[i];
    }
    date[10] = '\0';
    return date;
}

string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int dateDifference(string dateOne, string dateTwo) {
    return dateToDays(dateOne) - dateToDays(dateTwo);
}

int dateToDays(string date) {
    // for format YYYY-MM-DD
    string year = { date[0], date[1], date[2], date[3], '\0' };
    string month = { date[5], date[6], '\0' };
    string day = { date[8], date[9], '\0' };
    return stoi(day) + stoi(month) * 30 + (stoi(year) - 2000) * 365;
}
int generateTrackingCode() {
    int trackingCode = 10000 + (rand() % 89999);
    while (!uniqueCode(trackingCode)) {
        trackingCode = 10000 + (rand() % 89999);
    }
    return trackingCode;
}

bool uniqueCode(int code) {
    for (int i = 0; i < orderItems; i++) {
        if (code == orderItemTrackingCodes[i]) {
            return false;
        }
    }
    return true;
}

int generateArrivalTime() {
    return 1 + (rand() % 7);
}

void confirmationDialog() {
    cout << setw(optionsPadding) << "" << ">  OK" << endl;
    getch();
}

bool tryAgainDialog() {
    cout << setw(optionsPadding) << "" << "   Try Again" << endl;
    cout << setw(optionsPadding) << "" << "   Go Back" << endl;
    int option = handleOptionSelection(optionsPadding, wherey() - 2, 2, 1, false);
    return (option == 1);
}

int displayItemsPage(char displayType) {
    int itemIndex, option;
    int lastStart = -1;
    if (displayItems == 0) {
        cout << setw((TERMINAL_WIDTH - 14) / 2) << "" << "NO ITEMS FOUND" << endl << endl;
    }
    int startingY = wherey();
    int itemsPerScreen = (TERMINAL_HEIGHT - startingY - 3) / 4;
    bool onDisplayScreen = true, isScrollable = false;
    int startValue = 0;
    int endValue = (displayItems < itemsPerScreen) ? displayItems : itemsPerScreen;
    if (displayItems > itemsPerScreen) {
        gotoPosition((TERMINAL_WIDTH - 52) / 2, TERMINAL_HEIGHT - 1);
        cout << "USE RIGHT AND LEFT ARROW KEY TO SCROLL BETWEEN PAGES";
        isScrollable = true;
    }
    while (onDisplayScreen) {
        if (lastStart != startValue) {
            clearItems(startingY);
            for (int i = startValue; i < endValue; i++) {
                itemIndex = displayItemIndex[i];
                if (displayType == 'I')
                    displayItem(itemIndex);
                else if (displayType == 'C')
                    displayCartItem(itemIndex);
                else if (displayType == 'O')
                    displayOrderItem(itemIndex);
            }
            cout << setw(optionsPadding) << "" << "   Go Back" << endl;
            lastStart = startValue;
        }
        option = handleOptionSelection(optionsPadding, startingY, endValue - startValue + 1, 4, isScrollable);
        if (option == endValue - startValue + 1) {
            onDisplayScreen = false;
        } else if (option == -1) {
            if (displayItems > endValue) {
                startValue += itemsPerScreen;
                endValue += itemsPerScreen;
            }
            if (endValue > displayItems) {
                endValue = displayItems;
            }
        } else if (option == -2) {
            if (startValue != 0) {
                startValue -= itemsPerScreen;
                if (endValue % itemsPerScreen != 0) {
                    endValue -= endValue % itemsPerScreen;
                } else {
                    endValue -= itemsPerScreen;
                }
            }
        } else {
            return displayItemIndex[startValue + option - 1];
        }
    }
    return -1;
}

void displayItem(int itemIndex) {
    cout << setw(optionsPadding) << "" << "   " << itemNames[itemIndex] << endl;
    cout << setw(optionsPadding) << "" << "   PKR " << itemPrices[itemIndex] << " - ";
    cout << ((itemQuantities[itemIndex] == itemSold[itemIndex]) ? "Out-Of-Stock" : "In-Stock");
    cout << endl;
    cout << setw(optionsPadding) << "" << "   Seller: ";
    cout << ((sellerIds[itemIndex] == sessionUserIndex) ? "You" : userfullNames[sellerIds[itemIndex]]);
    cout << endl << endl;
}

void displayCartItem(int itemIndex) {
    int itemQuantity = cartItemQuantities[itemIndex];
    float itemPrice = cartItemPrices[itemIndex];
    string itemName = itemNames[cartItemIds[itemIndex]];
    string itemSeller = userfullNames[sellerIds[cartItemIds[itemIndex]]];
    cout << setw(optionsPadding) << "" << "   " << itemQuantity << "x " << itemName << endl;
    cout << setw(optionsPadding) << "" << "   Total Price: PKR " << itemQuantity * itemPrice << endl;
    cout << setw(optionsPadding) << "" << "   Seller: " << itemSeller << endl;
    cout << endl;
}

void displayOrderItem(int itemIndex) {
    int itemQuantity = orderItemQuantities[itemIndex];
    int itemCode = orderItemTrackingCodes[itemIndex];
    int itemPrice = orderItemPrices[itemIndex];
    int leftDays = orderArrivalTimes[itemIndex] - dateDifference(currentDate, orderDates[itemIndex]);
    string itemName = itemNames[orderItemIds[itemIndex]];
    cout << setw(optionsPadding) << "" << "   " << itemQuantity << "x " << itemName << endl;
    cout << setw(optionsPadding) << "" << "   " << itemCode << " - " << leftDays << " Days" << endl;
    cout << setw(optionsPadding) << "" << "   Total Price: PKR " << itemQuantity * itemPrice << endl;
    cout << endl;
}

void clearItems(int startingY) {
    gotoPosition(0, startingY);
    for (int i = startingY; i < TERMINAL_HEIGHT - 1; i++) {
        for (int j = 0; j < TERMINAL_WIDTH; j++) {
            cout << " ";
        }
        cout << endl;
    }
    gotoPosition(0, startingY);
}

void printTitle(string title) {
    system("cls");
    printHeader();
    cout << setw(optionsPadding) << "" << "DARWAZA.PK > " << title << endl << endl;
}

void printHeader() {
    if (largeTerminal) {
        printBigHeader();
    } else {
        printSmallHeader();
    }
}

void printBigHeader() {
    cout << endl << endl << endl;
    cout << setw(headerPadding) << "" << "$$$$$$$\\   $$$$$$\\  $$$$$$$\\  $$\\      $$\\  $$$$$$\\  $$$$$$$$\\  $$$$$$\\      $$$$$$$\\  $$\\   $$\\ " << endl;
    cout << setw(headerPadding) << "" << "$$  __$$\\ $$  __$$\\ $$  __$$\\ $$ | $\\  $$ |$$  __$$\\ \\____$$  |$$  __$$\\     $$  __$$\\ $$ | $$  |" << endl;
    cout << setw(headerPadding) << "" << "$$ |  $$ |$$ /  $$ |$$ |  $$ |$$ |$$$\\ $$ |$$ /  $$ |    $$  / $$ /  $$ |    $$ |  $$ |$$ |$$  / " << endl;
    cout << setw(headerPadding) << "" << "$$ |  $$ |$$$$$$$$ |$$$$$$$  |$$ $$ $$\\$$ |$$$$$$$$ |   $$  /  $$$$$$$$ |    $$$$$$$  |$$$$$  /  " << endl;
    cout << setw(headerPadding) << "" << "$$ |  $$ |$$  __$$ |$$  __$$< $$$$  _$$$$ |$$  __$$ |  $$  /   $$  __$$ |    $$  ____/ $$  $$<   " << endl;
    cout << setw(headerPadding) << "" << "$$ |  $$ |$$ |  $$ |$$ |  $$ |$$$  / \\$$$ |$$ |  $$ | $$  /    $$ |  $$ |    $$ |      $$ |\\$$\\  " << endl;
    cout << setw(headerPadding) << "" << "$$$$$$$  |$$ |  $$ |$$ |  $$ |$$  /   \\$$ |$$ |  $$ |$$$$$$$$\\ $$ |  $$ |$$\\ $$ |      $$ | \\$$\\ " << endl;
    cout << setw(headerPadding) << "" << "\\_______/ \\__|  \\__|\\__|  \\__|\\__/     \\__|\\__|  \\__|\\________|\\__|  \\__|\\__|\\__|      \\__|  \\__|" << endl;
    cout << endl << endl << endl;
    printLineBreak();
    cout << endl;
}

void printSmallHeader() {
    cout << endl << endl;
    cout << setw(headerPadding) << "" << " ___   _   _____      ___    ____  _     ___ _  __" << endl;
    cout << setw(headerPadding) << "" << "|   \\ /_\\ | _ \\ \\    / /_\\  |_  / /_\\   | _ \\ |/ /" << endl;
    cout << setw(headerPadding) << "" << "| |) / _ \\|   /\\ \\/\\/ / _ \\  / / / _ \\ _|  _/ ' < " << endl;
    cout << setw(headerPadding) << "" << "|___/_/ \\_\\_|_\\ \\_/\\_/_/ \\_\\/___/_/ \\_(_)_| |_|\\_\\" << endl;
    cout << endl << endl;
    printLineBreak();
    cout << endl;
}

void printLineBreak() {
    for (int i = 0; i < TERMINAL_WIDTH; i++) {
        cout << "*";
    }
    cout << endl;
}

int handleOptionSelection(int arrowX, int arrowY, int numberOfOptions, int offset, bool isScrollable) {
    // key codes 
    int upKey = 72;
    int downKey = 80;
    int rightKey = 77;
    int leftKey = 75;
    int enterKey = 13;
    int backspaceKey = 8;

    unsigned char key = 0;
    int movingY = arrowY;
    displayArrow(arrowX, movingY);

    while (true) {
        key = getch();
        // fix for detecting arrow keys - they return two values
        // Source: https://social.msdn.microsoft.com/Forums/vstudio/en-US/a1759ad8-56d9-487d-84c9-3020300c87a3
        if (key == 0 || key == 0xE0) {
            key = getch();
        }

        if (key == downKey) {
            removeArrow(arrowX, movingY);
            movingY += offset;
            if (movingY > arrowY + offset * (numberOfOptions - 1)) {
                movingY = arrowY;
            }
        } else if (key == upKey) {
            removeArrow(arrowX, movingY);
            movingY -= offset;
            if (movingY < arrowY) {
                movingY = arrowY + offset * (numberOfOptions - 1);
            }
        } else if (key == enterKey) {
            break;
        } else if (key == backspaceKey) {
            removeArrow(arrowX, movingY);
            movingY = arrowY + offset * (numberOfOptions - 1);
            displayArrow(arrowX, movingY);
            Sleep(80);
            return numberOfOptions;
        } else if (key >= 49 && key <= 57) {
            int keyDiff = key - 48;
            if (keyDiff <= numberOfOptions) {
                removeArrow(arrowX, movingY);
                movingY = arrowY + offset * (keyDiff - 1);
                displayArrow(arrowX, movingY);
                Sleep(80);
                return keyDiff;
            }
        }
        if (isScrollable) {
            if (key == rightKey) {
                return -1;
            } else if (key == leftKey) {
                return -2;
            }
        }
        displayArrow(arrowX, movingY);
    }
    return ((movingY - arrowY) / offset) + 1;
}

void displayArrow(int x, int y) {
    gotoPosition(x, y);
    cout << ">";
}

void removeArrow(int x, int y) {
    gotoPosition(x, y);
    cout << " ";
}

void gotoPosition(int x, int y) {
    COORD coordinates;
    coordinates.X = x;
    coordinates.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}

int wherey() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE),
        &csbi
    ))
        return -1;
    return csbi.dwCursorPosition.Y;
}

void showConsoleCursor(bool flag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = flag;
    SetConsoleCursorInfo(out, &cursorInfo);
}