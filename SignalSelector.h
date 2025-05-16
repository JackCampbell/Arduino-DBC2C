#pragma once
#include "DBCFile.h"
#include "Utils.h"

namespace Ui {
	class SignalSelectorDialog;
}

class SignalSelectorDialog : public wxDialog {
public:
	SignalSelectorDialog( wxWindow *parent, wxVector<wxDBCFile *> files, wxWindowID id = wxID_ANY );
	virtual void InitDialog();
	virtual bool Validate();
	virtual void EndModal( int retCode );
	void SetQueryText( const wxString &query ) { this->query = query; }
	void SetSelected( const wxArrayString &selected ) { this->selected = selected; }
	const wxArrayString &GetSelected() const { return this->selected; }
protected:
	void OnSearchData( wxCommandEvent &event );
	void OnAddSignal( wxCommandEvent &event );
	void OnRemoveSignal( wxCommandEvent &event );
	void OnFocusDialog(wxFocusEvent &event);
private:
	void UpdateSelectedList();

	void LoadSelectedList();
	void LoadAllList();
private:
	wxVector<wxDBCFile *> files;
	wxString query;
	wxArrayString selected;
	wxSharedPtr<Ui::SignalSelectorDialog> ui;
};
