#include "bankfunctions.h"
#include <wx/wx.h>

void transaction::saveToFile() {     //save user info
    ofstream file("accounts.txt", ios::app);
    if (file.is_open()) {
        file << accounts->name << endl << accounts->pincode << endl << accounts->cardNum << endl << accounts->balance << endl;
        file.close();
    }
    else {
        cout << "Unable to open file.";
    }
}


void transaction::retrieve() {
    ifstream file("accounts.txt", ios::app);
    if (!file.is_open()) {
        cout << "File Error" << endl;
        return;
    }
    string fileName, filePin, fileCardNumber;
    int fileBalance;
    while (file >> fileName >> filePin >> fileCardNumber >> fileBalance) {
        wxString name = fileName;
        wxString pin = filePin;
        wxString CardNumber = fileCardNumber;
        filetoLink(name, pin, CardNumber, fileBalance);
    }
}

void transaction::filetoLink(wxString fileName, wxString filePin, wxString fileCardNumber, int fileBalance) { // pushes data of file to a linked list
    account* newaccount = new account();

    newaccount->name = fileName;
    newaccount->cardNum = fileCardNumber;
    newaccount->pincode = filePin;
    newaccount->balance = fileBalance;
    newaccount->next = NULL;

    if (isEmpty()) {
        accounts = newaccount;
    }
    else {
        accounts->next = newaccount;
        accounts = newaccount;
    }
}

int transaction::userLogin(wxString pin) {

    wxMessageBox(pin);
    if (search(pin)) {
        wxMessageBox("Log In Successfully!!!");
        return 1;
    }
    else {
        wxMessageBox("MALI KA BOSS");
        return 0;
    }
}

bool transaction::search(wxString pin) {
    account* search = accounts;

    while (search != nullptr){
            if (search->pincode == pin) {
                login = search;
                return true;
            }
        search = search->next;
    }
    return false;
}
