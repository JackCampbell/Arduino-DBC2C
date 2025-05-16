// Generate UiDesigner 1.2
#pragma once
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/listctrl.h>
#include "Resource.h"



namespace Ui {
// Generate MainFrame
class MainFrame {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(800, 600));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		maintool = new wxToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL);
		wxToolBarToolBase *toolbaritem_2 = maintool->AddTool(wxID_ADD_ITEM, wxEmptyString, add_icon(), wxEmptyString, wxITEM_NORMAL);
		wxToolBarToolBase *toolbaritem_4 = maintool->AddTool(wxID_REM_ITEM, wxEmptyString, trash_icon(), wxEmptyString, wxITEM_NORMAL);
		maintool->AddSeparator();
		wxToolBarToolBase *toolbaritem_6 = maintool->AddTool(wxID_LOAD_ITEM, wxEmptyString, folder_icon(), wxEmptyString, wxITEM_NORMAL);
		wxToolBarToolBase *toolbaritem_7 = maintool->AddTool(wxID_SAVE_ITEM, wxEmptyString, save_icon(), wxEmptyString, wxITEM_NORMAL);
		maintool->AddSeparator();
		wxToolBarToolBase *toolbaritem_9 = maintool->AddTool(wxID_GEN_ITEM, wxEmptyString, setting_icon(), wxEmptyString, wxITEM_NORMAL);
		maintool->AddSeparator();
		wxToolBarToolBase *toolbaritem_1 = maintool->AddTool(wxID_DBC_FILES, wxEmptyString, books_icon(), wxEmptyString, wxITEM_NORMAL);
		maintool->Realize();
		boxsizer_0->Add(maintool, 0, wxEXPAND, 4);
		wxBoxSizer *boxsizer_3 = new wxBoxSizer(wxHORIZONTAL);
		channel_info = new wxStaticText(parent, wxID_ANY, "Channel", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL|wxBORDER_NONE);
		boxsizer_3->Add(channel_info, 1, wxALL, 4);
		channel_setting = new wxButton(parent, wxID_CHANNEL_BTN, "Setting", wxDefaultPosition, wxDefaultSize, wxBORDER_NONE|wxBU_NOTEXT);
		channel_setting->SetBitmap(setting_icon(), wxSOUTH);
		boxsizer_3->Add(channel_setting, 0, wxALL, 4);
		boxsizer_0->Add(boxsizer_3, 0, wxALL|wxEXPAND, 4);
		gens = new wxListCtrl(parent, wxID_ITEM_LIST, wxDefaultPosition, wxDefaultSize, wxVSCROLL|wxLC_REPORT|wxBORDER_NONE|wxTAB_TRAVERSAL|wxHSCROLL);
		gens->AppendColumn("Type");
		gens->AppendColumn("Desc");
		gens->AppendColumn("Signals");
		boxsizer_0->Add(gens, 1, wxEXPAND, 4);
		genfile = new wxStaticText(parent, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, 0);
		boxsizer_0->Add(genfile, 0, wxALL|wxEXPAND, 4);
		parent->SetSizer(boxsizer_0);
		parent->Layout();
	}
public:
	wxToolBar *maintool;
	wxStaticText *channel_info;
	wxButton *channel_setting;
	wxListCtrl *gens;
	wxStaticText *genfile;
};
// End Generate
}
