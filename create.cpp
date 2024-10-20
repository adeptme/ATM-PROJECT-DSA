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
#include <unistd.h>

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
    Account* last;  // points to last account
    map<string, Account> accounts;
    string drivepath;
    string fileinusb;

    string generateRandAccNum() {
        int accNum = rand() % 90000 + 10000;
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
    Create() : first(NULL), last(NULL) {}

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
                    drivepath = fdpath;
                    fileinusb = drivepath + "ATMaccount.txt";

                    if (fileExists(fileinusb)) {
                        cout << "CARD is registered" << endl; 
                        cout << "Terminating Program" << endl;
                        exit(0);
                    }
                    return true;
                }
            }
        }
        cout << "\nNo removable flash drive detected." << endl;
        return false;
    }

    bool fileExists(const string& filePath) {
        ifstream file(filePath);
        return file.good();  // Check if the file stream is valid (i.e., file exists)
    }

    bool isAccountExistingLocally(const string& cardNum) {
        ifstream file("accounts.txt");
        if (!file.is_open()) {
            cout << "Unable to open the local accounts file." << endl;
            return false;
        }

        string fileName, filePin, fileCardNumber, fileBalance, fileBirthday, fileContactNum;
        while (getline(file, fileName)) {
            getline(file, filePin);
            getline(file, fileCardNumber);
            getline(file, fileBalance);
            getline(file, fileBirthday);
            getline(file, fileContactNum);

            if (fileCardNumber == cardNum) {
                cout << "Account with Card Number: " << cardNum << " already exists locally." << endl;
                file.close();
                return true;
            }
        }

        file.close();
        return false;
    }

    void createFileInFlashDrive() {
        if (!fileExists(fileinusb)) {
            ofstream createFile(fileinusb);
            if (createFile.is_open()) {
                cout << "File created on the flash drive: " << fileinusb << endl;
                createFile.close();
            } else {
                cout << "Error: Unable to create file on the flash drive." << endl;
            }
        } else {
            cout << "File already exists on the flash drive." << endl;
        }
    }

 /*  void saveAccounts() {
        if (drivepath.empty()) {
            cout << "The Flash Drive is not yet inserted." << endl;
            system("pause");
            return;
        }

        ofstream createFile(drivepath);
        if (!createFile.is_open()) {
            cout << "Error in opening the file" << endl;
            system("pause");
            return;
        } else if (isEmpty()) {
            cout << "No accounts to save. The accounts map is empty." << endl;
            system("pause");
            return;
        } else {
            for (const auto& pair : accounts) {
                cout << pair.first << endl;
                cout << pair.second.pin << endl;
                createFile << pair.first << " " << encrypt(pair.second.pin) << endl;
            } 
        }
        createFile.close();
        cout << "Accounts saved successfully to: " << drivepath << endl;
    }*/ 

    void createAccount() {
        string tName, tPin, tCardNum, tBalance, tBirthday, tContactNum;

        // Generate a unique account number
        do {
            tCardNum = generateRandAccNum();
        } while (accounts.find(tCardNum) != accounts.end());  // Check if the account number already exists in memory

        // Check if the account already exists locally
        if (isAccountExistingLocally(tCardNum)) {
            cout << "Account already exists locally. Please try a different account." << endl;
            return;
        }

        cout << "Enter account holder's name: ";
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
            } else if (balanceamount < 0) {
                cout << "Balance cannot be negative!" << endl;
            } else {
                valid_balance = true;
            }
            tBalance = std::to_string(balanceamount);
        } while (!valid_balance);

        tPin = createPin();

        cout << "\nEnter Birthday (MMDDYYYY): "; cin >> tBirthday;
        cout << "\nEnter Contact Number (+63): "; cin >> tContactNum;

        cout << "\nAccount created successfully! \nYour account number is: " << tCardNum << endl;

        
        ofstream file("accounts.txt", ios::app);
            file << endl << tName << endl
            << tPin << endl
            << tCardNum << endl
            << tBalance << endl
            << tBirthday << endl 
            << tContactNum;
            file.close();
            
        
        createFileInFlashDrive();// Now check and create the file on the flash drive if needed

        ofstream flashFile(fileinusb, ios::app); // Save the updated accounts to the flash drive
        if (flashFile.is_open()) {
            flashFile << tName << endl
                      << encrypt(tPin) << endl;
            flashFile.close();
            cout << "Account saved to flash drive." << endl;
        } else {
            cout << "Error: Unable to save account to flash drive." << endl;
        }
    }

    void retrieveFromDatabase() {
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
            getline(file, fileBirthday);
            getline(file, fileContactNum);
            DatatoLink(fileName, filePin, fileCardNumber, fileBalance, fileBirthday, fileContactNum);
        }
    }

    void DatatoLink(string fileName, string filePin, string fileCardNum, string fileBalance, string fileBirthday, string fileContactNum) {
        Account* newAccount = new Account();
        newAccount->name = fileName;
        newAccount->balance = fileBalance;
        newAccount->cardNum = fileCardNum;
        newAccount->pin = filePin;
        newAccount->birthday = fileBirthday;       // Set birthday
        newAccount->contact = fileContactNum;      // Set contact number

        if (isEmpty()) {
            first = last = newAccount;
        } else {
            last->next = newAccount;
            last = newAccount;
        }
        accounts[fileCardNum] = *newAccount;
    }

    void LinktoDatabase(string Name, string Pin, string CardNum, string Balance, string Birthday, string ContactNum) {
        ofstream file("accounts.txt", ios::app);
         if (file.is_open()) {
        Account* current = first;

        while (current != NULL) {
            file << current->name << endl
                 << current->pin << endl
                 << current->cardNum << endl
                 << current->balance << endl
                 << current->birthday << endl  // Save birthday
                 << current->contact;       // Save contact number

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

    Create atm;
    atm.retrieveFromDatabase();

    if (atm.detectFlashDrive()) {
        atm.createAccount();
    }

    return 0;
}
