#include "ATMFrame.h"
#include "bankfunctions.h"
#include <wx/wx.h>
#include <vector>

wxPanel* log_in;
wxPanel* bank_functions;

wxFont titleFont(80, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_EXTRABOLD);
wxFont textFont(32, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
wxFont passFont(40, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

// LOG-IN PANEL
wxButton* button1;
wxStaticText* title1;
wxTextCtrl* textbox;
wxBoxSizer* sizer;

ATMFrame::ATMFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) { // determines the size and position of the window
	log_in = new wxPanel(this); //panel as a parent 

	button1 = new wxButton(log_in, wxID_ANY, "ENTER", wxPoint(800, 450), wxSize(150, 80));// (parent, ID , position, size, style)
	title1 = new wxStaticText(log_in, wxID_ANY, "ATM NG MGA POGI", wxPoint(150, 100));
	textbox = new wxTextCtrl(log_in, wxID_ANY, "", wxPoint(300, 450), wxSize(500, 80), wxTE_PASSWORD);// (parent, ID , position, size)
	sizer = new wxBoxSizer(wxHORIZONTAL);
	
	log_in->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
	log_in->Bind(wxEVT_BUTTON, &ATMFrame::OnButtonAnyClicked, this);
	title1->SetFont(titleFont);
	button1->Bind(wxEVT_BUTTON, &ATMFrame::OnEnterClicked, this);
	textbox->SetValidator(wxTextValidator(wxFILTER_DIGITS));
	textbox->SetFont(passFont);

	this->Bind(wxEVT_CLOSE_WINDOW, &ATMFrame::OnClose, this);
	CreateStatusBar();
}

void ATMFrame::OnEnterClicked(wxCommandEvent& evt) {	
	LogIn();
	evt.Skip();
}

void ATMFrame::LogIn() {
	wxString pinField = textbox->GetValue();
	Transaction.retrieve();
	Transaction.userLogin(pinField);
	wxLogStatus("Log In Successfully");
	wxLogMessage("Lopit");
	log_in->Hide();
	BankFunctions();
}

void ATMFrame::OnButtonAnyClicked(wxCommandEvent& evt) {
	wxLogMessage("Lopit");
}

void ATMFrame::OnClose(wxCloseEvent& evt) {
	wxLogMessage("Thank you for using our service.");
	evt.Skip();
}

void ATMFrame::BankFunctions() {
	bank_functions = new wxPanel(this); //panel as a parent 
	this->SetBackgroundColour(wxColour(*wxLIGHT_GREY));
	bank_functions->Show();

	std::vector<wxButton*> buttons;
	wxGridSizer* gridSizer = new wxGridSizer(3, 2, wxSize());
	wxSizerFlags flags = wxSizerFlags().Expand(). Border(wxALL, 10);
	
	wxButton* checkbal = new wxButton(this, wxID_ANY, "CHECK BALANCE", wxDefaultPosition, wxSize(619,220));
	checkbal->SetFont(textFont);
	buttons.push_back(checkbal);

	wxButton* withdraw = new wxButton(this, wxID_ANY, "WITHDRAW CASH", wxDefaultPosition, wxSize(619, 220));
	withdraw->SetFont(textFont);
	buttons.push_back(withdraw);

	wxButton* deposit = new wxButton(this, wxID_ANY, "DEPOSIT", wxDefaultPosition, wxSize(619, 220));
	deposit->SetFont(textFont);
	buttons.push_back(deposit);

	wxButton* banktransfer = new wxButton(this, wxID_ANY, "BANK TRANSFER", wxDefaultPosition, wxSize(619, 220));
	banktransfer->SetFont(textFont);
	buttons.push_back(banktransfer);

	wxButton* accountsettings = new wxButton(this, wxID_ANY, "ACCOUNT SETTINGS", wxDefaultPosition, wxSize(619, 220));
	accountsettings->SetFont(textFont);
	buttons.push_back(accountsettings);

	wxButton* exit = new wxButton(this, wxID_ANY, "LOG OUT", wxDefaultPosition, wxSize(619, 220));
	exit->SetFont(textFont);
	buttons.push_back(exit);

	gridSizer->Add(checkbal, flags);
	gridSizer->Add(withdraw, flags);
	gridSizer->Add(deposit, flags);
	gridSizer->Add(banktransfer, flags);
	gridSizer->Add(accountsettings, flags);
	gridSizer->Add(exit, flags);

	this->SetSizer(gridSizer);
	gridSizer->SetSizeHints(this);

	exit->Bind(wxEVT_BUTTON, &ATMFrame::OnButtonAnyClicked, this);

}
