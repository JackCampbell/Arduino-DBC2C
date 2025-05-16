// Generate UiDesigner 1.2
#pragma once
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include <wx/button.h>
#include "Resource.h"



namespace Ui {
// Generate FileSelectDialog
class FileSelectDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(400, 400));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxToolBar *toolbar_0 = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
		wxToolBarToolBase *toolbaritem_0 = toolbar_0->AddTool(wxID_ADD_ITEM, wxEmptyString, add_icon(), wxEmptyString, wxITEM_NORMAL);
		wxToolBarToolBase *toolbaritem_1 = toolbar_0->AddTool(wxID_REM_ITEM, wxEmptyString, trash_icon(), wxEmptyString, wxITEM_NORMAL);
		toolbar_0->Realize();
		boxsizer_0->Add(toolbar_0, 0, wxEXPAND, 4);
		files = new wxListCtrl(parent, wxID_ITEM_LIST, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxLC_REPORT|wxBORDER_NONE|wxTAB_TRAVERSAL|wxHSCROLL);
		files->AppendColumn("Name");
		boxsizer_0->Add(files, 1, wxEXPAND, 4);
		wxStdDialogButtonSizer *dialogbuttonsizer_0 = new wxStdDialogButtonSizer();
		dialogbuttonsizer_0->AddButton(new wxButton(parent, wxID_CANCEL));
		dialogbuttonsizer_0->Realize();
		boxsizer_0->Add(dialogbuttonsizer_0, 0, wxEXPAND, 4);
		parent->SetSizer(boxsizer_0);
		parent->Fit();
	}
public:
	wxListCtrl *files;
};
// End Generate
}
