// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include "DBCBitMap.h"
#include "AttributePanel.h"
#include "NodePanel.h"
#include "MessagePanel.h"
#include "Resource.h"



namespace Ui {
// Generate MessageDialog
class MessageDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(500, 300));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *boxsizer_1 = new wxBoxSizer(wxHORIZONTAL);
		wxFlexGridSizer *flexgridsizer_0 = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Can ID", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_0, 0, wxALL, 4);
		id = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(id, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_1 = new wxStaticText(parent, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_1, 0, wxALL, 4);
		name = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(name, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_2 = new wxStaticText(parent, wxID_ANY, "Size", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_2, 0, wxALL, 4);
		size = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(size, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_3 = new wxStaticText(parent, wxID_ANY, "Sender", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_3, 0, wxALL, 4);
		sender = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		flexgridsizer_0->Add(sender, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->AddGrowableCol(1, 1);
		boxsizer_1->Add(flexgridsizer_0, 1, wxEXPAND, 4);
		maps = new ::BitMapCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		boxsizer_1->Add(maps, 1, wxEXPAND, 4);
		boxsizer_0->Add(boxsizer_1, 0, wxALL|wxEXPAND, 4);
		wxNotebook *notebook_0 = new wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		comment = new wxTextCtrl(notebook_0, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxBORDER_NONE);
		notebook_0->AddPage(comment, "Comment");
		attrs = new ::AttributePanel(notebook_0, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		notebook_0->AddPage(attrs, "Attributes");
		transmitters = new ::NodeListPanel(notebook_0, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		notebook_0->AddPage(transmitters, "Transmitters");
		signals = new ::SignalPanel(notebook_0, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		notebook_0->AddPage(signals, "Signals");
		boxsizer_0->Add(notebook_0, 1, wxALL|wxEXPAND, 4);
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
	wxTextCtrl *id;
	wxTextCtrl *name;
	wxTextCtrl *size;
	wxChoice *sender;
	::BitMapCtrl *maps;
	wxTextCtrl *comment;
	::AttributePanel *attrs;
	::NodeListPanel *transmitters;
	::SignalPanel *signals;
};
// End Generate
}
