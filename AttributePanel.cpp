#include "AttributePanel.h"
#include "stubs/AttributePanelUi.h"
#include "stubs/AttributeDialogUi.h"
#include "stubs/Resource.h"
#ifdef WIN32
#pragma hdrstop
#endif

extern void ListCtrlAutoColumn(wxListCtrl *view);
// ListCtrlAutoColumn(ui->list);

AttrMapValidator::AttrMapValidator( wxStrMap *map, int object, const wxDBCFile *file ) : wxValidator() {
	this->map = map;
	this->object = object;
	this->file = file;
}

AttrMapValidator::~AttrMapValidator() {
	file = nullptr;
	map = nullptr;
}

bool AttrMapValidator::TransferToWindow() {
	try {
		AttributePanel *panel = dynamic_cast<AttributePanel *>( m_validatorWindow );
		panel->SetAttrMap( *map, object, file );
	} catch( ... ) {
		wxFAIL_MSG( _T("DefineAttrValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}

bool AttrMapValidator::TransferFromWindow() {
	try {
		AttributePanel *panel = dynamic_cast<AttributePanel *>( m_validatorWindow );
		*map = panel->GetAttrMap();
	} catch( ... ) {
		wxFAIL_MSG( _T("DefineAttrValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}












AttrValueValidator::AttrValueValidator( wxString *value, const wxDBCAttrDefine *const &define ) : wxValidator(), define(define) {
	this->value = value;
}

bool AttrValueValidator::TransferToWindow() {
	if( !m_validatorWindow->IsEnabled() ) {
		return true;
	}
	try {
		wxTextCtrl *panel = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
		panel->SetValue( *value );
	} catch( ... ) {
		wxFAIL_MSG( _T("DefineAttrValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}

bool AttrValueValidator::TransferFromWindow() {
	if( !m_validatorWindow->IsEnabled() ) {
		return true;
	}
	try {
		wxTextCtrl *panel = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
		*value = panel->GetValue();
	} catch( ... ) {
		wxFAIL_MSG( _T("DefineAttrValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}

bool AttrValueValidator::Validate( wxWindow *parent ) {
	if( !m_validatorWindow->IsEnabled() ) {
		return true;
	}
	if(define == nullptr) {
		wxMessageBox( "define of attribute is null" );
		return false;
	}
	double fvalue;
	long ivalue;
	try {
		wxTextCtrl *panel = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
		wxString value = panel->GetValue();
		if( define->type == ATTR_TYPE_FLOAT && !value.ToDouble( &fvalue ) ) {
			wxMessageBox( "The value entered is not real number" );
			return false;
		}
		if( define->type == ATTR_TYPE_INT && !value.ToLong( &ivalue ) ) {
			wxMessageBox( "The value entered is not decimal number" );
			return false;
		}
		if( define->type == ATTR_TYPE_HEX && !value.ToLong( &ivalue, 16 ) ) {
			wxMessageBox( "The value entered is not hexadecimal number" );
			return false;
		}
		if( define->type == ATTR_TYPE_ENUM && define->enums.Index( value ) == wxNOT_FOUND ) {
			wxMessageBox( "The value entered is not enum value" );
			return false;
		}
	} catch( ... ) {
		wxFAIL_MSG( _T("DefineAttrValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}









AttributeDialog::AttributeDialog( wxWindow *parent, const wxDBCFile *file, wxWindowID id ) : wxDialog( parent, id, "Attribute Dialog", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	wxASSERT_MSG( file, "Attribute object is null" );
	ui = new Ui::AttributeDialog();
	ui->Setup( this );
	ui->names->Bind( wxEVT_CHOICE, &AttributeDialog::OnChangeAttribute, this );
	ui->enums->Bind( wxEVT_CHOICE, &AttributeDialog::OnChangeEnums, this );
	ui->value->SetValidator( AttrValueValidator( &value, define ) );
	ui->names->SetValidator( wxGenericValidator( &name ) );
	this->file = file;
}

AttributeDialog::~AttributeDialog() {
	file = nullptr;
}

void AttributeDialog::SetAttrValue( const wxString &name, const wxString &value ) {
	this->name = name;
	this->value = value;
}

void AttributeDialog::SetAttrNames( const wxArrayString &names ) {
	this->names = names;
}

void AttributeDialog::InitDialog() {
	define = file->FindDefine( name );
	if( define ) {
		ui->names->Append( define->name );
		ui->names->SetSelection( 0 );
		ui->names->Enable( false );
	} else {
		ui->names->Append( names );
		ui->names->Enable( true );
	}
	ui->value->SetValue( value );
	UpdateDefineType();
	TransferDataToWindow();
	wxDialog::InitDialog();
}

void AttributeDialog::OnChangeAttribute( wxCommandEvent &event ) {
	UpdateDefineType();
}

void AttributeDialog::OnChangeEnums( wxCommandEvent &event ) {
	wxString value = ui->enums->GetStringSelection();
	ui->value->SetValue( value );
}

void AttributeDialog::UpdateDefineType() {
	wxString name = ui->names->GetStringSelection();
	define = file->FindDefine( name );
	if( !define ) {
		return;
	}
	ui->type->SetLabel( strValueType[define->type] );
	ui->enums->Set( define->enums );
	ui->enums->SetStringSelection( value );
	ui->enums->Show( define->type == ATTR_TYPE_ENUM );
	ui->value->Show( define->type != ATTR_TYPE_ENUM );
	ui->layout->Layout();
}

void AttributeDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate() ) {
		return;
	}
	wxDialog::EndModal( retCode );
}

bool AttributeDialog::Validate() {
	TransferDataFromWindow();
	if( name.empty() ) {
		wxMessageBox( "Name is empty", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}







AttributePanel::AttributePanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPanel( parent, id, pos, size, style ) {
	ui = new Ui::AttributePanel();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &AttributePanel::OnAddAttribute, this, wxID_ADD_ATTR );
	Bind( wxEVT_TOOL, &AttributePanel::OnRemoveAttribute, this, wxID_REM_ATTR );
	Bind( wxEVT_LIST_ITEM_ACTIVATED, &AttributePanel::OnEditAttribute, this, wxID_ATTR_LIST );
	file = nullptr;
}

void AttributePanel::SetAttrMap( wxStrMap map, int object, const wxDBCFile *file ) {
	this->map = map;
	this->file = file;
	this->object = object;
	UpdateList();
}

void AttributePanel::UpdateList() {
	wxASSERT_MSG( file, "File object is null!" );
	ui->list->Freeze();
	ui->list->DeleteAllItems();
	for( wxStrMap::const_iterator iter = map.begin(); iter != map.end(); iter++ ) {
		int count = ui->list->GetItemCount();
		long index = ui->list->InsertItem( count, iter->first );
		ui->list->SetItem( index, 1, iter->second );
	}
	ListCtrlAutoColumn(ui->list);
	ui->list->Thaw();
}

wxString AttributePanel::GetSelected() const {
	int index = ui->list->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == -1 ) {
		return wxEmptyString;
	}
	return ui->list->GetItemText( index, 0 );
}

wxArrayString AttributePanel::GetAttrNames() const {
	wxArrayString names;
	wxASSERT_MSG( file, "File object is null!" );
	for( int i = 0; i < file->NumDefine(); i++ ) {
		const wxDBCAttrDefine *define = file->GetDefine( i );
		if( object != define->GetObject() ) {
			continue;
		}
		const wxString &name = define->GetName();
		names.push_back( name );
	}
	for( wxStrMap::const_iterator iter = map.begin(); iter != map.end(); iter++ ) {
		int index = names.Index( iter->first );
		if( index != wxNOT_FOUND ) {
			names.RemoveAt( index );
		}
	}
	return names;
}

void AttributePanel::OnAddAttribute( wxCommandEvent &event ) {
	const wxArrayString names = GetAttrNames();

	AttributeDialog dialog( this, file );
	dialog.SetAttrNames( names );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	wxString name = dialog.GetAttrName();
	wxString value = dialog.GetAttrValue();
	map.insert( wxStrMap::value_type( name, value ) );
	UpdateList();
}

void AttributePanel::OnEditAttribute( wxListEvent &event ) {
	const wxArrayString names = GetAttrNames();
	wxString name = GetSelected();
	if( name.empty() ) {
		return;
	}
	wxStrMap::iterator iter = map.find( name );
	if( iter == map.end() ) {
		return;
	}
	AttributeDialog dialog( this, file );
	dialog.SetAttrNames( names );
	dialog.SetAttrValue( name, iter->second );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	iter->second = dialog.GetAttrValue();
	UpdateList();
}

void AttributePanel::OnRemoveAttribute( wxCommandEvent &event ) {
	wxString name = GetSelected();
	if( name.empty() ) {
		return;
	}
	wxStrMap::iterator iter = map.find( name );
	if( iter == map.end() ) {
		return;
	}
	wxString message = wxString::Format( "Are you sure?\nAttr:%s = %s", iter->first, iter->second );
	if( wxMessageBox( message, "Remove", wxYES_NO | wxICON_EXCLAMATION ) != wxYES ) {
		return;
	}
	map.erase( iter );
	UpdateList();
}










