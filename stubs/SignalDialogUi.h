// Generate UiDesigner 1.1
#pragma once
#include <wx/sizer.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include "DBCBitMap.h"
#include "AttributePanel.h"
#include "ValTablePanel.h"
#include "Resource.h"



namespace Ui {
// Generate SignalDialog
class SignalDialog {
public:
	void Setup(wxWindow *parent) {
		parent->SetMinSize(wxSize(900, 800));
		wxBoxSizer *boxsizer_0 = new wxBoxSizer(wxVERTICAL);
		wxGridBagSizer *gridbagsizer_0 = new wxGridBagSizer(0, 0);
		wxStaticText *statictext_0 = new wxStaticText(parent, wxID_ANY, "Name", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(statictext_0, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 4);
		name = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(name, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		wxStaticText *statictext_1 = new wxStaticText(parent, wxID_ANY, "BitMap", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(statictext_1, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, 4);
		start_bit = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		start_bit->SetHint("Start Bit");
		gridbagsizer_0->Add(start_bit, wxGBPosition(0, 3), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		bit_len = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		bit_len->SetHint("Bit Len");
		gridbagsizer_0->Add(bit_len, wxGBPosition(0, 4), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		map = new ::BitMapCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(map, wxGBPosition(1, 0), wxGBSpan(7, 2), wxALL|wxEXPAND, 4);
		wxStaticText *statictext_2 = new wxStaticText(parent, wxID_ANY, "Byte Order", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(statictext_2, wxGBPosition(1, 2), wxGBSpan(1, 1), wxALL, 4);
		endial = new wxCheckBox(parent, wxID_ANY, "Big Endial", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(endial, wxGBPosition(1, 3), wxGBSpan(1, 2), wxALL|wxEXPAND, 4);
		wxStaticText *statictext_3 = new wxStaticText(parent, wxID_ANY, "Value Type", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(statictext_3, wxGBPosition(2, 2), wxGBSpan(1, 1), wxALL, 4);
		type = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, 0);
		gridbagsizer_0->Add(type, wxGBPosition(2, 3), wxGBSpan(1, 2), wxALL|wxEXPAND, 4);
		wxStaticText *statictext_4 = new wxStaticText(parent, wxID_ANY, "Linear", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(statictext_4, wxGBPosition(3, 2), wxGBSpan(1, 1), wxALL, 4);
		scale = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		scale->SetHint("Scale");
		gridbagsizer_0->Add(scale, wxGBPosition(3, 3), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		offset = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		offset->SetHint("Offset");
		gridbagsizer_0->Add(offset, wxGBPosition(3, 4), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		wxStaticText *statictext_5 = new wxStaticText(parent, wxID_ANY, "Clamp", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(statictext_5, wxGBPosition(4, 2), wxGBSpan(1, 1), wxALL, 4);
		min = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		min->SetHint("Min");
		gridbagsizer_0->Add(min, wxGBPosition(4, 3), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		max = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		max->SetHint("Max");
		gridbagsizer_0->Add(max, wxGBPosition(4, 4), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		wxStaticText *statictext_6 = new wxStaticText(parent, wxID_ANY, "Unit", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(statictext_6, wxGBPosition(5, 2), wxGBSpan(1, 1), wxALL, 4);
		unit = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(unit, wxGBPosition(5, 3), wxGBSpan(1, 2), wxALL|wxEXPAND, 4);
		wxStaticText *statictext_7 = new wxStaticText(parent, wxID_ANY, "Multiplexer", wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(statictext_7, wxGBPosition(6, 2), wxGBSpan(1, 1), wxALL, 4);
		multiplexer = new wxTextCtrl(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
		gridbagsizer_0->Add(multiplexer, wxGBPosition(6, 3), wxGBSpan(1, 2), wxALL|wxEXPAND, 4);
		gridbagsizer_0->Add(1, 1, wxGBPosition(7, 2), wxGBSpan(1, 1), wxALL|wxEXPAND, 4);
		gridbagsizer_0->AddGrowableRow(7, 1);
		gridbagsizer_0->AddGrowableCol(1, 1);
		gridbagsizer_0->AddGrowableCol(3, 1);
		gridbagsizer_0->AddGrowableCol(4, 1);
		boxsizer_0->Add(gridbagsizer_0, 0, wxEXPAND, 4);
		wxNotebook *notebook_0 = new wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		comment = new wxTextCtrl(notebook_0, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxBORDER_NONE);
		notebook_0->AddPage(comment, "Comment");
		attrs = new ::AttributePanel(notebook_0, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		notebook_0->AddPage(attrs, "Attributes");
		values = new ::ValueListPanel(notebook_0, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
		notebook_0->AddPage(values, "Values");
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
	wxTextCtrl *start_bit;
	wxTextCtrl *bit_len;
	::BitMapCtrl *map;
	wxCheckBox *endial;
	wxChoice *type;
	wxTextCtrl *scale;
	wxTextCtrl *offset;
	wxTextCtrl *min;
	wxTextCtrl *max;
	wxTextCtrl *unit;
	wxTextCtrl *multiplexer;
	wxTextCtrl *comment;
	::AttributePanel *attrs;
	::ValueListPanel *values;
};
// End Generate
}
