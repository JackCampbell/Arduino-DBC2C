#pragma once
#include "Precompiled.h"
#include "DBCFile.h"

namespace Ui {
	class DefinePanel;
	class DefineDialog;
	class DefineStrPanel;
};
class wxListEvent;

// declare: DefinePanel.cpp
class DefinePanel : public wxPanel {
public:
									DefinePanel( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
									~DefinePanel();
	void							SetDefine( wxDBCFile *file );
	wxVector<wxDBCAttrDefine *> &	GetDefine() { return defines; }
	void							UpdateList();
protected:
	void							OnAddDefine( wxCommandEvent &event );
	void							OnEditDefine( wxListEvent &event );
	void							OnRemoveDefine( wxCommandEvent &event );
public:
	wxDBCFile *						file;
	wxVector<wxDBCAttrDefine *>		defines;
	wxSharedPtr<Ui::DefinePanel>		ui;
};

/*
class EnumValidator : public wxValidator {
public:
	EnumValidator( wxArrayString *enums );
	~EnumValidator() { enums = nullptr; }
	virtual wxObject *Clone() const { return new EnumValidator( *this ); }
	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();
	virtual bool Validate( wxWindow *parent );
private:
	wxArrayString *enums;
};

class TypeNumValidator : public wxValidator {
public:
	TypeNumValidator( wxString *value );
	~TypeNumValidator() { value = nullptr; }
	virtual wxObject *Clone() const { return new TypeNumValidator( *this ); }
	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();
	virtual bool Validate( wxWindow *parent );
	void SetType(int type) { this->type = type; }
private:
	wxString *value;
	int type;
};
*/
class DefineDialog : public wxDialog {
public:
	DefineDialog( wxWindow *parent, wxDBCAttrDefine *define, wxWindowID id = wxID_ANY );
	~DefineDialog();
	virtual void InitDialog();
	virtual bool Validate();
	virtual void EndModal( int retCode );
protected:
	void UpdateParms();
protected:
	void OnUpdateParms( wxCommandEvent &event );
private:
	wxSharedPtr<Ui::DefineDialog> ui;
	wxString name, min, max;
	int type, object;
	wxArrayString enums;
	wxDBCAttrDefine *define;
};
