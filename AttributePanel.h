#pragma once
#include "Precompiled.h"
#include "DBCFile.h"

namespace Ui {
class AttributePanel;
class AttributeDialog;
}; // namespace Ui

class AttrMapValidator : public wxValidator {
public:
							AttrMapValidator( wxStrMap *map, int object, const wxDBCFile *file );
							~AttrMapValidator();
	virtual wxObject *		Clone() const { return new AttrMapValidator( *this ); }
	virtual bool			TransferFromWindow();
	virtual bool			TransferToWindow();
	virtual bool			Validate( wxWindow *parent ) { return true; }
private:
	wxStrMap *							map;
	const wxDBCFile *					file;
	int									object;
};


class AttrValueValidator : public wxValidator {
public:
							AttrValueValidator( wxString *value, const wxDBCAttrDefine *const &define );
							~AttrValueValidator() { value = nullptr; }
	virtual wxObject *		Clone() const { return new AttrValueValidator( *this ); }
	virtual bool			TransferFromWindow();
	virtual bool			TransferToWindow();
	virtual bool			Validate( wxWindow *parent );
private:
	wxString *							value;
	const wxDBCAttrDefine *const &		define;
};


class AttributeDialog : public wxDialog {
public:
							AttributeDialog( wxWindow *parent, const wxDBCFile *file, wxWindowID id = wxID_ANY );
							~AttributeDialog();
	void					SetAttrValue( const wxString &name, const wxString &value );
	void					SetAttrNames( const wxArrayString &names );
	virtual void			InitDialog();
	virtual bool			Validate();
	virtual void			EndModal( int retCode );
	const wxString &		GetAttrName() const { return name; }
	const wxString &		GetAttrValue() const { return value; }
protected:
	void					OnChangeAttribute( wxCommandEvent &event );
	void					OnChangeEnums( wxCommandEvent &event );
	void					UpdateDefineType();
private:
	const wxDBCFile *					file;
	const wxDBCAttrDefine *				define;
	wxString							name;
	wxString							value;
	wxArrayString						names;
	wxSharedPtr<Ui::AttributeDialog>		ui;
};





// declare: AttributePanel.cpp
class AttributePanel : public wxPanel {
public:
							AttributePanel( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
	void					SetAttrMap( wxStrMap map, int object, const wxDBCFile *file );
	const wxStrMap &		GetAttrMap() { return map; }
protected:
	void					UpdateList();
	wxString				GetSelected() const;
	wxArrayString			GetAttrNames() const;
	void					OnAddAttribute( wxCommandEvent &event );
	void					OnEditAttribute( wxListEvent &event );
	void					OnRemoveAttribute( wxCommandEvent &event );
private:
	const wxDBCFile *					file;
	wxStrMap							map;
	int									object;
	wxSharedPtr<Ui::AttributePanel>		ui;
};


