// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate NodeSelectDialog
class NodeSelectDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(300, 300));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxFlexGridSizer *flexgridsizer_0 = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Node", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_0, 0, wxALL, 4);
		node = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		flexgridsizer_0->Add(node, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->Add(1, 1, 0, wxALL, 4);
		comment = new wxStaticText(parent, wxID_ANY, "-", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(comment, 1, wxALL|wxEXPAND, 4);
		flexgridsizer_0->AddGrowableRow(1, 1);
		flexgridsizer_0->AddGrowableCol(1, 1);
		boxsizer_0->Add(flexgridsizer_0, 1, wxEXPAND, 4);
		wxStdDialogButtonSizer *dialogbuttonsizer_0 = new wxStdDialogButtonSizer();
		wxButton *dialogbuttonsizer_0_ok = new wxButton(parent, wxID_OK);
		dialogbuttonsizer_0_ok->SetDefault();
		dialogbuttonsizer_0->AddButton(dialogbuttonsizer_0_ok);
		dialogbuttonsizer_0->AddButton(new wxButton(parent, wxID_CANCEL));
		dialogbuttonsizer_0->Realize();
		boxsizer_0->Add(dialogbuttonsizer_0, 0, wxALL|wxEXPAND, 4);
		parent->SetSizer(boxsizer_0);
		parent->Layout();
	}
public:
	wxChoice *node;
	wxStaticText *comment;
};
// End Generate
}
