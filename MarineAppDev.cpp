#include "MarineApp.h"
#include "Lexer.h"
#include "DBCFile.h"
#include "DBCBitMap.h"
#include "DBCFileDialog.h"
#include "GenSignal.h"
#include "Utils.h"
#include "SignalSelector.h"
#include "FileSelectDialog.h"
#ifdef WIN32
#pragma hdrstop
#endif

void MainFrame::OnDeveloper( wxCommandEvent &event ) {
	wxLogMessage( "Test Completed\n" );
}
