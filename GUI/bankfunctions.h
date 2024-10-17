#pragma once

#include <iostream>
#include <string.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <wx/wx.h>

using namespace std;


struct LoginResult {       //storing login results
    bool success;
    wxString cardNumber;
};

struct account {
    wxString name;
    wxString cardNum;
    wxString pincode;
    int balance;
    account* next;

    account(): balance(0), next(NULL){}
};

class transaction {  //user info and storing it to file handling keme
private:
    account* accounts;
    account* login;
public:
    transaction() : accounts(NULL), login(NULL) {};

    bool isEmpty() {
        return (accounts == NULL);
    }

    void retrieve();
    void saveToFile();
    void filetoLink(wxString fileName, wxString filePin, wxString fileCardNumber, int fileBalance);

    bool search(wxString pin);
    //log-in
    int userLogin(wxString pin);
};