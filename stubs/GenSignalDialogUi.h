// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate GenSignalDialog
class GenSignalDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(400, 400));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxFlexGridSizer *flexgridsizer_0 = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Type", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_0, 0, wxALL, 4);
		const wxString type_list[] = { "Receive","Send","Static","Count" };
		type = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, type_list, 0);
		type->SetSelection(0);
		flexgridsizer_0->Add(type, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_1 = new wxStaticText(parent, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_1, 0, wxALL, 4);
		name = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(name, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_2 = new wxStaticText(parent, wxID_ANY, "Variables", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_2, 0, wxALL, 4);
		variable = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		flexgridsizer_0->Add(variable, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_3 = new wxStaticText(parent, wxID_ANY, "Value", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_3, 0, wxALL, 4);
		value = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(value, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_4 = new wxStaticText(parent, wxID_ANY, "Signals", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_4, 0, wxALL, 4);
		wxBoxSizer *boxsizer_1 = new wxBoxSizer(wxHORIZONTAL);
		signals = new wxStaticText(parent, wxID_ANY, "-", wxDefaultPosition, wxDefaultSize, 0);
		boxsizer_1->Add(signals, 1, wxALL, 4);
		select = new wxButton(parent, wxID_ANY, "Choice", wxDefaultPosition, wxDefaultSize, 0);
		boxsizer_1->Add(select, 0, wxALL, 4);
		flexgridsizer_0->Add(boxsizer_1, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_5 = new wxStaticText(parent, wxID_ANY, "Hook", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_5, 0, wxALL, 4);
		hook = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(hook, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->AddGrowableRow(4, 1);
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
	wxChoice *type;
	wxTextCtrl *name;
	wxChoice *variable;
	wxTextCtrl *value;
	wxStaticText *signals;
	wxButton *select;
	wxTextCtrl *hook;
};
// End Generate
}
