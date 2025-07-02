#include "SignalSelector.h"
#include "MarineApp.h"
#include "stubs/SignalSelectorDialogUi.h"
#ifdef WIN32
#pragma hdrstop
#endif

wxSize signalSelectorFrameSize = wxSize( 700, 448 );

SignalSelectorDialog::SignalSelectorDialog( wxWindow *parent, wxVector<wxDBCFile *> files, wxWindowID id ) : wxDialog( parent, id, "Signal Selector", wxDefaultPosition, signalSelectorFrameSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	this->files = files;
	ui = new Ui::SignalSelectorDialog();
	ui->Setup( this );
	ui->search->Bind( wxEVT_TEXT, &SignalSelectorDialog::OnSearchData, this );
	ui->passive->Bind( wxEVT_LIST_ITEM_ACTIVATED, &SignalSelectorDialog::OnAddSignal, this );
	Bind( wxEVT_TOOL, &SignalSelectorDialog::OnRemoveSignal, this, wxID_REMOVE );
	Bind( wxEVT_TOOL, &SignalSelectorDialog::OnAddSignal, this, wxID_MOVE_UP );
	Bind( wxEVT_SIZE, &SignalSelectorDialog::OnSize, this );
	// Bind( wxEVT_SET_FOCUS, &SignalSelectorDialog::OnFocusDialog, this );
}

void SignalSelectorDialog::OnFocusDialog( wxFocusEvent &event ) {
	// PASS
}

void SignalSelectorDialog::InitDialog() {
	LoadSelectedList();
	LoadAllList();
	wxDialog::InitDialog();
}

void SignalSelectorDialog::OnSize( wxSizeEvent &event ) {
	signalSelectorFrameSize = event.GetSize();
	event.Skip();
}

bool SignalSelectorDialog::Validate() {
	UpdateSelectedList();
	if( selected.empty() ) {
		wxMessageBox( "Not selected", "Warning", wxOK, this );
		return false;
	}
	return wxDialog::Validate();
}

void SignalSelectorDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK ) {
#ifdef DEBUG
		for( int i = 0; i < selected.size(); i++ ) {
			wxPrintf( "Selected: %s\n", selected[i] );
			fflush( stdout );
		}
#endif
	}
	wxDialog::EndModal( retCode );
}

void SignalSelectorDialog::OnSearchData( wxCommandEvent &event ) {
	query = ui->search->GetValue().Lower().Trim();
	LoadAllList();
}

void SignalSelectorDialog::UpdateSelectedList() {
	long index = -1;
	while( true ) {
		index = ui->passive->GetNextItem( index, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
		if( index == -1 ) {
			break;
		}
		wxString path = ui->passive->GetItemText( index, 4 );
		SigPath sigpath;
		if( !sigpath.SetPath( path ) ) {
			continue;
		}
		selected.push_back( path );
	}
	LoadSelectedList();
	LoadAllList();
}

void SignalSelectorDialog::OnAddSignal( wxCommandEvent &event ) {
	UpdateSelectedList();
}

void SignalSelectorDialog::OnRemoveSignal( wxCommandEvent &event ) {
	int index = ui->active->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == -1 ) {
		return;
	}
	wxString path = ui->active->GetItemText( index, 4 );
	index = selected.Index( path );
	if( index != wxNOT_FOUND ) {
		selected.RemoveAt( index );
	}
	LoadSelectedList();
	LoadAllList();
}

void SignalSelectorDialog::LoadSelectedList() {
	ui->active->Freeze();
	ui->active->DeleteAllItems();
	for( int i = 0; i < files.size(); i++ ) {
		wxDBCFile *file = files[i];
		if( !file ) {
			continue;
		}
		for( int j = 0; j < file->NumMessage(); j++ ) {
			wxDBCMessage *message = file->GetMessage( j );
			for( int k = 0; k < message->signals.size(); k++ ) {
				wxDBCSignal *signal = message->signals[k];
				if( signal->name.EndsWith( "V" ) ) {
					// continue;
				}
				wxString strChannel = wxString::Format( "CH%d", i );
				
				wxString strSignal = signal->FindAttr("SignalLongName");
				if(strSignal.IsEmpty()) {
					strSignal = wxString::Format( "%s %s", signal->name, signal->comment ).Trim(); // signal->GetName();
				}
				
				wxString strMessage; // = signal->FindAttr("SignalLongName");
				// if(strMessage.IsEmpty())
				{
					strMessage = wxString::Format( "%03X-%s", message->ID, message->name );
				}

				SigPath sigpath;
				sigpath.channel = i;
				sigpath.file = file;
				sigpath.message = message;
				sigpath.signal = signal;
				wxString path = sigpath.GetPath();
				if( selected.Index( path ) == wxNOT_FOUND ) {
					continue;
				}
				int count = ui->active->GetItemCount();
				long index = ui->active->InsertItem( count, strChannel );
				ui->active->SetItem( index, 1, file->GetName() );
				ui->active->SetItem( index, 2, strMessage );
				ui->active->SetItem( index, 3, strSignal );
				ui->active->SetItem( index, 4, path );
			}
		}
	}
	ListCtrlAutoColumn( ui->active );
	ui->active->Thaw();
}

bool FilterKeyword( const wxArrayString &filter, const wxString &path ) {
	if( filter.IsEmpty() ) {
		return true;
	}
	return true;
}

void SignalSelectorDialog::LoadAllList() {
	ui->passive->Freeze();
	ui->passive->DeleteAllItems();
	for( int i = 0; i < files.size(); i++ ) {
		wxDBCFile *file = files[i];
		if( !file ) {
			continue;
		}
		for( int j = 0; j < file->NumMessage(); j++ ) {
			wxDBCMessage *message = file->GetMessage( j );
			for( int k = 0; k < message->signals.size(); k++ ) {
				wxDBCSignal *signal = message->signals[k];
				if( signal->name.EndsWith( "V" ) ) {
					// continue;
				}
				wxString strChannel = wxString::Format( "CH%d", i );
				
				wxString strSignal = signal->FindAttr("SignalLongName");
				if(strSignal.IsEmpty()) {
					strSignal = wxString::Format( "%s %s", signal->name, signal->comment ).Trim(); // signal->GetName();
				}
				
				wxString strMessage; // = signal->FindAttr("SignalLongName");
				// if(strMessage.IsEmpty()) 
				{
					strMessage = wxString::Format( "%03X-%s", message->ID, message->name );
				}
				
				if( !query.empty() && ( strMessage.Lower().Find( query ) == wxNOT_FOUND && strSignal.Lower().Find( query ) == wxNOT_FOUND ) ) {
					continue;
				}

				SigPath sigpath;
				sigpath.channel = i;
				sigpath.file = file;
				sigpath.message = message;
				sigpath.signal = signal;
				wxString path = sigpath.GetPath();
				
				if( selected.Index( path ) != wxNOT_FOUND ) {
					continue;
				}
				
				int count = ui->passive->GetItemCount();
				long index = ui->passive->InsertItem( count, strChannel );
				ui->passive->SetItem( index, 1, file->GetName() );
				ui->passive->SetItem( index, 2, strMessage );
				ui->passive->SetItem( index, 3, strSignal );
				ui->passive->SetItem( index, 4, path );
			}
		}
	}
	ListCtrlAutoColumn( ui->passive );
	ui->passive->Thaw();
}
