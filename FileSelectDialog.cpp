#include "FileSelectDialog.h"
#include "DBCFile.h"
#include "DBCFileDialog.h"
#include "stubs/FileSelectDialogUi.h"
#ifdef WIN32
#pragma hdrstop
#endif

wxVector<wxDBCFile *> localFiles;

FileSelectDialog::FileSelectDialog( wxWindow *parent, wxWindowID id ) : wxDialog( parent, id, "DBC Library", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER ) {
	ui = new Ui::FileSelectDialog();
	ui->Setup( this );
	Bind( wxEVT_TOOL, &FileSelectDialog::OnAddItem, this, wxID_ADD_ITEM );
	Bind( wxEVT_TOOL, &FileSelectDialog::OnTranshItem, this, wxID_REM_ITEM );
	Bind( wxEVT_LIST_ITEM_ACTIVATED, &FileSelectDialog::OnEditItem, this, wxID_ITEM_LIST );
}

FileSelectDialog::~FileSelectDialog() {
	selected.clear();
}

void FileSelectDialog::InitDialog() {
	UpdateList();
	wxDialog::InitDialog();
}

bool FileSelectDialog::Validate() {
	TransferDataFromWindow();
	if( selected.IsEmpty() ) {
		wxMessageBox( "Not selected!", "DBC Files", wxOK );
		return false;
	}
	return wxDialog::Validate();
}

void FileSelectDialog::EndModal( int retCode ) {
	if( retCode == wxID_OK ) {
		//
	}
	wxDialog::EndModal( retCode );
}

bool FileSelectDialog::TransferDataFromWindow() {
	long index = ui->files->GetNextItem( -1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED );
	if( index == -1 ) {
		selected.clear();
	} else {
		selected = ui->files->GetItemText( index, 0 );
	}
	return wxDialog::TransferDataFromWindow();
}

bool FileSelectDialog::TransferDataToWindow() {
	return wxDialog::TransferDataToWindow();
}

void FileSelectDialog::OnAddItem( wxCommandEvent &event ) {
	wxFileDialog dialog( this, "Open DBC file", "", "", "DBC files (*.dbc)|*.dbc", wxFD_OPEN | wxFD_FILE_MUST_EXIST );
	if( dialog.ShowModal() == wxID_CANCEL ) {
		return;
	}
	const wxString filename = dialog.GetPath();
	AddDBCFile( filename );
	UpdateList();
}

void FileSelectDialog::OnEditItem( wxCommandEvent &event ) {
	TransferDataFromWindow();
	if( selected.IsEmpty() ) {
		return;
	}
	wxDBCFile *file = FindDBCFile( selected );
	if( !file ) {
		return;
	}
	DBCFileDialog dialog( this, file );
	if( dialog.ShowModal() != wxID_OK ) {
		return;
	}
	UpdateList();
}

void FileSelectDialog::OnTranshItem( wxCommandEvent &event ) {
	TransferDataFromWindow();
	if( selected.IsEmpty() ) {
		return;
	}
	RemoveDBCFile( selected );
	UpdateList();
}

void FileSelectDialog::UpdateList() {
	ui->files->Freeze();
	ui->files->DeleteAllItems();
	for( int i = 0; i < localFiles.size(); i++ ) {
		ui->files->InsertItem( i, localFiles[i]->GetName() );
	}
	ui->files->SetColumnWidth( 0, 200 );
	ui->files->Thaw();
}

bool AddDBCFile( const wxString &filename ) {
	wxString name = wxFileName( filename ).GetName();
	wxDBCFile *file = FindDBCFile( name );
	if( file ) {
		wxPrintf( "Already file %s\n", name );
		return true;
	}
	file = new wxDBCFile();
	if( !file->LoadFile( filename ) ) {
		wxLogMessage( file->GetLastError() );
		delete file;
		return false;
	}
	localFiles.push_back( file );
	return true;
}

wxDBCFile *FindDBCFile( const wxString &name ) {
	for( int i = 0; i < localFiles.size(); i++ ) {
		wxDBCFile *file = localFiles[i];
		if( name == file->GetName() ) {
			return file;
		}
	}
	return nullptr;
}

bool RemoveDBCFile( const wxString &name ) {
	for( wxVector<wxDBCFile *>::iterator iter = localFiles.begin(); iter != localFiles.end(); iter++ ) {
		wxDBCFile *file = ( *iter );
		if( name == file->GetName() ) {
			localFiles.erase( iter );
			return true;
		}
	}
	return false;
}

wxArrayString DBCFileNames() {
	wxArrayString names;
	for( wxVector<wxDBCFile *>::iterator iter = localFiles.begin(); iter != localFiles.end(); iter++ ) {
		wxString name = ( *iter )->GetName();
		names.push_back( name );
	}
	return names;
}

void ReadDBCFilenames() {
	wxString filename = wxFileName( wxGetUserHome(), ".marinemods-files" ).GetFullPath();
	if( !wxFileExists( filename ) ) {
		return;
	}
#if 0
	wxTextFile stream( filename );
	if( !stream.Open() ) {
		return;
	}
	wxString dbcpath = stream.GetFirstLine();
	do {
		AddDBCFile( dbcpath );
		dbcpath = stream.GetNextLine();
	} while( !stream.Eof() );
	stream.Close();
#else
	FILE *stream = fopen( filename.ToStdString().c_str(), "rb" );
	if( !stream ) {
		return;
	}
	char line[2048];
	while( fgets( line, sizeof( line ), stream ) ) {
		wxString dbcpath = wxString( line ).Trim();
		if( !dbcpath.empty() ) {
			AddDBCFile( dbcpath );
		}
	}
	fclose( stream );
#endif
}

void SaveDBCFilenames() {
	wxString filename = wxFileName( wxGetUserHome(), ".marinemods-files" ).GetFullPath();
	wxFile stream;
	if( !stream.Open( filename, wxFile::write ) ) {
		return;
	}
	for( wxVector<wxDBCFile *>::iterator iter = localFiles.begin(); iter != localFiles.end(); iter++ ) {
		wxString dbcpath = ( *iter )->GetFilename();
		stream.Write( dbcpath + "\n" );
	}
	stream.Close();
}
