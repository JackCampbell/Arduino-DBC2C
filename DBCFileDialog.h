#pragma once
#include "Precompiled.h"
#include "DBCFile.h"

namespace Ui {
	class DBCFileDialog;
	class DBCFileGlobal;
};


// declare: DBCFileDialog.cpp
class DBCFileDialog : public wxDialog {
public:
	DBCFileDialog(wxWindow *parent, wxDBCFile *file, wxWindowID id = wxID_ANY);
	virtual void		InitDialog();
	virtual bool		Validate();
	virtual bool		TransferDataFromWindow();
	virtual bool		TransferDataToWindow();
	virtual void		EndModal(int retCode);
private:
	wxDBCFile *						file;
	wxString						comment;
	wxStrMap						attrs;
	int								baudrate;
	int								btr1, btr2;
	wxSharedPtr<Ui::DBCFileDialog>	ui;
};
