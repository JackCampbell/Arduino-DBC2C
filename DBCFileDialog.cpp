#include "DBCFileDialog.h"
#include "DBCFile.h"
#include "stubs/DBCFileDialogUi.h"
#include "stubs/DBCFileGlobalUi.h"
#ifdef WIN32
#pragma hdrstop
#endif


DBCFileDialog::DBCFileDialog(wxWindow *parent, wxDBCFile *file, wxWindowID id) : wxDialog(parent, id, "File Dialog", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
	this->file = file;
	ui = new Ui::DBCFileDialog();
	ui->Setup(this);
	
	ui->comment->SetValidator( wxTextValidator( wxFILTER_NONE, &comment ) );
	ui->attrs->SetValidator( AttrMapValidator( &attrs, OBJ_TYPE_NODE, file ) );
	ui->baudrate->SetValidator( wxIntegerValidator<int>( &baudrate ) );
	ui->btr1->SetValidator( wxIntegerValidator<int>( &btr1 ) );
	ui->btr2->SetValidator( wxIntegerValidator<int>( &btr2 ) );
	ui->tabs->SetSelection(0);
}

void DBCFileDialog::InitDialog() {
	comment = file->comment;
	attrs = file->attr;
	baudrate = file->baudrate;
	btr1 = file->btr1;
	btr2 = file->btr2;
	wxDialog::InitDialog();
}

bool DBCFileDialog::Validate() {
	return wxDialog::Validate();
}

bool DBCFileDialog::TransferDataFromWindow() {
	file->defines = ui->defines->GetDefine();
	file->tables = ui->tables->GetTable();
	file->nodes = ui->nodes->GetNodes();
	file->messages = ui->messages->GetMessages();
	return wxDialog::TransferDataFromWindow();
}

bool DBCFileDialog::TransferDataToWindow() {
	ui->defines->SetDefine(file);
	ui->tables->SetTable(file);
	ui->nodes->SetNodes(file);
	ui->messages->SetMessages(file);
	return wxDialog::TransferDataToWindow();
}

void DBCFileDialog::EndModal(int retCode) {
	if(retCode == wxID_OK) {
#if 0
		wxString filename = file->GetFilename();
		if( filename.empty() ) {
			wxFileDialog dialog( this, "Open DBC file", "", "", "DBC files (*.dbc)|*.dbc", wxFD_SAVE | wxFD_OVERWRITE_PROMPT );
			if( dialog.ShowModal() == wxID_CANCEL ) {
				return;
			}
			filename = dialog.GetPath();
		} else {
			if(wxMessageBox("Will you save the file?", "Info", wxYES_NO | wxICON_INFORMATION) != wxYES) {
				return;
			}
		}
		file->baudrate = baudrate;
		file->btr1 = btr1;
		file->btr2 = btr2;
		file->comment = comment;
		file->attr = attrs;
		file->filename = filename;
		file->SaveFile( filename );
#endif
	}
	wxDialog::EndModal(retCode);
}
