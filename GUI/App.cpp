#include "App.h"
#include "ATMFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App); // calling implementaion function for the wxWidgets to know what class represents the application

bool App::OnInit() {
	ATMFrame* atmFrame = new ATMFrame("ATM NG MGA POGI");
	atmFrame->SetMinClientSize(wxSize(1280,720));
	atmFrame->SetMaxClientSize(wxSize(1280,720));
	atmFrame->Show();
	return true; // return true for process to continue, false to close immediately
}
