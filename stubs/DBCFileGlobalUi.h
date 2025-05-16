// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include "AttributePanel.h"
#include "Resource.h"



namespace Ui {
// Generate DBCFileGlobal
class DBCFileGlobal {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(400, 400));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxFlexGridSizer *flexgridsizer_0 = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Version", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_0, 0, wxALL, 4);
		version = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(version, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_1 = new wxStaticText(parent, wxID_ANY, "Baudrate", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_1, 0, wxALL, 4);
		baudrate = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(baudrate, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->Add(1, 1, 0, wxALL, 4);
		btr1 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		btr1->SetHint("BTR1");
		flexgridsizer_0->Add(btr1, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->Add(1, 1, 0, wxALL, 4);
		btr2 = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		btr2->SetHint("BTR2");
		flexgridsizer_0->Add(btr2, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->AddGrowableCol(1, 1);
		boxsizer_0->Add(flexgridsizer_0, 0, wxALL|wxEXPAND, 4);
		wxNotebook *notebook_0 = new wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		comment = new wxTextCtrl(notebook_0, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxBORDER_NONE);
		notebook_0->AddPage(comment, "Comment");
		attrs = new ::AttributePanel(notebook_0, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		notebook_0->AddPage(attrs, "Attributes");
		boxsizer_0->Add(notebook_0, 1, wxALL|wxEXPAND, 4);
		parent->SetSizer(boxsizer_0);
		parent->Fit();
	}
public:
	wxTextCtrl *version;
	wxTextCtrl *baudrate;
	wxTextCtrl *btr1;
	wxTextCtrl *btr2;
	wxTextCtrl *comment;
	::AttributePanel *attrs;
};
// End Generate
}
