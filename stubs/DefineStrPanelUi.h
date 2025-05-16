// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include "Resource.h"



namespace Ui {
// Generate DefineStrPanel
class DefineStrPanel {
public:
	void Setup(wxWindow *parent) {
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		is_default = new wxCheckBox(parent, wxID_ANY, "Has Default Value", wxDefaultPosition, wxDefaultSize, 0);
		boxsizer_0->Add(is_default, 0, 0, 4);
		value = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		value->SetHint("Default Value");
		boxsizer_0->Add(value, 0, wxTOP|wxEXPAND, 4);
		parent->SetSizer(boxsizer_0);
		parent->Layout();
	}
public:
	wxCheckBox *is_default;
	wxTextCtrl *value;
};
// End Generate
}
