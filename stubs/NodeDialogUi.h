// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include "AttributePanel.h"
#include "Resource.h"



namespace Ui {
// Generate NodeDialog
class NodeDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(400, 300));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxFlexGridSizer *flexgridsizer_0 = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_0, 0, wxALL, 4);
		name = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(name, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->AddGrowableCol(1, 1);
		boxsizer_0->Add(flexgridsizer_0, 0, wxEXPAND, 4);
		wxNotebook *notebook_0 = new wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		comment = new wxTextCtrl(notebook_0, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxBORDER_NONE);
		notebook_0->AddPage(comment, "Comment");
		attrs = new ::AttributePanel(notebook_0, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		notebook_0->AddPage(attrs, "Attributes");
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
	wxTextCtrl *name;
	wxTextCtrl *comment;
	::AttributePanel *attrs;
};
// End Generate
}
