// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate DefineDialog
class DefineDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(500, 300));
		layout = new wxBoxSizer(wxVERTICAL);
		form = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, 0);
		form->Add(statictext_0, 0, wxALL, 4);
		name = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		form->Add(name, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_1 = new wxStaticText(parent, wxID_ANY, "Object Type", wxDefaultPosition, wxDefaultSize, 0);
		form->Add(statictext_1, 0, wxALL, 4);
		object = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		form->Add(object, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_2 = new wxStaticText(parent, wxID_ANY, "Value Type", wxDefaultPosition, wxDefaultSize, 0);
		form->Add(statictext_2, 0, wxALL, 4);
		type = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		form->Add(type, 1, wxALL|wxEXPAND, 4);
		form->Add(1, 1, 0, wxALL, 4);
		enums = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		enums->SetHint("Enums");
		form->Add(enums, 1, wxALL|wxEXPAND, 4);
		form->Add(1, 1, 0, wxALL, 4);
		min = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		min->SetHint("Min");
		form->Add(min, 0, wxALL|wxEXPAND, 4);
		form->Add(1, 1, 0, wxALL, 4);
		max = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		max->SetHint("Max");
		form->Add(max, 0, wxALL|wxEXPAND, 4);
		form->Add(1, 1, 0, wxALL, 4);
		has_defs = new wxCheckBox(parent, wxID_ANY, "HasDefault", wxDefaultPosition, wxDefaultSize, 0);
		form->Add(has_defs, 0, wxALL, 4);
		wxStaticText *statictext_4 = new wxStaticText(parent, wxID_ANY, "Value", wxDefaultPosition, wxDefaultSize, 0);
		form->Add(statictext_4, 0, wxALL, 4);
		value = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		value->SetHint("");
		form->Add(value, 0, wxALL|wxEXPAND, 4);
		form->AddGrowableRow(3, 1);
		form->AddGrowableCol(1, 1);
		layout->Add(form, 1, wxEXPAND, 4);
		wxStdDialogButtonSizer *dialogbuttonsizer_0 = new wxStdDialogButtonSizer();
		wxButton *dialogbuttonsizer_0_ok = new wxButton(parent, wxID_OK);
		dialogbuttonsizer_0_ok->SetDefault();
		dialogbuttonsizer_0->AddButton(dialogbuttonsizer_0_ok);
		dialogbuttonsizer_0->AddButton(new wxButton(parent, wxID_CANCEL));
		dialogbuttonsizer_0->Realize();
		layout->Add(dialogbuttonsizer_0, 0, wxALL|wxEXPAND, 4);
		parent->SetSizer(layout);
		parent->Fit();
	}
public:
	wxBoxSizer *layout;
	wxFlexGridSizer *form;
	wxTextCtrl *name;
	wxChoice *object;
	wxChoice *type;
	wxTextCtrl *enums;
	wxTextCtrl *min;
	wxTextCtrl *max;
	wxCheckBox *has_defs;
	wxTextCtrl *value;
};
// End Generate
}
