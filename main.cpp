#include <conio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;

const int ARRAY_SIZE = 50;
int sessionUserIndex;

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


int TERMINAL_WIDTH, TERMINAL_HEIGHT;
int HEADER_WIDTH, HEADER_HEIGHT;
int TITLE_HEIGHT = 2;
int headerPadding, optionsPadding;
bool largeTerminal;

void startApplication();
void printMainScreen();

void loginUser();
void openSellerAccount();
void printSellerMenu();
void addSellerItem();

void signUpUser();

bool validateLogin(string username, string password);
bool uniqueUsername(string username);
bool detectCommas(string text);

void loadFiles();
void writeUsersData();
void writeItemData();

void setTerminalDimensions();
string parseCSV(string dataText, int index);
void confirmationDialog();
bool tryAgainDialog();

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
        cout << endl;
        showConsoleCursor(true);
        cout << setw(optionsPadding) << "" << "Username: ";
        cin >> username;
        cout << setw(optionsPadding) << "" << "Password: ";
        cin >> password;
        showConsoleCursor(false);
        cout << endl;

        if (validateLogin(username, password)) {
            cout << setw(optionsPadding) << "" << "Login Sucessful" << endl << endl;
            onLoginScreen = false;
            loggedIn = true;
            confirmationDialog();
        } else {
            cout << setw(optionsPadding) << "" << "Login Failed" << endl;
            cout << setw(optionsPadding) << "" << "Incorrect Username/Password" << endl << endl;
            if (!tryAgainDialog()) {
                onLoginScreen = false;
            }
        }
    }
    if (loggedIn) {
        if (userTypes[sessionUserIndex] == "Seller") {
            openSellerAccount();
        } else {
            // buyerScreen();
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
    cout << setw(optionsPadding) << "" << "   Go Back" << endl;
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
        cout << endl;
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
            writeItemData();
            sellingItems++;
            addingItem = false;
            cout << setw(optionsPadding) << "" << "Item Add Sucessfully!!!" << endl << endl;
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
        cout << endl;

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
            cout << setw(optionsPadding) << "" << "Account Created Sucessfully!!!" << endl;
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
    string sellerId, itemName, itemDescription, itemCategory, itemPrice, itemQuantity, itemSoldCount;
    fstream sellersFileHandle;
    sellersFileHandle.open(sellersFile, ios::in);

    while (getline(sellersFileHandle, rawData)) {
        if (rawData != "") {
            sellerId = parseCSV(rawData, 1);
            itemName = parseCSV(rawData, 2);
            itemDescription = parseCSV(rawData, 3);
            itemCategory = parseCSV(rawData, 4);
            itemPrice = parseCSV(rawData, 5);
            itemQuantity = parseCSV(rawData, 6);
            itemSoldCount = parseCSV(rawData, 7);


            sellerIds[sellingItems] = stoi(sellerId);
            itemNames[sellingItems] = itemName;
            itemDescriptions[sellingItems] = itemDescription;
            itemCategories[sellingItems] = itemCategory;
            itemPrices[sellingItems] = stof(itemPrice);
            itemQuantities[sellingItems] = stoi(itemQuantity);
            itemSold[sellingItems] = stoi(itemSoldCount);

            sellingItems++;
        }
    }
    sellersFileHandle.close();
}

void writeUsersData() {
    fstream usersFileHandle;
    usersFileHandle.open(usersFile, ios::app);
    usersFileHandle << userfullNames[usersCount] << "," << usernames[usersCount] << "," << passwords[usersCount] << "," << userTypes[usersCount] << endl;
    usersFileHandle.close();
}

void writeItemData() {
    fstream sellersFileHandle;
    sellersFileHandle.open(sellersFile, ios::app);
    sellersFileHandle << sellerIds[sellingItems] << "," << itemNames[sellingItems] << "," << itemDescriptions[sellingItems] << "," << itemCategories[sellingItems] << ",";
    sellersFileHandle << itemPrices[sellingItems] << "," << itemQuantities[sellingItems] << "," << itemSold[sellingItems] << endl;
    sellersFileHandle.close();
}


bool validateLogin(string username, string password) {
    bool loginSucessful = false;
    for (int i = 0; i < usersCount; i++) {
        if (username == usernames[i] && password == passwords[i]) {
            loginSucessful = true;
            sessionUserIndex = i;
            break;
        }
    }
    return loginSucessful;
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
            return numberOfOptions;
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