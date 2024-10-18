#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <string>
#include <limits>
#include <algorithm> 
#include <windows.h>

using namespace std;

string x; //temp for moving encrypt to be decrypt
char ch; //storing characters

string encrypt(string pin){            //encrypting
    for(int i=0; i<pin.size(); i++){
        pin[i] = pin[i] + 200;
    }
return pin;
}

string decrypt(string pin){           //decrypting
    for(int i=0; i<pin.size(); i++){
        pin[i] = pin[i] - 200;
    }
return pin;
}

struct Account {
   string name;
   double balance;
   string pin;
   string birthday;
   string contact;
};

class create{
    private:
    map<string, Account> accounts;
    string drivepath;

    string generateRandAccNum(){
    int accNum = rand() % 90000 + 10000; 
    return to_string(accNum);
     }
    string createPin(){
    string pin;
    bool is_valid = false;

    do {
        cout << "Set a 4-digit PIN: ";
        cin >> pin;

        if (pin.length() == 4 && all_of(pin.begin(), pin.end(), ::isdigit)) {
            is_valid = true;
        } else {
            cout << "Invalid PIN! Please enter exactly 4 numeric digits." << endl;
        }
    } while (!is_valid);

    return pin;
    }
    bool detectFlashDrive(){
        DWORD fd = GetLogicalDrives();
        for(char drive = 'E'; drive <= 'Z'; drive++){
            if(fd & (1 << (drive - 'A'))){
                string fdpath = string(1, drive) + ":/";
                if(GetDriveTypeA(fdpath.c_str()) == DRIVE_REMOVABLE){
                    drivepath = fdpath + "accounts.txt";
                    return true;
                }
            }
        }
        return false;
    }

    public:
    create(){
        if(!detectFlashDrive()){
            cout << "No Flash Drive Detected!" << endl;
            system("pause");
        } else {
            retrieveAccounts();
        }
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
        for(const auto& pair: accounts){
            createFile << pair.first << " " << pair.second.pin << endl;
        }

        createFile.close();
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
            acc.pin = pin;
            accounts[accnum] = acc;
        }
        createFile.close();
    }
    void createAccount(map<string, Account>& accounts) {
    string account_number;

    do {
        account_number = generateRandAccNum();  // Corrected function name
    } while (accounts.find(account_number) != accounts.end());

    Account account;
    cout << "Enter account holder's name: ";
    cin >> account.name;

    bool valid_balance = false;
    do {
        cout << "Enter initial balance: ";
        cin >> account.balance;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clears input buffer
            cout << "Invalid balance! Please enter a valid number." << endl;
        } else if (account.balance < 0) {
            cout << "Balance cannot be negative!" << endl;
        } else {
            valid_balance = true;
        }
    } while (!valid_balance);

    account.pin = createPin();

    accounts[account_number] = account;
    cout << "Account created successfully! Your account number is: " << account_number << endl;
    }
    bool verifyPin(const Account& account) {
    string entered_pin;
    cout << "Enter your 4-digit PIN: ";
    cin >> entered_pin;
    return entered_pin == account.pin;
    }
};

int main() {

    class create c;
    srand(time(0));

    // Map to hold account details
    map<string, Account> accounts;

    c.createAccount(accounts);

    return 0;
}


