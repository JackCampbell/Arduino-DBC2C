#include "Utils.h"
#ifdef _WIN32
#pragma hdrstop
#endif

bool wxBoolValidator::TransferFromWindow() {
	try {
		wxCheckBox *ctrl = static_cast<wxCheckBox *>( m_validatorWindow );
		*value = ctrl->GetValue();
	} catch( ... ) {
		wxFAIL_MSG( _T("wxBoolValidator works only with wxCheckBox") );
		return false;
	}
	return true;
}

bool wxBoolValidator::TransferToWindow() {
	try {
		wxCheckBox *ctrl = static_cast<wxCheckBox *>( m_validatorWindow );
		ctrl->SetValue( *value );
	} catch( ... ) {
		wxFAIL_MSG( _T("wxBoolValidator works only with wxCheckBox") );
		return false;
	}
	return true;
}

void ListCtrlAutoColumn( wxListCtrl *view ) {
	const int count = view->GetColumnCount();

	for( int col = 0; col < count; col++ ) {
		view->SetColumnWidth( col, wxLIST_AUTOSIZE_USEHEADER );
		int wh = view->GetColumnWidth( col );

		view->SetColumnWidth( col, wxLIST_AUTOSIZE );
		int wc = view->GetColumnWidth( col );

		int max = MAX( wh, wc ) + 24;
		view->SetColumnWidth( col, max );
	}
}

bool wxHexValidator::TransferFromWindow() {
	try {
		wxTextCtrl *ctrl = static_cast<wxTextCtrl *>( m_validatorWindow );
		if( !ctrl->GetValue().Trim().ToUInt( &value, 16 ) ) {
			return false;
		}
	} catch( ... ) {
		wxFAIL_MSG( _T("wxHexValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}

bool wxHexValidator::Validate( wxWindow *parent ) {
	try {
		uint32_t test;
		wxTextCtrl *ctrl = static_cast<wxTextCtrl *>( m_validatorWindow );
		if( !ctrl->GetValue().Trim().ToUInt( &test, 16 ) ) {
			wxMessageBox( "Value is not hex decimal", "Warning", wxOK, parent );
			return false;
		}
	} catch( ... ) {
		wxFAIL_MSG( _T("wxHexValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}

bool wxHexValidator::TransferToWindow() {
	try {
		wxTextCtrl *ctrl = static_cast<wxTextCtrl *>( m_validatorWindow );
		ctrl->SetValue( wxString::Format( "%X", value ) );
	} catch( ... ) {
		wxFAIL_MSG( _T("wxHexValidator works only with wxTextCtrl") );
		return false;
	}
	return true;
}

bool wxCanValidator::GetCanID( wxString data, uint32_t &test ) const {
	int index = data.Find( ' ' );
	if( index != wxNOT_FOUND ) {
		data = data.substr( 0, index ).Trim();
	}
	if( !data.ToUInt( &test, 16 ) ) {
		return false;
	}
	return true;
}

bool wxCanValidator::Validate( wxWindow *parent ) {
	if( !m_validatorWindow->IsEnabled() ) {
		return true;
	}
	try {
		wxChoice *ctrl = static_cast<wxChoice *>( m_validatorWindow );
		wxString data = ctrl->GetStringSelection();

		uint32_t test;
		if( !GetCanID( data, test ) ) {
			wxMessageBox( "CanID not validate", "Warning", wxOK, parent );
			return false;
		}
	} catch( ... ) {
		wxFAIL_MSG( _T("wxCanValidator works only with wxChoice") );
		return false;
	}
	return true;
}

bool wxCanValidator::TransferFromWindow() {
	if( !m_validatorWindow->IsEnabled() ) {
		return true;
	}
	try {
		wxChoice *ctrl = static_cast<wxChoice *>( m_validatorWindow );
		wxString data = ctrl->GetStringSelection();
		if( !GetCanID( data, value ) ) {
			return false;
		}
	} catch( ... ) {
		wxFAIL_MSG( _T("wxCanValidator works only with wxCheckBox") );
		return false;
	}
	return true;
}

bool wxCanValidator::TransferToWindow() {
	try {
		wxChoice *ctrl = static_cast<wxChoice *>( m_validatorWindow );
		for( int i = 0; i < ctrl->GetCount(); i++ ) {
			wxString data = ctrl->GetString( i );

			uint32_t test;
			if( !GetCanID( data, test ) ) {
				continue;
			}
			if( test == value ) {
				ctrl->SetSelection( i );
				return true;
			}
		}
	} catch( ... ) {
		wxFAIL_MSG( _T("wxCanValidator works only with wxChoice") );
		return false;
	}
	return true;
}
