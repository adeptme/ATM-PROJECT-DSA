
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <conio.h>

using namespace std;

struct LoginResult{       //storing login results
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
        account *first; //points to first
        account *accounts; //points to last 
        account *login;
    public:
        transaction() : first(NULL), accounts(NULL), login (NULL) {};
        
        bool isEmpty(){
            return (first == NULL && accounts == NULL);
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

        void mainMenu();

        void retrieve();
        void saveToFile();
        void filetoLink(string fileName, string filePin, string fileCardNumber, string fileBalance, string fileBirthday, string fileContact);

        bool search(string acc_num, string pin);


        void userLogin();
        void withdraw();
        void checkBal();
        void bankTrans();
        void accSett();
        void updateAccount(const account& updatedAccount);
};


void transaction :: saveToFile(){     //save user info
        ofstream file("accounts.txt");
            if(file.is_open()){
                while(file.is_open()){
                    file << first->name << endl
                    << first->pincode << endl
                    << first->cardNum << endl
                    << first->balance << endl
                    << first->birthday << endl  // Save birthday
                    << first->contact;

                    first = first->next;


                    if (first != NULL) {
                        file << endl; // Add a new line between accounts
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
    double balance;
        if(balance > 0){
            cout<<" Your current balance is: " << login->balance << endl;
        }else{
            cout<<"MAY UTANG KA NGINAMO" << endl;
        }
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

    // Assuming you want to encrypt the new PIN (optional)
    login->pincode = newPin;  // Directly update the PIN in the logged-in account

    cout << "\nPIN updated successfully." << endl;

    // Save the updated account to the file
    updateAccount(*login);  // Save changes to the file
}


int main(){
    transaction transac;
    int choice;
    transac.retrieve();

    while(1){
        transac.mainMenu();
        cin>>choice;
        switch(choice){
            case 1:
            system("cls");
            transac.userLogin();
            transac.checkBal();
            transac.accSett();
            transac.saveToFile();

                break;
            case 0:
            system("cls");
            cout<<"Thank you for using this ATM";
            default:
                cout<<"Invalid choice, Please try again.";
        }
    } 
}
