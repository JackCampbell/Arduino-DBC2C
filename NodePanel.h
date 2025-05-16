#pragma once
#include "Precompiled.h"
#include "DBCFile.h"

namespace Ui {
	class NodePanel;
	class NodeDialog;
	class NodeSelectDialog;
	class NodeListPanel;
}

class NodeListValidator : public wxValidator {
public:
						NodeListValidator( wxArrayString *nodes, const wxDBCFile *file );
	virtual 			~NodeListValidator();
	virtual wxObject *	Clone() const { return new NodeListValidator( *this ); }
	virtual bool		TransferFromWindow();
	virtual bool		TransferToWindow();
	virtual bool		Validate( wxWindow *parent );
private:
	wxArrayString *					nodes;
	const wxDBCFile *				file;
};

class NodeSelectDialog : public wxDialog {
public:
							NodeSelectDialog( wxWindow *parent, const wxDBCFile *file, const wxArrayString &names, wxWindowID id = wxID_ANY );
	const wxString &		GetNodeName() const { return name; }
	virtual void			InitDialog();
	virtual bool			Validate();
	virtual void			EndModal( int retCode );
protected:
	void					OnChangeNode( wxCommandEvent &event );
private:
	const wxDBCFile *					file;
	wxString							name;
	wxSharedPtr<Ui::NodeSelectDialog>	ui;
};

class NodeListPanel : public wxPanel {
public:
							NodeListPanel( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
							~NodeListPanel();
	void					SetNodeList( wxArrayString &nodes, const wxDBCFile *file );
	const wxArrayString &	GetNodeList() const { return nodes; }
private:
	void					UpdateList();
	wxArrayString			GetNodeNames() const;
protected:
	void					OnAddNode( wxCommandEvent &event );
	void					OnRemoveNode( wxCommandEvent &event );
private:
	const wxDBCFile *					file;
	wxArrayString						nodes;
	wxSharedPtr<Ui::NodeListPanel>		ui;
};







class NodeDialog : public wxDialog {
public:
							NodeDialog( wxWindow *parent, const wxDBCFile *file, wxWindowID id = wxID_ANY );
							~NodeDialog();
	virtual void			InitDialog();
	virtual bool			Validate();
	virtual void			EndModal( int retCode );
	void					SetName( const wxString &name ) { this->name = name; }
	void					SetComment( const wxString &comment ) { this->comment = comment; }
	void					SetAttrs( const wxStrMap &attrs ) { this->attrs = attrs; }
	const wxString &		GetNodeName() const { return name; }
	const wxString &		GetComment() const { return comment; }
	const wxStrMap &		GetAttrs() const { return attrs; }
private:
	const wxDBCFile *					file;
	wxString							name;
	wxString							comment;
	wxStrMap							attrs;
	wxSharedPtr<Ui::NodeDialog>			ui;
};

// declare: NodePanel.cpp
class NodePanel : public wxPanel {
public:
							NodePanel( wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxTAB_TRAVERSAL );
							~NodePanel();
	void					SetNodes( wxDBCFile *file );
	wxVector<wxDBCNode *> &	GetNodes() { return nodes; }
protected:
	void					UpdateList();
	void					OnAddNode( wxCommandEvent &event );
	void					OnEditNode( wxListEvent &event );
	void					OnRemoveNode( wxCommandEvent &event );
private:
	wxDBCNode *							GetSelected() const;
	wxDBCNode *							FindName(const wxString &name) const;
private:
	wxDBCFile *							file;
	wxVector<wxDBCNode *>				nodes;
	wxSharedPtr<Ui::NodePanel> 			ui;
};





