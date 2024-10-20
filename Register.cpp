#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

//to store acc info

struct Account {
   std::string name;
   double balance;
   std::string pin;
   std::string birthday;
   std::string contact;
};


//to generate a random 5-digit number
std::string generateRandAccNum(){
   int accNum = rand() % 90000 + 1000;
   return std::to_string(accNum);
}

//to create account
std::string createPin() {
    std::string pin;
    bool is_valid = false;
    
    do {
        std::cout << "Set a 4-digit PIN: ";
        std::cin >> pin;
        
        if (pin.length() == 4 && std::all_of(pin.begin(), pin.end(), ::isdigit)) {
            is_valid = true;
        } else {
            std::cout << "Invalid PIN! Please enter exactly 4 numeric digits." << std::endl;
        }
    } while (!is_valid);
    
    return pin;
}

// Function to create a new account
void createAccount(std::map<std::string, Account>& accounts) {
    std::string account_number;

    do {
        account_number = generateRandomAccountNumber();
    } while (accounts.find(account_number) != accounts.end());  // Ensure it's unique

    Account account;
    std::cout << "Enter account holder's name: ";
    std::cin >> account.name;
    
    bool valid_balance = false;
    do {
        std::cout << "Enter initial balance: ";
        std::cin >> account.balance;
        if (std::cin.fail()) {
            std::cin.clear(); 
            std::cin.ignore(INT_MAX, '\n'); 
            std::cout << "Invalid balance! Please enter a valid number." << std::endl;
        } else {
            valid_balance = true;
        }
    } while (!valid_balance);
    
    account.pin = createPin();

    accounts[account_number] = account;
    std::cout << "Account created successfully! Your account number is: " << account_number << std::endl;
    saveAccounts(accounts);
}

bool verifyPin(const Account& account){
   std::string entered_pin;
   std::cout << "Enter your 4-digit PIN: ";
   std::cin >> entered_pin;
   return entered_pin == account.pin;
}

