// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate AttributeDialog
class AttributeDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(400, 240));
		layout = new wxBoxSizer(wxVERTICAL);
		form = new wxGridBagSizer(0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Attribute", wxDefaultPosition, wxDefaultSize, 0);
		form->Add(statictext_0, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL|wxALIGN_CENTER_VERTICAL, 4);
		names = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		form->Add(names, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		type = new wxStaticText(parent, wxID_ANY, "Type", wxDefaultPosition, wxDefaultSize, 0);
		form->Add(type, wxGBPosition(1, 1), wxGBSpan(1, 1), wxALL, 4);
		wxStaticText *noname = new wxStaticText(parent, wxID_ANY, "Value", wxDefaultPosition, wxDefaultSize, 0);
		form->Add(noname, wxGBPosition(2, 0), wxGBSpan(2, 1), wxALL|wxALIGN_CENTER_VERTICAL, 4);
		value = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		form->Add(value, wxGBPosition(2, 1), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		enums = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		form->Add(enums, wxGBPosition(3, 1), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		form->AddGrowableCol(1, 1);
		layout->Add(form, 1, wxALL|wxEXPAND, 4);
		wxStdDialogButtonSizer *dialogbuttonsizer_0 = new wxStdDialogButtonSizer();
		wxButton *dialogbuttonsizer_0_ok = new wxButton(parent, wxID_OK);
		dialogbuttonsizer_0_ok->SetDefault();
		dialogbuttonsizer_0->AddButton(dialogbuttonsizer_0_ok);
		dialogbuttonsizer_0->AddButton(new wxButton(parent, wxID_CANCEL));
		dialogbuttonsizer_0->Realize();
		layout->Add(dialogbuttonsizer_0, 0, wxALL|wxEXPAND, 4);
		parent->SetSizer(layout);
		parent->Layout();
	}
public:
	wxBoxSizer *layout;
	wxGridBagSizer *form;
	wxChoice *names;
	wxStaticText *type;
	wxTextCtrl *value;
	wxChoice *enums;
};
// End Generate
}
