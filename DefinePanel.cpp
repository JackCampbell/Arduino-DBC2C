#include "DefinePanel.h"
#include "stubs/DefinePanelUi.h"
#include "stubs/DefineDialogUi.h"
#include "stubs/DefineStrPanelUi.h"
#include "Utils.h"
#ifdef _WIN32
#pragma hdrstop
#endif

DefinePanel::DefinePanel( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style ) : wxPanel( parent, id, pos, size, style ) {
	ui = new Ui::DefinePanel();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &DefinePanel::OnAddDefine, this, wxID_ADD_DEFINE );
	Bind( wxEVT_TOOL, &DefinePanel::OnRemoveDefine, this, wxID_REM_DEFINE );
	Bind( wxEVT_LIST_ITEM_ACTIVATED, &DefinePanel::OnEditDefine, this, wxID_DEFINE_LIST );
	file = nullptr;
}

DefinePanel::~DefinePanel() {
	file = nullptr;
}

void DefinePanel::SetDefine( wxDBCFile *file ) {
	this->file = file;
	defines.clear();
	for( int i = 0; i < file->NumDefine(); i++ ) {
		wxDBCAttrDefine *copy = file->GetDefine( i );
		wxASSERT_MSG(copy, "Define is null!");
		defines.push_back( new wxDBCAttrDefine( copy ) );
	}
	UpdateList();
}

void DefinePanel::UpdateList() {
	ui->list->Freeze();
	ui->list->DeleteAllItems();
	for( int i = 0; i < defines.size(); i++ ) {
		const wxDBCAttrDefine *define = defines[i];
		// wxLogMessage(wxString::Format("-- %d %d\n", define->object, define->type));
		long index = ui->list->InsertItem( i, define->GetName() );
		ui->list->SetItem( index, 1, strObjectType[define->object] );
		ui->list->SetItem( index, 2, strValueType[define->type] );
		if( define->has_default ) {
			ui->list->SetItem( index, 3, define->value );
		}
		if( define->type == ATTR_TYPE_ENUM ) {
			ui->list->SetItem( index, 4, wxJoin( define->enums, ',', 0 ) );
		} else if( define->type != ATTR_TYPE_STRING ) {
			ui->list->SetItem( index, 4, wxString::Format( "Min:%s Max:%s", define->min, define->max ) );
		}
	}
	ListCtrlAutoColumn(ui->list);
	ui->list->Thaw();
}

void DefinePanel::OnAddDefine( wxCommandEvent &event ) {
	/*DefineDialog dialog(this, nullptr);
	if(dialog.ShowModal() != wxID_OK) {
	    return;
	}
	UpdateList();
	*/
	wxLogMessage( "Not Implement\n" );
}

void DefinePanel::OnEditDefine( wxListEvent &event ) {
	wxLogMessage( "Not Implement\n" );
}

void DefinePanel::OnRemoveDefine( wxCommandEvent &event ) {
	wxLogMessage( "Not Implement\n" );
}

DefineDialog::DefineDialog( wxWindow *parent, wxDBCAttrDefine *define, wxWindowID id ) : wxDialog( parent, id, "Attribute Define", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	ui = new Ui::DefineDialog();
	ui->Setup( this );
	ui->object->Append( 4, strObjectType );
	ui->type->Append( 5, strValueType );
	ui->name->SetValidator( wxTextValidator( wxFILTER_ALPHANUMERIC, &name ) );
	ui->object->SetValidator( wxGenericValidator( &object ) );
	ui->type->SetValidator( wxGenericValidator( &type ) );
	// ui->min->SetValidator( TypeNumValidator( &max ) );
	// ui->max->SetValidator( TypeNumValidator( &max ) );
	// ui->enums->SetValidator( EnumValidator( &enums ) );
	ui->type->Bind( wxEVT_CHOICE, &DefineDialog::OnUpdateParms, this );
	this->define = define;
}

DefineDialog::~DefineDialog() {
	define = nullptr;
}

void DefineDialog::InitDialog() {
	if( define ) {
		name = define->GetName();
		min = define->GetMin();
		max = define->GetMax();
		enums = define->GetEnums();
		type = define->GetType();
		object = define->GetObject();
	}
	UpdateParms();
	TransferDataToWindow();
	wxDialog::InitDialog();
}

void DefineDialog::OnUpdateParms( wxCommandEvent &event ) {
	UpdateParms();
}

void DefineDialog::UpdateParms() {
	int type = ui->type->GetSelection();
	switch( type ) {
	case ATTR_TYPE_ENUM:
		break;
	}
	bool isEnum = type == ATTR_TYPE_ENUM;
	bool isNum = ( type == ATTR_TYPE_HEX || type == ATTR_TYPE_INT || type == ATTR_TYPE_FLOAT );
	ui->enums->Enable( isEnum );
	ui->min->Enable( isNum );
	ui->max->Enable( isNum );
	// wxStaticCast( ui->min->GetValidator(), TypeNumValidator )->SetType( type );
	// wxStaticCast( ui->max->GetValidator(), TypeNumValidator )->SetType( type );
	ui->layout->Layout();
}

bool DefineDialog::Validate() {
	TransferDataFromWindow();
	return wxDialog::Validate();
}

void DefineDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK && !Validate()) {
		return;
	}
	wxDialog::EndModal( retCode );
}

/*

EnumValidator::EnumValidator(wxArrayString *enums) {
    this->enums = enums;
}

bool EnumValidator::TransferToWindow() {
    try {
        wxString value = wxJoin(*enums, '\n', 0);
        wxTextCtrl *field = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
        field->SetValue(value);
    } catch( ... ) {
        wxFAIL_MSG( _T("EnumValidator works only with wxTextCtrl") );
        return false;
    }
    return true;
}

bool EnumValidator::TransferFromWindow() {
    try {
        wxTextCtrl *field = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
        wxString value = field->GetValue();
        *enums = wxSplit(value, '\n', 0);
    } catch( ... ) {
        wxFAIL_MSG( _T("EnumValidator works only with wxTextCtrl") );
        return false;
    }
    return true;

}

bool EnumValidator::Validate( wxWindow *parent ) {
    if( !m_validatorWindow->IsEnabled() ) {
        return true;
    }
    try {
        wxTextCtrl *field = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
        wxString value = field->GetValue();
        wxArrayString list = wxSplit(value, '\n', 0);
        if(list.empty()) {
            wxMessageBox("Enums list is empty!", "Warning", wxOK, parent);
            return false;
        }
    } catch( ... ) {
        wxFAIL_MSG( _T("EnumValidator works only with wxTextCtrl") );
        return false;
    }
    return true;
}

TypeNumValidator::TypeNumValidator(wxString *value) {
    this->value = value;
    this->type = ATTR_TYPE_STRING;
}


bool TypeNumValidator::TransferToWindow() {
    try {
        wxTextCtrl *field = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
        field->SetValue(*value);
    } catch( ... ) {
        wxFAIL_MSG( _T("EnumValidator works only with wxTextCtrl") );
        return false;
    }
    return true;
}

bool TypeNumValidator::TransferFromWindow() {
    try {
        wxTextCtrl *field = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
        *value = field->GetValue();
    } catch( ... ) {
        wxFAIL_MSG( _T("EnumValidator works only with wxTextCtrl") );
        return false;
    }
    return true;

}

bool TypeNumValidator::Validate( wxWindow *parent ) {
    if( !m_validatorWindow->IsEnabled() ) {
        return true;
    }
    long ivalue;
    double fvalue;
    try {
        wxTextCtrl *field = dynamic_cast<wxTextCtrl *>( m_validatorWindow );
        wxString value = field->GetValue();
        if(type == ATTR_TYPE_INT && !value.ToLong(&ivalue, 10)) {
            wxMessageBox("value is not decimal value!", "Warning", wxOK, parent);
            return false;
        }
        if(type == ATTR_TYPE_HEX && !value.ToLong(&ivalue, 16)) {
            wxMessageBox("value is not hexadecimal value!", "Warning", wxOK, parent);
            return false;
        }
        if(type == ATTR_TYPE_INT && !value.ToDouble(&fvalue)) {
            wxMessageBox("value is not real value!", "Warning", wxOK, parent);
            return false;
        }
    } catch( ... ) {
        wxFAIL_MSG( _T("EnumValidator works only with wxTextCtrl") );
        return false;
    }
    return true;
}
*/