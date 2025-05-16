// Generate UiDesigner 1.2
#pragma once
#include <wx/sizer.h>
#include <wx/stc/stc.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate GenPreviewDialog
class GenPreviewDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(800, 800));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		source = new wxStyledTextCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		boxsizer_0->Add(source, 1, wxALL|wxEXPAND, 4);
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
	wxStyledTextCtrl *source;
};
// End Generate
}
