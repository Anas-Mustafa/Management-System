#include <conio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;

// iomanip library usage: 
// iomanip for formatting and alignment of text on terminal
// setw(optionsPadding) >> "" before every print is for leaving
// empty space equal to value set in optionsPadding 
// and left to align text to left

int sessionUserIndex;
string currentDate;

const int ARRAY_SIZE = 50;
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

// Notifications Data
string notificationFile = "notificationData.csv";
int notificationsCount = 0;
int notificationsUserIds[ARRAY_SIZE];
string notificationsText[ARRAY_SIZE];
int userNotifications = 0;
int userNotificationsIndex[ARRAY_SIZE];

// Coupon Data
string couponFile = "couponData.csv";
int couponCount = 0;
int couponSellerIds[ARRAY_SIZE];
string coupons[ARRAY_SIZE];
int couponDiscounts[ARRAY_SIZE];

int TERMINAL_WIDTH, TERMINAL_HEIGHT;
int HEADER_WIDTH, HEADER_HEIGHT;
int TITLE_HEIGHT = 2;
int headerPadding, optionsPadding;
bool largeTerminal;

void startApplication();
void printMainScreen();

void loginUser();
void signUpUser();
void displayManual();

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
void addCoupon();
void removeCoupon();
int findCouponIndex();
bool validateCoupon(string coupon);

void openBuyerAccount();
void printBuyerMenu();
void loadUserNotifications();
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
void trackOrderExtended(int orderIndex);
void removeDeletedItemFromCarts(int itemIndex);
void removeDeletedItemFromOrders(int itemIndex);
void removeCartItem(int cartIndex);
void removeOrderItem(int orderIndex);
void displayNotificationsCount();
void viewNotifications();
void removeUserNotifications();
void addItemRemovalNotification(int index, char deletionType);
void addItemDeliveryNotification(int orderIndex);

bool validateLogin(string username, string password);
bool uniqueUsername(string username);
bool detectCommas(string text);
int applyCoupon(string coupon, int sellerId);
string getInput(string displayText, string inputType, bool canContainSpaces);

void loadFiles();
void writeUsersData();
void writeSellerData();
void reWriteSellerData();
void writeCartData();
void reWriteCartData();
void writeOrderData();
void reWriteOrderData();
void writeNotificationsData();
void reWriteNotificationsData();
void writeCouponDate();
void reWriteCouponDate();

void setTerminalDimensions();
void checkForDeliveredItems();
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
void clearDisplay(int startingY, int endingY);

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
void setColor(int color);

int main() {
    showConsoleCursor(false);
    currentDate = getCurrentDate();
    loadFiles();
    checkForDeliveredItems();
    setTerminalDimensions();
    startApplication();
}

void startApplication() {
    int options = 4, offset = 1;
    bool isScrollable = false;
    bool applicationRunning = true;
    while (applicationRunning) {
        printMainScreen();
        // get user input based on selection
        int option = handleOptionSelection(optionsPadding, wherey() - options, options, offset, isScrollable);
        if (option == 1) {
            loginUser();
        } else if (option == 2) {
            signUpUser();
        } else if (option == 3) {
            displayManual();
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
        username = getInput("Username", "AlphaNumeric", false);
        password = getInput("Password", "Password", false);
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
        // display customized for user of each type
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
        } else if (option == 6) {
            addCoupon();
        } else if (option == 7) {
            removeCoupon();
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
    // load all sellers data for displaying purposes
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
        itemName = getInput("Item Name", "AlphaNumeric", true);
        itemDescription = getInput("Item Description", "AlphaNumeric", true);
        itemPrice = stof(getInput("Item Price", "Float", false));
        itemQuantity = stoi(getInput("Item Quantity", "Integer", false));
        cout << setw(optionsPadding) << "" << "Item Category: " << endl;
        for (int i = 0; i < 8; i++) {
            cout << setw(optionsPadding) << "" << "   " << categories[i] << endl;
        }
        int arrowY = wherey();
        option = handleOptionSelection(optionsPadding, arrowY - 8, 8, 1, false);
        // setting itemCategory from option number 
        // as options are 1 indexed and array 0 indexed
        itemCategory = categories[option - 1];

        // sanitizing user input for commas as storage is CSV
        // setting true if any of input contains it
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
        // displaying extended view of selected item
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

        // displaying options for each item 
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
        // updating certain attribute based on user selection
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
        if (option == 1) {
            itemName = getInput("Updated Item Name", "AlphaNumeric", true);
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
                cout << endl;
                confirmationDialog();
                onUpdateScreen = false;
            }
        } else if (option == 2) {
            itemDescription = getInput("Updated Item Description", "AlphaNumeric", true);
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
            itemPrice = stof(getInput("Updated Item Price", "Float", false));
            cout << endl;
            itemPrices[itemIndex] = itemPrice;
            cout << setw(optionsPadding) << "" << "Item Price Updated Successfully!!!" << endl;
            confirmationDialog();
            onUpdateScreen = false;
        } else if (option == 4) {
            additionalStock = stoi(getInput("Additional Stock", "Integer", false));
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
    // an item removed from seller must be removed from
    // the all users cart and tracking order list
    int i = 0;
    removeDeletedItemFromCarts(itemIndex);
    removeDeletedItemFromOrders(itemIndex);
    // removing specified item and updating array accordingly
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
    sellingItems--;
    // loading updating date for displaying
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

void addCoupon() {
    string coupon;
    int discount;
    bool addingCoupon = true;
    while (addingCoupon) {
        int lastCouponIndex = findCouponIndex();
        printTitle("Seller > Add Coupon");
        coupon = getInput("Coupon Code", "AlphaNumeric", false);
        cout << endl;
        if (validateCoupon(coupon)) {
            discount = stoi(getInput("Discount Percentage", "Integer", false));
            if (discount < 5 || discount > 50) {
                cout << setw(optionsPadding) << "" << "Discount Amount Must be in range 5-50" << endl;
            } else {
                cout << endl;
                if (lastCouponIndex == -1) {
                    cout << setw(optionsPadding) << "" << "Your Coupon Has Been Added Successfully!!!" << endl;
                    couponSellerIds[couponCount] = sessionUserIndex;
                    coupons[couponCount] = coupon;
                    couponDiscounts[couponCount] = discount;
                    couponCount++;
                    writeCouponDate();
                } else {
                    cout << setw(optionsPadding) << "" << "Your Coupon Has Been Updated Successfully!!!" << endl;
                    couponSellerIds[lastCouponIndex] = sessionUserIndex;
                    coupons[lastCouponIndex] = coupon;
                    couponDiscounts[lastCouponIndex] = discount;
                    reWriteCouponDate();
                }
                addingCoupon = false;
            }
        }
        cout << endl;
        confirmationDialog();
    }
}

void removeCoupon() {
    printTitle("Seller > Remove Coupon");
    cout << setw(optionsPadding) << "" << "Are You Sure You want to Remove Coupon?" << endl;
    cout << setw(optionsPadding) << "" << endl;
    cout << setw(optionsPadding) << "" << "   Yes" << endl;
    cout << setw(optionsPadding) << "" << "   Cancel" << endl;
    int currentY = wherey();
    int option = handleOptionSelection(optionsPadding, currentY - 2, 2, 1, false);
    if (option == 1) {
        for (int i = 0; i < couponCount; i++) {
            if (couponSellerIds[i] == sessionUserIndex) {
                for (int j = i; j < couponCount - 1; j++) {
                    couponSellerIds[j] = couponSellerIds[j + 1];
                    coupons[j] = coupons[j + 1];
                    couponDiscounts[j] = couponDiscounts[j + 1];
                }
                couponCount--;
                reWriteCouponDate();
                break;
            }
        }
        gotoPosition(0, currentY + 2);
        cout << setw(optionsPadding) << "" << "Coupon Has Been Removed Successfully!!!" << endl;
        cout << endl;
        confirmationDialog();
    }
}

int findCouponIndex() {
    int index = -1;
    for (int i = 0; i < couponCount; i++) {
        if (couponSellerIds[i] == sessionUserIndex) {
            index = i;
            break;
        }
    }
    return index;
}

bool validateCoupon(string coupon) {
    int couponLength = 6;
    if (coupon.length() != couponLength) {
        cout << setw(optionsPadding) << "" << "Coupon Length Needs to Be 6" << endl;
        return false;
    }
    for (int i = 0; i < couponLength; i++) {
        if ((coupon[i] >= 'A' && coupon[i] <= 'Z') || (coupon[i] >= '0' && coupon[i] <= '9')) {
            continue;
        } else {
            cout << setw(optionsPadding) << "" << "Coupon Can Only Contain Capital Alphabets and Digits" << endl;
            return false;
        }
    }
    for (int i = 0; i < couponCount; i++) {
        if (coupon == coupons[i]) {
            cout << setw(optionsPadding) << "" << "This Coupon Already Exists" << endl;
            return false;
        }
    }
    return true;
}
void openBuyerAccount() {
    int currentY = 0;
    loadUserNotifications();
    int options = 8;
    int option;
    bool onBuyerScreen = true;
    while (onBuyerScreen) {
        printTitle("Buyer");
        printBuyerMenu();
        currentY = wherey();
        if (userNotifications > 0) displayNotificationsCount();
        option = handleOptionSelection(optionsPadding, currentY - options, options, 1, false);
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
        } else if (option == 7) {
            viewNotifications();
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

void loadUserNotifications() {
    // loading current user notifications from all notifications
    userNotifications = 0;
    for (int i = 0; i < notificationsCount; i++) {
        if (notificationsUserIds[i] == sessionUserIndex) {
            userNotificationsIndex[userNotifications] = i;
            userNotifications++;
        }
    }
}

void viewMarketPlace() {
    int itemIndex;
    bool onMarketPlace = true;
    while (onMarketPlace) {
        printTitle("Buyer > Market Place");
        displayItems = 0;
        // loading all items for displaying 
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
        query = getInput("Search Query", "AlphaNumeric", true);
        cout << endl;
        // loading items that satisfy search 
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
            // loading items of specified category
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
    int itemSellerId = sellerIds[itemIndex];
    int itemQuantity, currentY, option, discount;
    string title = "Order Item";
    if (orderType == 'C') {
        title = "Add to Cart";
    }
    // take user order and add it to shopping cart or place order accordingly
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
            itemQuantity = stoi(getInput("Item Quantity", "Integer", false));
            if (itemQuantity < 1 || itemQuantity > remainingItem) {
                cout << setw(optionsPadding) << "" << "ITEM QUANTITY OUT OF RANGE!!!" << endl;
                cout << setw(optionsPadding) << "" << "Valid Range is 1-" << remainingItem << endl;
            } else {
                cout << endl;
                cout << setw(optionsPadding) << "" << "   Apply Coupon" << endl;
                cout << setw(optionsPadding) << "" << "   " << (orderType == 'C' ? "Add to Shopping Cart" : "Order Now") << endl;
                currentY = wherey();
                option = handleOptionSelection(optionsPadding, currentY - 2, 2, 1, false);
                gotoPosition(0, currentY + 1);
                if (option == 1) {
                    coupon = getInput("COUPON", "AlphaNumeric", false);
                    discount = applyCoupon(coupon, itemSellerId);
                    if (discount == 0) {
                        cout << setw(optionsPadding) << "" << "INVALID COUPON!!!" << endl;
                    } else {
                        cout << setw(optionsPadding) << "" << "Coupon Applied Successfully!!!" << endl;
                        if (orderType == 'C') {
                            cout << endl;
                            cout << setw(optionsPadding) << "" << "Item Added to Cart Successfully!!!" << endl;
                            addItemToCart(itemIndex, itemQuantity, discount);
                        } else {
                            cout << endl;
                            cout << setw(optionsPadding) << "" << "Order Placed Successfully!!!" << endl;
                            placeOrder(itemIndex, itemQuantity, discount);
                        }
                        onOrderScreen = false;
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
        // loading current user shopping cart
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
        cout << setw(optionsPadding) << "" << itemName << " Checked Out Successfully!!!" << endl << endl;
        confirmationDialog();
    } else if (option == 2) {
        itemSold[cartItemIds[cartIndex]] -= cartItemQuantities[cartIndex];
        removeCartItem(cartIndex);
        reWriteCartData();
        reWriteSellerData();
        cout << setw(optionsPadding) << "" << itemName << " Removed From Cart Successfully!!!" << endl << endl;
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
    bool trackingOrder = true;
    int orderIndex;
    while (trackingOrder) {
        printTitle("Buyer > Track Order");
        displayItems = 0;
        // load current users orders
        for (int i = 0; i < orderItems; i++) {
            if (orderBuyerIds[i] == sessionUserIndex) {
                displayItemIndex[displayItems] = i;
                displayItems++;
            }
        }
        orderIndex = displayItemsPage('O');
        if (orderIndex != -1) {
            trackOrderExtended(orderIndex);
        } else {
            trackingOrder = false;
        }
    }
}

void trackOrderExtended(int orderIndex) {
    int option;
    string itemName = itemNames[orderItemIds[orderIndex]];
    string itemSeller = userfullNames[sellerIds[orderItemIds[orderIndex]]];
    int itemQuantity = orderItemQuantities[orderIndex];
    int trackingCode = orderItemTrackingCodes[orderIndex];
    // calculate days left in order arrival based on order date
    int daysLeft = orderArrivalTimes[orderIndex] - dateDifference(currentDate, orderDates[orderIndex]);
    string orderDate = orderDates[orderIndex];
    float itemPrice = orderItemPrices[orderIndex];
    float totalPrice = itemPrice * itemQuantity;
    printTitle("Buyer > Track Order > " + itemName);
    cout << setw(optionsPadding) << "" << "Item Name: " << itemName << endl;
    cout << setw(optionsPadding) << "" << "Item Seller: " << itemSeller << endl;
    cout << setw(optionsPadding) << "" << "Tracking Code: " << trackingCode << endl;
    cout << setw(optionsPadding) << "" << "Ordered Date: " << orderDate << endl;
    cout << setw(optionsPadding) << "" << "Item Arrival Days Left: " << daysLeft << endl;
    cout << setw(optionsPadding) << "" << "Item Quantity: " << itemQuantity << endl;
    cout << setw(optionsPadding) << "" << "Item Price Per Unit: PKR " << itemPrice << endl;
    cout << setw(optionsPadding) << "" << "Total Price: PKR " << totalPrice << endl;

    cout << endl;
    cout << setw(optionsPadding) << "" << "   Cancel Order" << endl;
    cout << setw(optionsPadding) << "" << "   Go Back" << endl;
    int currentY = wherey();
    option = handleOptionSelection(optionsPadding, currentY - 2, 2, 1, false);
    gotoPosition(0, currentY + 2);
    if (option == 1) {
        itemSold[orderItemIds[orderIndex]] -= orderItemQuantities[orderIndex];
        removeOrderItem(orderIndex);
        reWriteOrderData();
        reWriteSellerData();
        cout << setw(optionsPadding) << "" << "Order Cancelled Successfully!!!" << endl;
        confirmationDialog();
    }
}

void removeDeletedItemFromCarts(int itemIndex) {
    int i = 0;
    while (i < cartItems) {
        if (cartItemIds[i] == itemIndex) {
            addItemRemovalNotification(i, 'C');
            removeCartItem(i);
        } else {
            i++;
        }
    }
    reWriteCartData();
}

void removeDeletedItemFromOrders(int itemIndex) {
    int i = 0;
    while (i < orderItems) {
        if (orderItemIds[i] == itemIndex) {
            addItemRemovalNotification(i, 'O');
            removeOrderItem(i);
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

void removeOrderItem(int orderIndex) {
    for (int j = orderIndex; j < orderItems - 1; j++) {
        orderItemIds[j] = orderItemIds[j + 1];
        orderBuyerIds[j] = orderBuyerIds[j + 1];
        orderItemPrices[j] = orderItemPrices[j + 1];
        orderItemQuantities[j] = orderItemQuantities[j + 1];
        orderItemTrackingCodes[j] = orderItemTrackingCodes[j + 1];
        orderDates[j] = orderDates[j + 1];
        orderArrivalTimes[j] = orderArrivalTimes[j + 1];
    }
    orderItems--;
}

void displayNotificationsCount() {
    // goto center of the terminal in last line
    gotoPosition((TERMINAL_WIDTH - 31) / 2, TERMINAL_HEIGHT - 1);
    cout << "YOU HAVE " << userNotifications << " NEW NOTIFICATION(S)";
}

void viewNotifications() {
    printTitle("Buyer > Notifications");
    // displaying notifications if they exists
    if (userNotifications <= 0) {
        cout << setw((TERMINAL_WIDTH - 22) / 2) << "" << "NO NOTIFICATIONS FOUND" << endl << endl;
        cout << setw(optionsPadding) << "" << ">  Go Back" << endl;
        getch();
    } else {
        for (int i = 0; i < userNotifications; i++) {
            cout << setw(optionsPadding) << "" << i + 1 << ". " << notificationsText[userNotificationsIndex[i]] << endl;
        }
        cout << endl;
        cout << setw(optionsPadding) << "" << ">  Mark As Read" << endl;
        getch();
        removeUserNotifications();
    }
}

void removeUserNotifications() {
    // remove current user notifications from all notifications
    for (int i = userNotifications - 1; i >= 0; i--) {
        for (int j = userNotificationsIndex[i]; j < notificationsCount; j++) {
            notificationsUserIds[j] = notificationsUserIds[j + 1];
            notificationsText[j] = notificationsText[j + 1];
        }
        notificationsCount--;
    }
    userNotifications = 0;
    reWriteNotificationsData();
}

void addItemRemovalNotification(int index, char deletionType) {
    // create notification for item removal
    int itemQuantity;
    int userId;
    string itemName;
    string deletionMessage;
    if (deletionType == 'C') {
        userId = cartBuyerIds[index];
        itemQuantity = cartItemQuantities[index];
        itemName = itemNames[cartItemIds[index]];
        deletionMessage = " Removed From Shopping Cart";
    } else {
        userId = orderBuyerIds[index];
        itemQuantity = orderItemQuantities[index];
        itemName = itemNames[orderItemIds[index]];
        deletionMessage = " Order Cancelled";
    }
    string notificationText = to_string(itemQuantity) + "x " + itemName + deletionMessage + " as it was Removed by the Seller";
    notificationsUserIds[notificationsCount] = userId;
    notificationsText[notificationsCount] = notificationText;
    writeNotificationsData();
    notificationsCount++;
}

void addItemDeliveryNotification(int orderIndex) {
    // create item delivery notification 
    int itemQuantity = orderItemQuantities[orderIndex];
    string itemName = itemNames[orderItemIds[orderIndex]];
    string notificationText = to_string(itemQuantity) + "x " + itemName + " Has Been Delivered";
    notificationsUserIds[notificationsCount] = orderBuyerIds[orderIndex];
    notificationsText[notificationsCount] = notificationText;
    writeNotificationsData();
    notificationsCount++;
}

int applyCoupon(string coupon, int sellerId) {
    int discount = 0;
    for (int i = 0; i < couponCount; i++) {
        if (coupons[i] == coupon && couponSellerIds[i] == sellerId) {
            discount = couponDiscounts[i];
            break;
        }
    }
    return discount;
}

string getInput(string displayText, string inputType, bool canContainSpaces) {
    string temp;
    bool gettingInput = true;
    int digitsCount = 0, spaceCount = 0, specialWordCount = 0, dotCount = 0, minusCount = 0;
    int startingY = wherey();
    while (gettingInput) {
        digitsCount = 0, spaceCount = 0, specialWordCount = 0, dotCount = 0, minusCount = 0;
        clearDisplay(startingY, wherey() + 1);
        gotoPosition(0, startingY);
        cout << setw(optionsPadding) << "" << displayText << ": ";
        cin.clear();
        cin.sync();
        showConsoleCursor(true);
        getline(cin >> ws, temp);
        showConsoleCursor(false);

        for (int i = 0; i < temp.length(); i++) {
            if ((temp[i] >= 'A' && temp[i] <= 'Z') || (temp[i] >= 'a' && temp[i] <= 'z') || (temp[i] >= '0' && temp[i] <= '9') || temp[i] == ' ' || temp[i] == '.' || temp[i] == '-') {
                if (temp[i] == ' ') {
                    spaceCount++;
                } else if (temp[i] == '.') {
                    dotCount++;
                } else if (temp[i] == '-') {
                    minusCount++;
                } else if ((temp[i] >= '0' && temp[i] <= '9')) {
                    digitsCount++;
                }
            } else {
                specialWordCount++;
            }
        }
        if ((inputType == "Integer" || inputType == "Float") && minusCount == 1) {
            cout << setw(optionsPadding) << "" << displayText << " Cannot Be Negative" << endl << endl;
        } else if (inputType == "Integer" && digitsCount != temp.length()) {
            cout << setw(optionsPadding) << "" << displayText << " Can Only Contain Numbers" << endl << endl;
        } else if (inputType == "Float" && !(digitsCount == temp.length() || (digitsCount == temp.length() - 1 && dotCount == 1))) {
            cout << setw(optionsPadding) << "" << displayText << " Can Only Contain Numbers and ." << endl << endl;
        } else if (spaceCount > 0 && !canContainSpaces) {
            cout << setw(optionsPadding) << "" << displayText << " Cannot Contain Spaces" << endl << endl;
        } else if (inputType == "Password" && temp.length() < 6) {
            cout << setw(optionsPadding) << "" << displayText << " Need To Be at least of 6 characters" << endl << endl;
        } else if (specialWordCount && inputType != "Password") {
            cout << setw(optionsPadding) << "" << displayText << " Cannot Contain Special Characters" << endl << endl;
        } else if (inputType == "Alphabetic" && !(digitsCount == 0 && dotCount == 0 && minusCount == 0)) {
            cout << setw(optionsPadding) << "" << displayText << " Can Only Contain Alphabets" << endl << endl;
        } else if (inputType == "AlphaNumeric" && dotCount > 0) {
            cout << setw(optionsPadding) << "" << displayText << " Can Only Contain Alphabets And Numbers" << endl << endl;
        } else {
            gettingInput = false;
        }

        if (gettingInput) {
            confirmationDialog();
        }
    }
    return temp;
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

        gotoPosition(0, currentY + 1);
        fullname = getInput("Full Name", "Alphabetic", true);
        username = getInput("Username", "AlphaNumeric", false);
        password = getInput("Password", "Password", false);

        // validation for commas
        containCommas = detectCommas(fullname) ? true : containCommas;
        containCommas = detectCommas(username) ? true : containCommas;
        containCommas = detectCommas(password) ? true : containCommas;

        cout << endl;
        // checking for unique names
        if (uniqueUsername(username) && !containCommas) {
            cout << setw(optionsPadding) << "" << "Account Created Successfully!!!" << endl << endl;
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

void displayManual() {
    printTitle("Application Manual");
    cout << setw(optionsPadding) << "" << "Navigation System: " << endl << endl;
    cout << setw(optionsPadding) << "" << "1. Up/Down Arrow Key - Move Between Options" << endl << endl;
    cout << setw(optionsPadding) << "" << "2. Right/Left Arrow Key - Move Between Different Pages" << endl;
    cout << setw(optionsPadding) << "" << "   Right/Left Arrow Key Can Only Be Used When This Text Appears" << endl;
    cout << setw(optionsPadding) << "" << "   USE RIGHT AND LEFT ARROW KEY TO SCROLL BETWEEN PAGES" << endl << endl;
    cout << setw(optionsPadding) << "" << "3. Enter Key - Select The Option With Cursor" << endl << endl;
    cout << setw(optionsPadding) << "" << "4. Backspace Key - Go Back To Last Menu OR Exit Current Menu" << endl << endl;
    cout << setw(optionsPadding) << "" << "5. Number Keys - Select Option Corresponding to Key Pressed" << endl << endl;
    confirmationDialog();
}


void loadFiles() {
    // loading User Files
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

    // loading Seller Files
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

    // loading Cart File
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

    // loading order file
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

    // loading notification file 
    rawData = "";
    fstream notificationFileHandle;
    notificationFileHandle.open(notificationFile, ios::in);
    while (getline(notificationFileHandle, rawData)) {
        if (rawData != "") {
            notificationsUserIds[notificationsCount] = stoi(parseCSV(rawData, 1));
            notificationsText[notificationsCount] = parseCSV(rawData, 2);
            notificationsCount++;
        }
    }
    notificationFileHandle.close();

    // loading Coupons File
    rawData = "";
    fstream couponFileHandle;
    couponFileHandle.open(couponFile, ios::in);
    while (getline(couponFileHandle, rawData)) {
        if (rawData != "") {
            couponSellerIds[couponCount] = stoi(parseCSV(rawData, 1));
            coupons[couponCount] = parseCSV(rawData, 2);
            couponDiscounts[couponCount] = stoi(parseCSV(rawData, 3));
            couponCount++;
        }
    }
    couponFileHandle.close();
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

void writeNotificationsData() {
    fstream notificationFileHandle;
    notificationFileHandle.open(notificationFile, ios::app);
    notificationFileHandle << notificationsUserIds[notificationsCount] << "," << notificationsText[notificationsCount] << endl;
    notificationFileHandle.close();
}

void reWriteNotificationsData() {
    fstream notificationFileHandle;
    notificationFileHandle.open(notificationFile, ios::out);
    for (int i = 0; i < notificationsCount; i++) {
        notificationFileHandle << notificationsUserIds[i] << "," << notificationsText[i] << endl;
    }
    notificationFileHandle.close();
}

void writeCouponDate() {
    fstream couponFileHandle;
    couponFileHandle.open(couponFile, ios::app);
    couponFileHandle << couponSellerIds[couponCount] << "," << coupons[couponCount] << "," << couponDiscounts[couponCount] << endl;
    couponFileHandle.close();
}

void reWriteCouponDate() {
    fstream couponFileHandle;
    couponFileHandle.open(couponFile, ios::out);
    for (int i = 0; i < couponCount; i++) {
        couponFileHandle << couponSellerIds[i] << "," << coupons[i] << "," << couponDiscounts[i] << endl;
    }
    couponFileHandle.close();
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

void checkForDeliveredItems() {
    // check if items are delivered by finding 
    // time difference between current and order date
    // remove those items and send notification
    int i = 0, daysLeft;
    while (i < orderItems) {
        daysLeft = orderArrivalTimes[i] - dateDifference(currentDate, orderDates[i]);
        if (daysLeft <= 0) {
            addItemDeliveryNotification(i);
            removeOrderItem(i);
        } else {
            i++;
        }
    }
    reWriteOrderData();
}

string parseCSV(string dataText, int index) {
    // extracting text from index part depending on commas
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
    // generate unique random token between 10000 and 99999
    int trackingCode = 10000 + (rand() % 90000);
    while (!uniqueCode(trackingCode)) {
        trackingCode = 10000 + (rand() % 90000);
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
    // randomly generating arrival time of order between 1 and 7
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
    // display items with scrolling window
    // depending on size of screen first get number of items per page
    // display those item and depending on key press move between pages
    // calculate item index when enter is pressed and return it
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
            clearDisplay(startingY, TERMINAL_HEIGHT - 1);
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

void clearDisplay(int startingY, int endingY) {
    gotoPosition(0, startingY);
    for (int i = startingY; i < endingY; i++) {
        for (int j = 0; j < TERMINAL_WIDTH; j++) {
            cout << " ";
        }
        cout << endl;
    }
    gotoPosition(0, startingY);
}

void printTitle(string title) {
    system("cls");
    setColor(013);
    printHeader();
    setColor(012);
    cout << setw(optionsPadding) << "" << "DARWAZA.PK > " << title << endl << endl;
    setColor(07);
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
            // reseting position for out of range position
            if (movingY > arrowY + offset * (numberOfOptions - 1)) {
                movingY = arrowY;
            }
        } else if (key == upKey) {
            removeArrow(arrowX, movingY);
            movingY -= offset;
            // reseting position for out of range position
            if (movingY < arrowY) {
                movingY = arrowY + offset * (numberOfOptions - 1);
            }
        } else if (key == enterKey) {
            break;
        } else if (key == backspaceKey) { // moving arrow to option in both of these
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

void setColor(int color) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, color);
}