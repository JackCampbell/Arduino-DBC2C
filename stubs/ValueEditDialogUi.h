// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate ValueEditDialog
class ValueEditDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(300, 200));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxFlexGridSizer *flexgridsizer_0 = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Value", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_0, 0, wxALL, 4);
		value = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		value->SetHint("0");
		flexgridsizer_0->Add(value, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_1 = new wxStaticText(parent, wxID_ANY, "Desc", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_1, 0, wxALL, 4);
		desc = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		desc->SetHint("Reserved");
		flexgridsizer_0->Add(desc, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->AddGrowableCol(1, 1);
		boxsizer_0->Add(flexgridsizer_0, 1, wxALL|wxEXPAND, 4);
		wxStdDialogButtonSizer *dialogbuttonsizer_0 = new wxStdDialogButtonSizer();
		wxButton *dialogbuttonsizer_0_ok = new wxButton(parent, wxID_OK);
		dialogbuttonsizer_0_ok->SetDefault();
		dialogbuttonsizer_0->AddButton(dialogbuttonsizer_0_ok);
		dialogbuttonsizer_0->AddButton(new wxButton(parent, wxID_CANCEL));
		dialogbuttonsizer_0->Realize();
		boxsizer_0->Add(dialogbuttonsizer_0, 0, wxALL|wxEXPAND, 4);
		parent->SetSizer(boxsizer_0);
		parent->Fit();
	}
public:
	wxTextCtrl *value;
	wxTextCtrl *desc;
};
// End Generate
}
