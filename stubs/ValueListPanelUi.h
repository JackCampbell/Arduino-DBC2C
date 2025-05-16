// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include "Resource.h"



namespace Ui {
// Generate ValueListPanel
class ValueListPanel {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(300, 200));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxToolBar *toolbar_0 = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
		wxToolBarToolBase *toolbaritem_0 = toolbar_0->AddTool(wxID_ADD_ITEM, wxEmptyString, add_icon(), wxEmptyString, wxITEM_NORMAL);
		wxToolBarToolBase *toolbaritem_1 = toolbar_0->AddTool(wxID_REM_ITEM, wxEmptyString, trash_icon(), wxEmptyString, wxITEM_NORMAL);
		toolbar_0->Realize();
		boxsizer_0->Add(toolbar_0, 0, wxEXPAND, 4);
		list = new wxListCtrl(parent, wxID_ITEM_LIST, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxLC_REPORT|wxBORDER_NONE|wxTAB_TRAVERSAL|wxHSCROLL);
		list->AppendColumn("Value");
		list->AppendColumn("Desc");
		boxsizer_0->Add(list, 1, wxEXPAND, 4);
		parent->SetSizer(boxsizer_0);
		parent->Layout();
	}
public:
	wxListCtrl *list;
};
// End Generate
}
