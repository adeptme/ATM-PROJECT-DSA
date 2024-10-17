#pragma once
#include <wx/wx.h>
#include "bankfunctions.h"

class ATMFrame : public wxFrame
{
public:
	transaction Transaction;
	ATMFrame(const wxString& title);
private: // event handlers
	
	void OnEnterClicked(wxCommandEvent& evt); // Event Handler, it should have reference as a parameter
	void OnButtonAnyClicked(wxCommandEvent& evt); // Event Handler, it should have reference as a parameter
	void OnClose(wxCloseEvent& evt);

	void LogIn();
	void BankFunctions();

	// log-in panel buttons
};
