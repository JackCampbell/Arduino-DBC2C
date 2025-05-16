#include "ValTablePanel.h"
#include "stubs/ValTablePanelUi.h"
#include "stubs/ValueEditDialogUi.h"
#include "stubs/ValueListPanelUi.h"
#include "stubs/ValTableDialogUi.h"
#include "Utils.h"
#ifdef _WIN32
#pragma hdrstop
#endif

ValueMapValidator::ValueMapValidator( wxValueMap *value, bool isEmpty ) : wxValidator() {
	this->value = value;
	this->isEmpty = isEmpty;
}

bool ValueMapValidator::Validate( wxWindow *parent ) {
	if( !m_validatorWindow->IsEnabled() ) {
		return true;
	}
	if( isEmpty ) {
		return true;
	}
	try {
		ValueListPanel *panel = dynamic_cast<ValueListPanel *>( m_validatorWindow );
		if( panel->GetValueMap().empty() ) {
			wxMessageBox( "Value is empty", "Warning", wxOK, parent );
			return false;
		}
	} catch( ... ) {
		wxFAIL_MSG( _T("CfloatValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}

bool ValueMapValidator::TransferFromWindow() {
	try {
		ValueListPanel *panel = dynamic_cast<ValueListPanel *>( m_validatorWindow );
		*value = panel->GetValueMap();
	} catch( ... ) {
		wxFAIL_MSG( _T("CfloatValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}

bool ValueMapValidator::TransferToWindow() {
	try {
		ValueListPanel *panel = dynamic_cast<ValueListPanel *>( m_validatorWindow );
		panel->SetValueMap( *value );
	} catch( ... ) {
		wxFAIL_MSG( _T("CfloatValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}












ValueEditDialog::ValueEditDialog( wxWindow *parent, wxWindowID id ) : wxDialog( parent, id, "Value Edit", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	ui = new Ui::ValueEditDialog();
	ui->Setup( this );
	ui->value->SetValidator( wxIntegerValidator<int>( &value ) );
	ui->desc->SetValidator( wxTextValidator( wxFILTER_NONE, &desc ) );
	ui->desc->SetFocus();
	value = 0;
}

void ValueEditDialog::SetValue( int value, const wxString &desc ) {
	this->value = value;
	this->desc = desc;
}

void ValueEditDialog::InitDialog() {
	TransferDataToWindow();
	wxDialog::InitDialog();
}

bool ValueEditDialog::Validate() {
	TransferDataFromWindow();
	if( desc.empty() ) {
		wxMessageBox( "Description is empty", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void ValueEditDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate() ) {
		return;
	}
	wxDialog::EndModal( retCode );
}











ValueListPanel::ValueListPanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPanel( parent, id, pos, size, style ) {
	ui = new Ui::ValueListPanel();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &ValueListPanel::OnAddValue, this, wxID_ADD_ITEM );
	Bind( wxEVT_TOOL, &ValueListPanel::OnRemoveValue, this, wxID_REM_ITEM );
	Bind( wxEVT_LIST_ITEM_ACTIVATED, &ValueListPanel::OnEditValue, this, wxID_ITEM_LIST );
	maxValue = 0;
}

void ValueListPanel::SetValueMap( const wxValueMap &map ) {
	this->map = map;
	UpdateList();
}

void ValueListPanel::UpdateList() {
	maxValue = -1;
	ui->list->Freeze();
	ui->list->DeleteAllItems();
	for( wxValueMap::const_iterator iter = map.begin(); iter != map.end(); iter++ ) {
		const int value = iter->first;
		maxValue = MAX( maxValue, value );

		int count = ui->list->GetItemCount();
		long index = ui->list->InsertItem( count, wxString::Format( "%02x", count ) );
		ui->list->SetItem( index, 1, iter->second );
	}
	ListCtrlAutoColumn(ui->list);
	ui->list->Thaw();
}

bool ValueListPanel::GetSelected( int &value, wxString &desc ) const {
	int index = ui->list->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == -1 ) {
		return false;
	}
	if( !ui->list->GetItemText( index, 0 ).ToInt( &value, 10 ) ) {
		return false;
	}
	wxValueMap::const_iterator iter = map.find( value );
	if( iter == map.end() ) {
		return false;
	}
	desc = iter->second;
	return true;
}

void ValueListPanel::OnAddValue( wxCommandEvent &event ) {
	ValueEditDialog dialog( this );
	dialog.SetValue( maxValue + 1, wxEmptyString );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	int value = dialog.GetValue();
	wxString desc = dialog.GetDesc();
	map.insert( wxValueMap::value_type( value, desc ) );
	UpdateList();
}

void ValueListPanel::OnEditValue( wxListEvent &event ) {
	int value;
	wxString desc;
	if( !GetSelected( value, desc ) ) {
		return;
	}
	ValueEditDialog dialog( this );
	dialog.SetValue( value, desc );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	wxValueMap::iterator iter = map.find( value );
	if( iter != map.end() ) {
		map.erase( iter );
	}
	value = dialog.GetValue();
	desc = dialog.GetDesc();
	map.insert( wxValueMap::value_type( value, desc ) );
	UpdateList();
}

void ValueListPanel::OnRemoveValue( wxCommandEvent &event ) {
	int value;
	wxString desc;
	if( !GetSelected( value, desc ) ) {
		return;
	}
	wxValueMap::iterator iter = map.find( value );
	if( iter == map.end() ) {
		return;
	}
	map.erase( iter );
	UpdateList();
}








ValTableDialog::ValTableDialog( wxWindow *parent, wxWindowID id ) : wxDialog( parent, id, "ValTable", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	ui = new Ui::ValTableDialog();
	ui->Setup( this );
	ui->name->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &name ) );
	ui->values->SetValidator( ValueMapValidator( &map ) );
}

void ValTableDialog::SetValTable( const wxString &name, const wxValueMap &map ) {
	this->name = name;
	this->map = map;
}

void ValTableDialog::InitDialog() {
	TransferDataToWindow();
	wxDialog::InitDialog();
}

bool ValTableDialog::Validate() {
	TransferDataFromWindow();
	if( name.empty() ) {
		wxMessageBox( "Name is empty", "Warning", wxOK, this );
		return false;
	}
	if( map.empty() ) {
		wxMessageBox( "Values is empty", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void ValTableDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate() ) {
		return;
	}
	wxDialog::EndModal( retCode );
}







ValTablePanel::ValTablePanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPanel( parent, id, pos, size, style ), file( nullptr ) {
	ui = new Ui::ValTablePanel();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &ValTablePanel::OnAddTable, this, wxID_ADD_ITEM );
	Bind( wxEVT_TOOL, &ValTablePanel::OnRemoveTable, this, wxID_REM_ITEM );
	Bind( wxEVT_LIST_ITEM_ACTIVATED, &ValTablePanel::OnEditTable, this, wxID_ITEM_LIST );
}

ValTablePanel::~ValTablePanel() {
	file = nullptr;
}

void ValTablePanel::SetTable( wxDBCFile *file ) {
	tables.clear();
	for( int i = 0; i < file->NumTable(); i++ ) {
		wxDBCValueTable *table = file->GetTable( i );
		tables.push_back( new wxDBCValueTable( table ) );
	}
	this->file = file;
	UpdateList();
}

void ValTablePanel::UpdateList() {
	ui->list->Freeze();
	ui->list->DeleteAllItems();
	for( int i = 0; i < tables.size(); i++ ) {
		const wxDBCValueTable *table = tables.at(i);
		long index = ui->list->InsertItem( i, table->GetName() );
		ui->list->SetItem( index, 1, table->GetDesc() );
	}
	ui->list->SetColumnWidth( 0, 250 );
	ui->list->SetColumnWidth( 1, 350 );
	ui->list->Thaw();
}

wxDBCValueTable *ValTablePanel::FindTable(const wxString &name) const {
	for(int i = 0; i < tables.size(); i++) {
		wxDBCValueTable *table = tables[i];
		if(table->name == name) {
			return table;
		}
	}
	return nullptr;
}

wxDBCValueTable *ValTablePanel::GetSelected() const {
	long index = ui->list->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == -1 ) {
		return nullptr;
	}
	wxString name = ui->list->GetItemText( index );
	return FindTable(name);
	
}

void ValTablePanel::OnAddTable( wxCommandEvent &event ) {
	ValTableDialog dialog( this );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	wxString name = dialog.GetTableName();
	wxDBCValueTable *table = FindTable(name);
	if(table) {
		wxMessageBox( "Dublicate table name", "Warning", wxOK, this );
		return;
	}
	table = new wxDBCValueTable(file);
	table->name = dialog.GetTableName();
	table->map = dialog.GetTableValues();
	tables.push_back(table);
	UpdateList();
}

void ValTablePanel::OnEditTable( wxListEvent &event ) {
	wxDBCValueTable *selected = GetSelected();
	if( !selected ) {
		return;
	}
	ValTableDialog dialog( this );
	dialog.SetValTable( selected->name, selected->map );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	selected->name = dialog.GetTableName();
	selected->map = dialog.GetTableValues();
	UpdateList();
}

void ValTablePanel::OnRemoveTable( wxCommandEvent &event ) {
	wxDBCValueTable *selected = GetSelected();
	if( !selected ) {
		return;
	}
	const wxString &name = selected->GetName();
	wxString message = wxString::Format( "Are you sure?\n%s [%s]\n", name, selected->GetDesc() );
	if( wxMessageBox( message, "Remove", wxYES_NO | wxICON_EXCLAMATION, this ) != wxYES ) {
		return;
	}
	wxVector<wxDBCValueTable *>::iterator iter = std::find(tables.begin(), tables.end(), selected);
	if(iter != tables.end()) {
		tables.erase(iter);
	}
	UpdateList();
}


