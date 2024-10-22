#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include <sstream>

using namespace std;

struct LoginResult{
    bool success;
    string cardNumber;
};

struct account{
    string name;
    string cardNum;
    string pincode;
    string birthday;
    string balance;
    string contact;
    account *next;
    account(): next(NULL){}
};

class transaction{  //user info and storing it to file handling keme
    private:
        account *first;
        account *accounts;
        account *login;
        string drivepath;
    public:
        transaction() : first(NULL), accounts(NULL), login (NULL) {};
        
        bool isEmpty(){
            return (first == NULL && accounts == NULL);
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
                    ifstream file(drivepath);
                        if (file.good()){
                            cout << "Account in USB exists.";
                            return true;
                        } else {
                            cout << "Account not registered.";
                            exit(0);
                        }
                    }
                } 
            }
        cout << "\nNo removable flash drive detected." << endl;
        return false;
    }
        
        bool accountFound(string target) {
        account* validator = first;

        while (validator != NULL) {
            if (validator->cardNum == target) {
                return true;
            }
            validator = validator->next;
        }
        return false;
    }

    string decryptPin(const string& encryptedPin){
        string decryptedPin = encryptedPin;
        return decryptedPin;
    }

        void mainMenu();

        void retrieve();
        void saveToFile();
        void filetoLink(string fileName, string filePin, string fileCardNumber, string fileBalance, string fileBirthday, string fileContact);

        bool search(string acc_num, string pin);
        bool validateLoginOnBoth(string acc_num, string pin); 
        bool searchInUSB(string acc_num, string pin);
        bool validateOnBoth(string acc_num, string pin, string (*decryptFunc)(const string&));
        void idleUSB(transaction transac);
        
        void userLogin();
        void withdraw();
        void checkBal();
        void bankTrans();
        void accSett();
        void updateAccount(const account& updatedAccount);
        void updatePinInFile(const string& accountNumber, const string& newPin);
        void loginWithFlashDrive();


};


void transaction :: saveToFile(){
        ofstream file("accounts.txt");
            if(file.is_open()){
                while(file.is_open()){
                    file << first->name << endl
                    << first->pincode << endl
                    << first->cardNum << endl
                    << first->balance << endl
                    << first->birthday << endl
                    << first->contact;

                    first = first->next;


                    if (first != NULL) {
                        file << endl;
                    }

                }
                
            }else{
                cout<<"Unable to open file.";
            }
            file.close();
        }


void transaction :: filetoLink(string fileName, string filePin, string fileCardNumber, string fileBalance, string fileBirthday, string fileContact){ // pushes data of file to a linked list
        account *newaccount = new account();
            newaccount->name = fileName;
            newaccount->cardNum = fileCardNumber;
            newaccount->pincode = filePin;
            newaccount->balance = fileBalance;
            newaccount->birthday = fileBirthday;
            newaccount->contact = fileContact;
            newaccount->next = NULL;

            cout << "User: "<< newaccount->name << endl;
            cout << "Card Number: "<< newaccount->cardNum << endl;
            cout << "Pin: "<< newaccount->pincode << endl;
            cout << "Balance: "<< newaccount->balance << endl << endl;
            cout << "Birthday: "<< newaccount->birthday << endl;
            cout << "Contact Number: "<< newaccount->contact << endl << endl;
            system("pause");

            if (isEmpty()) {
                first = accounts = newaccount;
                cout << "first account" << endl;
                
            } else {
                accounts->next = newaccount;
                accounts = newaccount;
            }
        }

void transaction :: retrieve(){
    ifstream file("accounts.txt");
    if (!file.is_open()) {
        cout << "File Error" << endl;
        return;
    } 
    string fileName, filePin, fileCardNumber, fileBalance, fileBirthday, fileContact;
    while (!file.eof()){
        getline(file, fileName);
        getline(file, filePin);
        getline(file, fileCardNumber);
        getline(file, fileBalance);
        getline(file, fileBirthday);
        getline(file, fileContact);

        filetoLink(fileName, filePin, fileCardNumber, fileBalance, fileBirthday, fileContact);
    }
}

bool transaction::validateLoginOnBoth(string acc_num, string pin) {
    return search(acc_num, pin) && searchInUSB(acc_num, pin);
}

void transaction :: userLogin(){

    cout << "User: "<< first->name << endl;
            cout << "Card Number: "<< first->cardNum << endl;
            cout << "Pin: "<< first->pincode << endl;
            cout << "Balance: "<< first->balance << endl << endl;
            cout << "Birthday: "<< first->birthday << endl;
            cout << "Contact Number: "<< first->contact << endl << endl;

    string acc_num, pin;
    cout<<"Enter your account number: ";
    cin>>acc_num;
    cout<<"Enter your pin: ";
    cin>>pin;


    if (search(acc_num, pin) == true){
        cout << "\n\nLog In Successfully!!!\n";
        system("pause"); 
        // bank functions
    } else {
        cout << "\n\nAccount Number or PIN incorrect!!!\n";
        system("pause");
    }
}

bool transaction :: search(string acc_num, string pin){
    account *search = first;

    while (search != NULL){
        if (search->cardNum == acc_num){
            if(search->pincode == pin){
                login = search;
                return true;
            } else {
                return false;
            }
        }
        search = search->next;
    } 
    return false;
}
void transaction :: mainMenu(){
    cout<<"\n***************************";
    cout<<"\n\tATM MACHINE\n";
    cout<<"***************************";
    cout<<"\n1.) Login";
    cout<<"\n0.) Exit";
    cout<<"\nEnter your choice: ";
}

void transaction::withdraw() {
    float amount;
    cout << "Enter amount to withdraw: ";
    cin >> amount;

    int bal = std::stoi(login->balance);  

    if (bal >= amount) {
        bal -= amount;  

        login->balance = std::to_string(bal);  

        cout << "Withdrawal successful! Remaining balance: " << fixed << setprecision(2) << bal << endl;
    } else {
        cout << "Insufficient balance!\n";
    }
}


void transaction :: checkBal(){
    string balance;
    double bal = std::stoi(login->balance);
        if(bal > 0){
            cout<<" Your current balance is: " << login->balance << endl;
        }else{
            cout<<"Invalid balance." << endl;
        }
}

bool transaction::searchInUSB(string acc_num, string pin) {
    ifstream file(drivepath);
    if (!file.is_open()) {
        cout << "Error opening USB file." << endl;
        return false;
    }
    
    string fileCardNumber, encryptedPin, balance;
    while (file >> fileCardNumber >> encryptedPin >> balance) {
        if (fileCardNumber == acc_num) {
            // Decrypt the PIN
            string decryptedPin = decryptPin(encryptedPin);

            // Compare decrypted PIN with the user's input PIN
            if (decryptedPin == pin) {
                return true;
            }
        }
    }
    
    file.close();
    return false;
}

void transaction::bankTrans() {
    int amountrans;
    string targetcardnum;
    int bal = std::stoi(login->balance);

    while (1) {
        cout << "Enter the target account number: ";
        cin >> targetcardnum;

        if (accountFound(targetcardnum)) {
            cout << "Enter amount to transfer: ";
            cin >> amountrans;

            if (bal >= amountrans) {
                account* targetAccount = first;

                // Find the target account
                while (targetAccount != NULL) {
                    if (targetAccount->cardNum == targetcardnum) {
                        bal -= amountrans;
                        login->balance = to_string(bal);

                        int targetbal = std::stoi(targetAccount->balance);
                        targetbal += amountrans;
                        targetAccount->balance = to_string(targetbal);

                        cout << "Transfer Successful! Your remaining balance is " << bal << endl;

                        saveToFile();
                        return;
                    }
                    targetAccount = targetAccount->next;
                }
            } else {
                cout << "Insufficient Balance!" << endl;
            }
        } else {
            char retry;
            cout << "Target account not found! Would you like to try again? (Y/N): ";
            cin >> retry;

            if (retry == 'Y' || retry == 'y') {
                continue;
            } else {
                cout << "Transfer cancelled!" << endl;
                return;
            }
        }
    }
}

void transaction::updateAccount(const account& updatedAccount) {
    ifstream file("accounts.txt");
    ofstream tempFile("temp.txt");  // Temporary file for writing updated accounts

    if (file.is_open() && tempFile.is_open()) {
        string fileLine;
        while (!file.eof()) {
            // Read account details from file
            string fileName, filePin, fileCardNumber, fileBalance, fileBirthday, fileContact;
            getline(file, fileName);
            getline(file, filePin);
            getline(file, fileCardNumber);
            getline(file, fileBalance);
            getline(file, fileBirthday);
            getline(file, fileContact);

            if (fileCardNumber == updatedAccount.cardNum) {
                // Write updated account details to the temporary file
                tempFile << updatedAccount.name << endl;
                tempFile << updatedAccount.pincode << endl;  // Updated PIN
                tempFile << updatedAccount.cardNum << endl;
                tempFile << updatedAccount.balance << endl;
                tempFile << updatedAccount.birthday << endl;
                tempFile << updatedAccount.contact << endl;
            } else {
                // Write the existing details if it's not the account to be updated
                tempFile << fileName << endl;
                tempFile << filePin << endl;
                tempFile << fileCardNumber << endl;
                tempFile << fileBalance << endl;
                tempFile << fileBirthday << endl;
                tempFile << fileContact << endl;
            }

            // If not at the end, add a new line to separate accounts
            if (!file.eof()) {
                tempFile << endl;
            }
        }

        file.close();
        tempFile.close();

        // Replace original file with updated file
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");

    } else {
        cout << "Error opening file!" << endl;
    }
}

void transaction::loginWithFlashDrive() {
    string acc_num, pin;
    cout << "Enter your account number: ";
    cin >> acc_num;
    cout << "Enter your PIN: ";
    cin >> pin;

    if (validateLoginOnBoth(acc_num, pin)) {
        cout << "\n\nLog In Successfully!!!\n";
    } else {
        cout << "\n\nAccount Number or PIN incorrect!!!\n";
    }
}

void transaction::accSett() {
    string newPin;
    cout << "Enter new PIN (4 digits): ";

    while (true) {
        newPin.clear();
        char ch;
        while (newPin.length() < 4) {
            ch = _getch();
            if (isdigit(ch)) {
                newPin += ch;
                cout << '*';  
            } else if (ch == 13) { 
                break;
            } else if (ch == 8 && !newPin.empty()) {
                newPin.pop_back();
                cout << "\b \b";
            }
        }

        if (newPin.length() == 4) {
            break;
        } else {
            cout << "\nInvalid input. PIN must be exactly 4 digits." << endl;
            cout << "Enter new PIN (4 digits): ";
        }
    }

   
    login->pincode = newPin; 

    cout << "\nPIN updated successfully." << endl;

    updateAccount(*login);
}

void transaction::updatePinInFile(const string& accountNumber, const string& newPin) {
    string filePath = "D:/ATMaccounts.txt";
    ifstream inFile(filePath);
    ofstream outFile("D:/temp.txt");
    string line;
    bool found = false;

    if (!inFile.is_open() || !outFile.is_open()) {
        cout << "Error opening file!" << endl;
        return;
    }

    // Read the file line by line
    while (getline(inFile, line)) {
        stringstream ss(line);
        string accountNum, pin, balance;

        ss >> accountNum >> pin >> balance;

        if (accountNum == accountNumber) {
            found = true;
            outFile << accountNum << " " << newPin << " " << balance << endl;
        } else {
            outFile << line << endl;
        }
    }

    inFile.close();
    outFile.close();

    if (found) {
        remove(filePath.c_str());
        rename("D:/temp.txt", filePath.c_str());  
        cout << "PIN updated in ATMaccounts.txt." << endl;
    } else {
        cout << "Account not found in ATMaccounts.txt." << endl;
        remove("D:/temp.txt"); 
    }
}

bool transaction::validateOnBoth(string acc_num, string pin, string (*decryptFunc)(const string&)) {
    account* inAcc = first;
    while(inAcc != NULL) {
        if(inAcc->cardNum == acc_num) {
            if(inAcc->pincode == pin) {
                ifstream file(drivepath);
                if(!file.is_open()) {
                    cout << "Error opening file." << endl;
                    return false;
                }
                
                string line, fileCardNum, encPinUSB, decPinUSB;
                while(getline(file, line)) {
                    stringstream ss(line);
                    ss >> fileCardNum >> encPinUSB;
                    if(fileCardNum == acc_num) {
                        // Use the function pointer to decrypt
                        decPinUSB = decryptFunc(encPinUSB);
                        
                        if(decPinUSB == pin) {
                            file.close();
                            return true;
                        } else {
                            file.close();
                            return false;
                        }
                    }
                }
                file.close();
                return false;
            } else {
                return false;
            }
        }
        inAcc = inAcc->next;
    }
    return false;
}

void transaction:: idleUSB(transaction transac) {
        if (detectFlashDrive() == false) {
            system("cls");
            cout << "PLEASE INSERT A FLASH DRIVE." << endl;
            Sleep(1);
            idleUSB(transac);
    }else{
        cout << "FLASH DRIVE DETECTED";
        return;
    }
}


int main() {
    transaction transac;
    int choice;
    transac.retrieve();
    transac.idleUSB(transac);

    while (1) {
        transac.mainMenu();
        cin >> choice;
        switch (choice) {
            case 1:
                system("cls");

                // Detect flash drive before login
                if (transac.detectFlashDrive()) {
                    transac.loginWithFlashDrive();
                } else {
                    transac.userLogin();
                }

                transac.checkBal();
                transac.withdraw();
                transac.saveToFile();
                break;

            case 0:
                system("cls");
                cout << "Thank you for using this ATM";
                return 0;

            default:
                cout << "Invalid choice, Please try again.";
        }
    }
}
