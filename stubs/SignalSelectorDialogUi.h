// Generate UiDesigner 1.2
#pragma once
#include <wx/sizer.h>
#include <wx/listctrl.h>
#include <wx/toolbar.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate SignalSelectorDialog
class SignalSelectorDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(700, 400));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		active = new wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxLC_REPORT|wxBORDER_NONE|wxTAB_TRAVERSAL|wxHSCROLL);
		active->AppendColumn("Channel");
		active->AppendColumn("File");
		active->AppendColumn("Message");
		active->AppendColumn("Signal");
		active->AppendColumn("sigpath");
		boxsizer_0->Add(active, 0, wxALL|wxEXPAND, 4);
		wxToolBar *toolbar_0 = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
		wxToolBarToolBase *toolbaritem_0 = toolbar_0->AddTool(wxID_MOVE_UP, wxEmptyString, move_up_icon(), wxEmptyString, wxITEM_NORMAL);
		toolbar_0->AddSeparator();
		wxToolBarToolBase *toolbaritem_2 = toolbar_0->AddTool(wxID_REMOVE, wxEmptyString, trash_icon(), wxEmptyString, wxITEM_NORMAL);
		toolbar_0->Realize();
		boxsizer_0->Add(toolbar_0, 0, wxALL|wxEXPAND, 4);
		search = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxBORDER_NONE);
		search->SetHint("Search");
		boxsizer_0->Add(search, 0, wxALL|wxEXPAND, 4);
		passive = new wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxLC_REPORT|wxBORDER_NONE|wxTAB_TRAVERSAL|wxHSCROLL);
		passive->AppendColumn("Channel");
		passive->AppendColumn("File");
		passive->AppendColumn("Message");
		passive->AppendColumn("Signal");
		passive->AppendColumn("sigpath");
		boxsizer_0->Add(passive, 1, wxALL|wxEXPAND, 4);
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
	wxListCtrl *active;
	wxTextCtrl *search;
	wxListCtrl *passive;
};
// End Generate
}
