#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

struct LoginResult{       //storing login results
    bool success;
    string cardNumber;
};

struct account{
    string name;
    string cardNum;
    string pincode;
    int balance;
    account *next;
};

class transaction{  //user info and storing it to file handling keme
    private:
        account *accounts;
        account *login;
    public:
        transaction() : accounts(NULL) {};
        
        bool isEmpty(){
            return (accounts==NULL);
        }
        void mainMenu();

        void retrieve();
        void saveToFile();
        void filetoLink(string fileName, string filePin, string fileCardNumber, int fileBalance);

        int search(string acc_num, string pin);


        void userLogin();
        void withdraw();
        void checkBal();
        void bankTrans();
        void accSett();
};


void transaction :: saveToFile(){     //save user info
            ofstream file("accounts.txt",ios::app);
            if(file.is_open()){
                file<< accounts->name<< endl << accounts->pincode<< endl << accounts->cardNum << endl << accounts->balance << endl;
                file.close();
            }else{
                cout<<"Unable to open file.";
            }
        }


void transaction :: filetoLink(string fileName, string filePin, string fileCardNumber, int fileBalance){ // pushes data of file to a linked list
            account *newaccount = new account();
            newaccount->name = fileName;
            newaccount->cardNum = fileCardNumber;
            newaccount->pincode = filePin;
            newaccount->balance = fileBalance;
            newaccount->next = NULL;

            cout << newaccount->name << endl;
            cout << newaccount->cardNum << endl;
            cout << newaccount->pincode << endl;
            cout << newaccount->balance << endl << endl;
            system("pause");

            if (isEmpty()){
                accounts = newaccount;
            } else {
                accounts->next = newaccount;
                accounts = newaccount;
            }
        }

void transaction :: retrieve(){
    ifstream file("accounts.txt", ios::app);
    if (!file.is_open()) {
        cout << "File Error" << endl;
        return;
    } 
    string fileName, filePin, fileCardNumber;
    int fileBalance;
    while (file >> fileName >> filePin >> fileCardNumber >> fileBalance){
        filetoLink(fileName, filePin, fileCardNumber, fileBalance);
    }
}

void transaction :: userLogin(){
    string acc_num, pin;
    cout<<"Enter your account number: ";
    cin>>acc_num;
    cout<<"Enter your pin: ";
    cin>>pin;

    if (search(acc_num, pin) == 1){
        cout << "\n\nLog In Successfully!!!\n";
        system("pause"); 
        // bank functions
    } else {
        cout << "\n\nAccount Number or PIN incorrect!!!\n";
        system("pause");
    }
}

int transaction :: search(string acc_num, string pin){
    account *search = accounts;

    while (search != NULL){
        if (search->cardNum == acc_num){
            if(search->pincode == pin){
                login = search;
                return 1;
            } else {
                return 0;
            }
        }
        search = search->next;
    } 
    return 0;
}
void transaction :: mainMenu(){
    cout<<"\n***************************";
    cout<<"\n\tATM MACHINE\n";
    cout<<"***************************";
    cout<<"\n1.) Login";
    cout<<"\n0.) Exit";
    cout<<"\nEnter your choice: ";
}

void transaction :: withdraw(){


}

void transaction :: checkBal(){
        if(login->balance > 0){
            cout<<"Your current balance is: " << login->balance << endl;
        }else{
            cout<<"MAY UTANG KA NGINAMO" << endl;
        }
}

void transaction :: bankTrans(){

}

void transaction :: accSett(){
    string newPin;
    cout << "Enter new pin: ";
    cin >> newPin;
    if(newPin.length() == 4){
        cout << "Pin updated successfully." << endl;
    }else{
        cout<<"Invalid input. Must be 4 digits." << endl;
    }

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
                break;
            case 0:
            system("cls");
            cout<<"Thank you for using this ATM";
            default:
                cout<<"Invalid choice, Please try again.";
        }
    } 
}
