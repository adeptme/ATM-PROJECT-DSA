#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

void mainMenu();

struct LoginResult{       //storing login results
    bool success;
    string cardNumber;
};

class UserAccount{  //user info and storing it to file handling keme
    private:
        string name;
        string pin;
        string cardNumber;
    public:
        UserAccount(string _name, string _pin){   //constructor kuno (sumunod lang ako tutorial bai)
            name= _name;
            pin= _pin;
            cardNumber=generateCardNum();  //generating random card num
        }

        string generateCardNum(){   //generating random 10-digit num
            string cardNum="";
            srand(time(0));
            for(int i=0;i<10;i++){
                cardNum+=to_string(rand()%10);
            }return cardNum;
        }

        //gayahin ko lang kay ku
        void saveToFile(){     //save user info
            ofstream file("accounts.txt",ios::app);
            if(file.is_open()){
                file<<name<<" "<<pin<<" "<< cardNumber;
                file.close();
            }else{
                cout<<"Unable to open file.";
            }
        }

        /*void saveToUsb(string name, ){
            ofstream file("")
        }*/
        
        void displayInfo(){
            cout<<"Account created successfully!\n";
            cout<<"Your card number is: "<< cardNumber;
        }

        static LoginResult login(string _name, string _pin){
            ifstream file("accounts.txt");
            string fileName, filePin, fileCardNumber;
            LoginResult result={false,""}; //initialize daw

            if(file.is_open()){
                while(file>>fileName>>filePin>>fileCardNumber){
                    if(fileName== _name && filePin== _pin){
                        result.success=true;
                        result.cardNumber=fileCardNumber;
                        break;
                    }
                }
            }else{
                cout<<"Unable to open file.";
            }return result;
        }
};

void createAccount(){
    string name, pin;
    cout<<"Enter your name: ";
    cin>>name;
    cout<<"Enter a 4-digit pin: ";
    cin>>pin;

    UserAccount newUser(name,pin);
    newUser.saveToFile();
    newUser.displayInfo();
}

void userLogin(){
    string name, pin;
    cout<<"Enter your name: ";
    cin>>name;
    cout<<"Enter your pin: ";
    cin>>pin;

    LoginResult loginResult = UserAccount::login(name, pin);

    if(loginResult.success){
        cout<<"Login successfull!\n";
        cout<<"Your card number is: "<<loginResult.cardNumber;
    }else{
        cout<<"Invalid name or pin.";
    }
}

int main(){

    int choice;

    while(1){
        mainMenu();
        cin>>choice;
        switch(choice){
            case 1:
            system("cls");
            userLogin();
                break;
            case 2:
            system("cls");
            createAccount();
                break;
            case 3:
            system("cls");
            cout<<"Thank you for using this ATM";
            default:
                cout<<"Invalid choice, Please try again.";
        }
    } 
}

void mainMenu(){
    cout<<"\n***************************";
    cout<<"\n\tATM MACHINE\n";
    cout<<"***************************";
    cout<<"\n1.) Login";
    cout<<"\n2.) Create Account";
    cout<<"\n3.) Exit";
    cout<<"\nEnter your choice: ";
}















































