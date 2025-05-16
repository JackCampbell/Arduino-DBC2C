#pragma once
#include "precompiled.h"

class wxDBCFile;
namespace Ui {
	class FileSelectDialog;
}

class FileSelectDialog : public wxDialog {
public:
	FileSelectDialog( wxWindow *parent, wxWindowID = wxID_ANY );
	virtual ~FileSelectDialog();
	virtual void InitDialog();
	virtual bool Validate();
	virtual void EndModal( int retCode );
	virtual bool TransferDataFromWindow();
	virtual bool TransferDataToWindow();
	const wxString &GetSelected() const { return selected; }
protected:
	void OnAddItem( wxCommandEvent &event );
	void OnTranshItem( wxCommandEvent &event );
	void OnEditItem( wxCommandEvent &event );
private:
	void UpdateList();
private:
	wxString selected;
	wxSharedPtr<Ui::FileSelectDialog> ui;
};

bool AddDBCFile( const wxString &filename );
wxDBCFile *FindDBCFile( const wxString &name );
bool RemoveDBCFile( const wxString &name );
wxArrayString DBCFileNames();
void SaveDBCFilenames();
void ReadDBCFilenames();
