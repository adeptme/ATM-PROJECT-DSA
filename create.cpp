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
   double balance;
   string pin;
   string birthday;
   string contact;
};
string encrypt(string pin){           //encryption
    for(int i=0; i<pin.size(); i++){
        pin[i] = pin[i] + 200;
    }
    return pin;
}
string decrypt(string pin){           //decryption
    for(int i=0; i<pin.size(); i++){
        pin[i] = pin[i] - 200;
    }
    return pin;
}
class create {
    private:
    map<string, Account> accounts;
    string drivepath;

    string generateRandAccNum(){
        int accNum = rand() % 90000 + 10000;
        return to_string(accNum);
    }

    string createPin(){
        string pin;
        char ch;

        cout << "Enter PIN (4 or 6 digits). Press Enter to continue: ";

        while(true){
            ch = _getch();

            if(ch == 8){              // backspace ASCII code
                if(!pin.empty()){
                    pin.pop_back();
                    cout << "\b \b";  // delete the last character from display
                }
            }else if(isdigit(ch)){
                if(pin.length() < 6){
                    pin += ch;
                    cout << '*';
                }
            }else if(ch == 13){         // enter ASCII code
                if(pin.length() == 4 || pin.length() == 6){
                    break;
                }else{
                    cout << "\nInvalid input. PIN must be 4 or 6 digits.\n";
                    pin.clear();
                    cout << "Enter PIN (4 or 6 digits). Press Enter to continue: ";
                }
            }
        }return pin;
    }
    public:
    bool detectFlashDrive(){
        DWORD fd = GetLogicalDrives();
        for(char drive = 'D'; drive <= 'Z'; drive++){
            if(fd & (1 << (drive - 'A'))){
                string fdpath = string(1, drive) + ":/";
                if(GetDriveTypeA(fdpath.c_str()) == DRIVE_REMOVABLE){
                    system("cls");
                    cout << "\nFlash drive detected at: " << fdpath << endl;
                    drivepath = fdpath + "accounts.txt";
                    return true;
                }
            }
        }return false;
    }
    void saveAccounts(){
        if(drivepath.empty()){
            cout << "The Flash Drive is not yet inserted." << endl;
            system("pause");
            return;
        }
        ofstream createFile(drivepath);
        if(!createFile){
            cout << "Error in opening the file" << endl;
            system("pause");
            return;
        }


        //debug
        if (accounts.empty()){
            cout << "No accounts to save. The accounts map is empty." << endl;

            system("pause");
            return;
        }for(const auto& pair: accounts){
            //debug
            cout << "Saving account: " << pair.first << " with PIN: " << pair.second.pin << endl;
            createFile << pair.first << " " << encrypt(pair.second.pin) << endl;
        }createFile.close();
        cout << "Accounts saved successfully to: " << drivepath << endl; //debug
    }
    void retrieveAccounts(){
        ifstream createFile(drivepath);
        if(!createFile){
            cout << "No data" << endl;
            system("pause");
            return;
        }

        string accnum, pin;

        while(createFile >> accnum >> pin){
            Account acc;
            acc.pin = decrypt(pin);
            accounts[accnum] = acc;
        }createFile.close();
    }
        void createAccount() {

        string account_number;

        do{
            account_number = generateRandAccNum();
        }while(accounts.find(account_number) != accounts.end());

        Account account;

        cout << "\nEnter account holder's name: ";
        cin >> account.name;

        bool valid_balance = false;
        do{
            cout << "Enter initial balance: ";
            cin >> account.balance;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid balance! Please enter a valid number." << endl;
            }else if(account.balance < 0) {
                cout << "Balance cannot be negative!" << endl;
            }else{
                valid_balance = true;
            }
        }while(!valid_balance);

        account.pin = createPin();

        accounts[account_number] = account;
        cout << "\nAccount created successfully! \nYour account number is: " << account_number << endl;
        //debug
        if(accounts.find(account_number) != accounts.end()){
            cout << "Account added: " << account_number << endl;
        }else{
            cout<<"Account not added." << endl;
        }saveAccounts();
    }
    bool verifyPin(const Account& account) {
        string entered_pin;
        cout << "Enter your 4 or 6-digit PIN: ";

        char ch;
        while(true){
            ch = _getch();

            if(ch == 8){                    // backspace ASCII code
                if(!entered_pin.empty()){
                    entered_pin.pop_back();
                    cout << "\b \b";
                }
            }else if(isdigit(ch)){
                if(entered_pin.length() < 6){
                    entered_pin += ch;
                    cout << '*';
                }
            }else if(ch == 13){                // enter ASCII code
                if(entered_pin.length() == 4 || entered_pin.length() == 6){
                    break;
                }else{
                    cout << "\nInvalid input. PIN must be 4 or 6 digits.";
                    entered_pin.clear();
                    cout << "\nEnter your 4 or 6-digit PIN: ";
                }
            }
        }return entered_pin == decrypt(account.pin);
    }

    void idleUSB(create &c) {
        if (detectFlashDrive() == false) {
            system("cls");
            cout << "PLEASE INSERT A FLASH DRIVE." << endl;
            sleep(1);
            idleUSB(c);
    }else{
        return;
    }
}
};

int main() {

    class create c;
    srand(time(0));

    c.idleUSB(c);
    c.createAccount();

    return 0;
}
