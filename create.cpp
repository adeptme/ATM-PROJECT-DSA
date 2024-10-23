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

    //map<string, Account> accounts;
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
                    pin.clear();
                    cout << "Enter PIN (4 or 6 digits). Press Enter to continue: ";
                }
            }
        }

        return pin;
    }

public:
    Create () : first(NULL), last(NULL) {};

    bool isEmpty();
    bool detectFlashDrive();
    bool verifyPin(const Account& account);
    void retrieveFromDatabase();
    void saveAccounts(string tCardNum, string tPin);
    void DatatoLink(string fileName, string filePin, string fileCardNum, string fileBalance, string fileBirthday, string fileContactNum);
    void createAccount();
    void LinktoDatabase(string fileName, string filePin, string fileCardNum, string fileBalance, string fileBirthday, string fileContactNum);
    void idleUSB(Create create);
    
};

    bool Create :: isEmpty() {
        return (first == NULL && last == NULL);
    }

    bool Create :: detectFlashDrive() {
        DWORD fd = GetLogicalDrives();
        cout << "Flash drive detected: ";
        for (char drive = 'D'; drive <= 'Z'; drive++) {
            if (fd & (1 << (drive - 'A'))) { // drive checker
                string fdpath = string(1, drive) + ":/"; // makes a string storing the path to drive

                cout << fdpath << " "; // debug

                if (GetDriveTypeA(fdpath.c_str()) == DRIVE_REMOVABLE) {
                    cout << "\nFlash drive detected at: " << fdpath << endl;
                    drivepath = fdpath + "ATMaccount.txt"; // directory of ATMAccount.txt in the drive
                    ifstream file(drivepath);
                        if (file.good()){ // exits program as file exists
                            cout << "Account already created." << endl << "Terminating Program";
                            exit(0);
                        } else { // file does not exist 
                            return true; 
                        }
                    }
                } 
            } // no flash drive found
        cout << "\nNo removable flash drive detected." << endl;
        return false;
    }

    void Create :: saveAccounts(string tCardNum, string tPin) {
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
        createFile << tCardNum << " " << encrypt(tPin); 
        createFile.close();
        cout << "Accounts saved successfully to: " << drivepath << endl; // debug
    }

    void Create :: retrieveFromDatabase(){
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
        file.close();
    }

    void Create :: DatatoLink(string fileName, string filePin, string fileCardNum, string fileBalance, string fileBirthday, string fileContactNum){
    Account* newAccount = new Account();
    newAccount->name = fileName;
    newAccount->balance = fileBalance;
    newAccount->cardNum = fileCardNum;
    newAccount->pin = filePin;
    newAccount->birthday = fileBirthday;       // Set birthday
    newAccount->contact = fileContactNum;   // Set contact number
    newAccount->next = NULL;

   /* // Debugging the content of the new account
    cout << "User: " << newAccount->name << endl;
    cout << "Card Number: " << newAccount->cardNum << endl;
    cout << "Pin: " << newAccount->pin << endl;
    cout << "Balance: " << newAccount->balance << endl;
    cout << "Birthday: " << newAccount->birthday << endl;  // Display birthday
    cout << "Contact Number: " << newAccount->contact << endl;  // Display contact number
    system("pause");*/

    if (isEmpty()) {
        first = last = newAccount;
    } else {
        last->next = newAccount;
        last = newAccount;
    }
}

    void Create :: createAccount() {
    string tName, tPin, tCardNum, tBalance, tBirthday, tContactNum;

    // Generate a unique account number
    Account* search = first;
    do {
        tCardNum = generateRandAccNum();
        search = search->next;
    } while (search != NULL);  // Check if the account number already exists

    cout << "\nEnter account holder's name: ";
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
    saveAccounts(tCardNum, tPin);  // This saves the current accounts to the flash drive
}

    void Create :: LinktoDatabase(string fileName, string filePin, string fileCardNum, string fileBalance, string fileBirthday, string fileContactNum) {
        ofstream file("accounts.txt", ios::app); // Open in append mode
        if (file.is_open()) {
            Account* current = last;
        string tName, tPin, tCardNum, tBalance, tBirthday, tContactNum;
            while (current != NULL) {
                file << fileName << endl
                    << filePin << endl
                    << fileCardNum << endl
                    << fileBalance << endl
                    << fileBirthday << endl  // Save birthday
                    << fileContactNum << endl;       // Save contact number

                current = current->next;
                
                /*if (current != NULL) {
                    file << endl; // Add a new line between accounts
                }*/
            }
            file.close();  // Close the file after writing
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    void Create :: idleUSB(Create create) {
        if (detectFlashDrive() == false) {
            system("cls");
            cout << "PLEASE INSERT A FLASH DRIVE." << endl;
            Sleep(1);
            idleUSB(create);
    }else{
        cout << "FLASH DRIVE DETECTED";
        return;
    }
}

int main() {
    Create create;
    srand(time(0));
    create.idleUSB(create);
    create.retrieveFromDatabase();
    create.createAccount();

    return 0;
}