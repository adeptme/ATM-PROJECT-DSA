#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <limits>
#include <algorithm>
#include <windows.h>
#include <conio.h>

using namespace std;

struct Account {
    string name;
    string balance;
    string cardNum;
    string pin;
    string birthday;
    string contact;
    Account *next;
    Account() : next(NULL) {}
};


string encrypt(string pin) {           // encryption
    for (int i = 0; i < pin.size(); i++) {
        pin[i] = pin[i] + 200;
    }
    return pin;
}

string decrypt(string pin) {           // decryption
    for (int i = 0; i < pin.size(); i++) {
        pin[i] = pin[i] - 200;
    }
    return pin;
}

class Create {
private:

    Account* first; // points to first account
    Account* last; // points to last account

    map<string, Account> accounts;
    string drivepath;

    string generateRandAccNum() {
        int accNum = rand() % 90000 + 10000;
        //validator
        return to_string(accNum);
    }

    string createPin() {
        string pin;
        char ch;

        cout << "Enter PIN (4 or 6 digits). Press Enter to continue: ";

        while (true) {
            ch = _getch();

            if (ch == 8) {              // backspace ASCII code
                if (!pin.empty()) {
                    pin.pop_back();
                    cout << "\b \b";  // delete the last character from display
                }
            }
            else if (isdigit(ch)) {
                if (pin.length() < 6) {
                    pin += ch;
                    cout << '*';
                }
            }
            else if (ch == 13) {         // enter ASCII code
                if (pin.length() == 4 || pin.length() == 6) {
                    break;
                }
                else {
                    cout << "\nInvalid input. PIN must be 4 or 6 digits.\n";
                    pin.clear();  // Clear the PIN and restart input
                    cout << "Enter PIN (4 or 6 digits). Press Enter to continue: ";
                }
            }
        }

        return pin;
    }

public:
    Create () : first(NULL), last(NULL) {};

    void detect() {
        if (!detectFlashDrive()) {
            cout << "No Flash Drive Detected!" << endl;
            system("pause");
        }
        else {
            retrieveAccounts();
        }
    }

    bool isEmpty() {
        return (first == NULL && last == NULL);
    }

    bool detectFlashDrive() {
        DWORD fd = GetLogicalDrives();
        cout << "Flash drive detected: ";
        for (char drive = 'D'; drive <= 'Z'; drive++) {
            if (fd & (1 << (drive - 'A'))) {
                string fdpath = string(1, drive) + ":/";

                cout << fdpath << " "; // debug

                if (GetDriveTypeA(fdpath.c_str()) == DRIVE_REMOVABLE) {
                    cout << "\nFlash drive detected at: " << fdpath << endl;
                    drivepath = fdpath + "ATMaccount.txt";
                    return true;
                }
            }
        }
        cout << "\nNo removable flash drive detected." << endl;
        return false;
    }

    bool verifyPin(const Account& account) {
        string entered_pin;
        cout << "Enter your 4 or 6-digit PIN: ";

        char ch;
        while (true) {
            ch = _getch();

            if (ch == 8) {                    // backspace ASCII code
                if (!entered_pin.empty()) {
                    entered_pin.pop_back();
                    cout << "\b \b";
                }
            }
            else if (isdigit(ch)) {
                if (entered_pin.length() < 6) {
                    entered_pin += ch;
                    cout << '*';
                }
            }
            else if (ch == 13) {                // enter ASCII code
                if (entered_pin.length() == 4 || entered_pin.length() == 6) {
                    break;
                }
                else {
                    cout << "\nInvalid input. PIN must be 4 or 6 digits.";
                    entered_pin.clear();
                    cout << "\nEnter your 4 or 6-digit PIN: ";
                }
            }
        }
        return entered_pin == decrypt(account.pin);
    }

    void saveAccounts() {
        if (drivepath.empty()) {
            cout << "The Flash Drive is not yet inserted." << endl;
            system("pause");
            return;
        }

        cout << "Saving accounts to: " << drivepath << endl; // debug

        ofstream createFile(drivepath);
        if (!createFile) {
            cout << "Error in opening the file" << endl;
            system("pause");
            return;
        }

        if (isEmpty()) {
            cout << "No accounts to save. The accounts map is empty." << endl;
            system("pause");
            return;
        }

        for (const auto& pair : accounts) {
            createFile << pair.first << " " << encrypt(pair.second.pin) << endl;
        }
        createFile.close();
        cout << "Accounts saved successfully to: " << drivepath << endl; // debug
    }

    void retrieveFromDatabase(){
        ifstream file("accounts.txt", ios::app);
            if (!file.is_open()) {
                cout << "File Error" << endl;
                return;
            }
            string fileName, filePin, fileCardNumber, fileBalance, fileBirthday, fileContactNum;
            while (!file.eof()) {
            getline(file, fileName);
            getline(file, filePin);
            getline(file, fileCardNumber);
            getline(file, fileBalance);
            getline(file, fileBirthday);       // Read birthday
            getline(file, fileContactNum);     // Read contact number
            DatatoLink(fileName, filePin, fileCardNumber, fileBalance, fileBirthday, fileContactNum);
        }
    }

void DatatoLink(string fileName, string filePin, string fileCardNum, string fileBalance, string fileBirthday, string fileContactNum){
    Account* newAccount = new Account();
    newAccount->name = fileName;
    newAccount->balance = fileBalance;
    newAccount->cardNum = fileCardNum;
    newAccount->pin = filePin;
    newAccount->birthday = fileBirthday;       // Set birthday
    newAccount->contact = fileContactNum;   // Set contact number
    newAccount->next = NULL;

    // Debugging the content of the new account
    cout << "User: " << newAccount->name << endl;
    cout << "Card Number: " << newAccount->cardNum << endl;
    cout << "Pin: " << newAccount->pin << endl;
    cout << "Balance: " << newAccount->balance << endl;
    cout << "Birthday: " << newAccount->birthday << endl;  // Display birthday
    cout << "Contact Number: " << newAccount->contact << endl;  // Display contact number
    system("pause");

    if (isEmpty()) {
        first = last = newAccount;
    } else {
        last->next = newAccount;
        last = newAccount;
    }
}
    
    void retrieveAccounts() {
        ifstream createFile(drivepath);
        if (!createFile) {
            cout << "No data" << endl;
            system("pause");
            return;
        }

        string accnum, pin;
        while (createFile >> accnum >> pin) {
            Account acc;
            acc.pin = decrypt(pin);
            accounts[accnum] = acc;
        }
        createFile.close();
    }

    void createAccount() {
    string tName, tPin, tCardNum, tBalance, tBirthday, tContactNum;

    // Generate a unique account number
    do {
        tCardNum = generateRandAccNum();
    } while (accounts.find(tCardNum) != accounts.end());  // Check if the account number already exists

    cout << "Enter account holder's name: ";
    //cin.ignore();
    getline(cin, tName);

    bool valid_balance = false;
    do {
        cout << "Enter initial balance: ";
        cin >> tBalance;
        int balanceamount = std::stoi(tBalance);
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid balance! Please enter a valid number." << endl;
        }
        else if (balanceamount < 0) {
            cout << "Balance cannot be negative!" << endl;
        }
        else {
            valid_balance = true;
        }
        tBalance = std::to_string(balanceamount);
    } while (!valid_balance);

    tPin = createPin();

    cout << "\nEnter Birthday (MMDDYYYY): "; cin >> tBirthday;
    cout << "\nEnter Contact Number (+63): "; cin >> tContactNum;
    
    cout << "\nAccount created successfully! \nYour account number is: " << tCardNum << endl;

    // Add the new account to the linked list
    LinktoDatabase(tName, tPin, tCardNum, tBalance, tBirthday, tContactNum); // reuse function

    // Save the updated accounts to file
    saveAccounts();  // This saves the current accounts to the flash drive
}

void LinktoDatabase(string fileName, string filePin, string fileCardNum, string fileBalance, string fileBirthday, string fileContactNum) {
        ofstream file("accounts.txt", ios::app); // Open in append mode
        if (file.is_open()) {
            Account* current = last;
        string tName, tPin, tCardNum, tBalance, tBirthday, tContactNum;
            while (current != NULL) {
                file << endl;
                file << fileName << endl
                    << filePin << endl
                    << fileCardNum << endl
                    << fileBalance << endl
                    << fileBirthday << endl  // Save birthday
                    << fileContactNum;       // Save contact number

                current = current->next;
                
                if (current != NULL) {
                    file << endl; // Add a new line between accounts
                }
            }
            file.close();  // Close the file after writing
        } else {
            cout << "Unable to open file." << endl;
        }
    }
};


int main() {
    srand(time(0));
    Create c;
    c.detect();
    c.retrieveFromDatabase();
    c.createAccount();

    return 0;
}