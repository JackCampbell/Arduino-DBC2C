// Generate UiDesigner 1.2
#pragma once
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate GenChannelDialog
class GenChannelDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(400, 200));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxFlexGridSizer *flexgridsizer_0 = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_1 = new wxStaticText(parent, wxID_ANY, "File", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_1, 0, wxALL, 4);
		name = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		flexgridsizer_0->Add(name, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_5 = new wxStaticText(parent, wxID_ANY, "Protocol", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_5, 0, wxALL, 4);
		protocol = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		flexgridsizer_0->Add(protocol, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_2 = new wxStaticText(parent, wxID_ANY, "Baudrate", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_2, 0, wxALL, 4);
		baudrate = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(baudrate, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Target", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_0, 0, wxALL, 4);
		target = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(target, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_4 = new wxStaticText(parent, wxID_ANY, "SPT", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_4, 0, wxALL, 4);
		spt = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		spt->SetHint("0 - 100");
		flexgridsizer_0->Add(spt, 0, wxALL|wxEXPAND, 4);
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
	wxChoice *name;
	wxChoice *protocol;
	wxTextCtrl *baudrate;
	wxTextCtrl *target;
	wxTextCtrl *spt;
};
// End Generate
}
