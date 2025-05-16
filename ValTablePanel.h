#pragma once
#include "Precompiled.h"
#include "DBCFile.h"

namespace Ui {
	class ValTablePanel;
	class ValTableDialog;
	class ValueEditDialog;
	class ValueListPanel;
};

class ValueMapValidator : public wxValidator {
public:
						ValueMapValidator( wxValueMap *value, bool isEmpty = false );
						~ValueMapValidator() { value = nullptr; }
	virtual wxObject *	Clone() const { return new ValueMapValidator( *this ); }
	virtual bool		TransferFromWindow();
	virtual bool		TransferToWindow();
	virtual bool		Validate( wxWindow *parent );
private:
	wxValueMap *		value;
	bool				isEmpty;
};

class ValueEditDialog : public wxDialog {
public:
						ValueEditDialog( wxWindow *parent, wxWindowID id = wxID_ANY );
	virtual void		InitDialog();
	virtual bool		Validate();
	virtual void		EndModal( int retCode );
	void				SetValue( int value, const wxString &desc );
	int					GetValue() const { return value; }
	const wxString &	GetDesc() const { return desc; }
private:
	int					value;
	wxString			desc;
	wxSharedPtr<Ui::ValueEditDialog> ui;
};


class ValueListPanel : public wxPanel {
public:
	ValueListPanel( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
	void				SetValueMap( const wxValueMap &map );
	void				UpdateList();
	const wxValueMap &	GetValueMap() const { return map; }
	bool				GetSelected( int &value, wxString &desc ) const;
protected:
	void				OnAddValue( wxCommandEvent &event );
	void				OnEditValue( wxListEvent &event );
	void				OnRemoveValue( wxCommandEvent &event );
private:
	wxValueMap			map;
	int					maxValue;
	wxSharedPtr<Ui::ValueListPanel> ui;
};


class ValTableDialog : public wxDialog {
public:
						ValTableDialog( wxWindow *parent, wxWindowID id = wxID_ANY );
	void				SetValTable( const wxString &name, const wxValueMap &map );
	virtual void		InitDialog();
	virtual bool		Validate();
	virtual void		EndModal( int retCode );
	const wxString &	GetTableName() const { return name; }
	const wxValueMap &	GetTableValues() const { return map; }
private:
	wxString			name;
	wxValueMap			map;
	wxSharedPtr<Ui::ValTableDialog> ui;
};


class ValTablePanel : public wxPanel {
public:
	ValTablePanel( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
	~ValTablePanel();
	wxVector<wxDBCValueTable *> &	GetTable() { return tables; }
	void							SetTable( wxDBCFile *file );
protected:
	void							UpdateList();
	wxDBCValueTable *				GetSelected() const;
	wxDBCValueTable *				FindTable( const wxString &name ) const;
protected:
	void							OnAddTable( wxCommandEvent &event );
	void							OnEditTable( wxListEvent &event );
	void							OnRemoveTable( wxCommandEvent &event );
private:
	wxDBCFile *						file;
	wxVector<wxDBCValueTable *>		tables;
	wxSharedPtr<Ui::ValTablePanel>	ui;
};


