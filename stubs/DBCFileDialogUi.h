// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include "AttributePanel.h"
#include "DefinePanel.h"
#include "ValTablePanel.h"
#include "NodePanel.h"
#include "MessagePanel.h"
#include "Resource.h"



namespace Ui {
// Generate DBCFileDialog
class DBCFileDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(800, 600));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		tabs = new wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		wxPanel *global = new wxPanel(tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		wxBoxSizer *boxsizer_1 = new wxBoxSizer(wxVERTICAL);
		wxFlexGridSizer *flexgridsizer_0 = new wxFlexGridSizer(0, 2, 0, 0);
		wxStaticText *statictext_0 = new wxStaticText(global, wxID_ANY, "Version", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_0, 0, wxALL, 4);
		version = new wxTextCtrl(global, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(version, 0, wxALL|wxEXPAND, 4);
		wxStaticText *statictext_1 = new wxStaticText(global, wxID_ANY, "Baudrate", wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(statictext_1, 0, wxALL, 4);
		baudrate = new wxTextCtrl(global, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(baudrate, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->Add(1, 1, 0, wxALL, 4);
		btr1 = new wxTextCtrl(global, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(btr1, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->Add(1, 1, 0, wxALL, 4);
		btr2 = new wxTextCtrl(global, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		flexgridsizer_0->Add(btr2, 0, wxALL|wxEXPAND, 4);
		flexgridsizer_0->AddGrowableCol(1, 1);
		boxsizer_1->Add(flexgridsizer_0, 0, wxALL|wxEXPAND, 4);
		wxNotebook *notebook_0 = new wxNotebook(global, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		comment = new wxTextCtrl(notebook_0, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxBORDER_NONE);
		notebook_0->AddPage(comment, "Comment");
		attrs = new ::AttributePanel(notebook_0, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		notebook_0->AddPage(attrs, "Attributes");
		boxsizer_1->Add(notebook_0, 1, wxALL|wxEXPAND, 4);
		global->SetSizer(boxsizer_1);
		tabs->AddPage(global, "Global");
		defines = new ::DefinePanel(tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		tabs->AddPage(defines, "Attr Defines");
		tables = new ::ValTablePanel(tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		tabs->AddPage(tables, "Value Tables");
		nodes = new ::NodePanel(tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		tabs->AddPage(nodes, "Nodes");
		messages = new ::MessagePanel(tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		tabs->AddPage(messages, "Messages");
		boxsizer_0->Add(tabs, 1, wxALL|wxEXPAND, 4);
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
	wxNotebook *tabs;
	wxTextCtrl *version;
	wxTextCtrl *baudrate;
	wxTextCtrl *btr1;
	wxTextCtrl *btr2;
	wxTextCtrl *comment;
	::AttributePanel *attrs;
	::DefinePanel *defines;
	::ValTablePanel *tables;
	::NodePanel *nodes;
	::MessagePanel *messages;
};
// End Generate
}
